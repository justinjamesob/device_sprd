/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          Module              DESCRIPTION                             *
 ** 16/08/2013    Hardware Composer   Add a new feature to Harware composer,  *
 **                                   verlayComposer use GPU to do the        *
 **                                   Hardware layer blending on Overlay      *
 **                                   buffer, and then post the OVerlay       *
 **                                   buffer to Display                       *
 ******************************************************************************
 ** Author:         fushou.yang@spreadtrum.com                                *
 **                 zhongjun.chen@spreadtrum.com                              *
 *****************************************************************************/



#include <hardware/hwcomposer.h>
#include <hardware/hardware.h>
#include <sys/ioctl.h>
#include "sprd_fb.h"
#include "SyncThread.h"

#include "OverlayNativeWindow.h"
#include "dump.h"


namespace android {

static SprdPrimaryPlane *DisplayPlane = NULL;
static unsigned int *Width = NULL;
static unsigned int *Height = NULL;
static int *Format = NULL;
static GraphicBufferInfo *gGFXBufferInfo[NUM_FRAME_BUFFERS] = {NULL, NULL};
static int mDebugFlag = 0;

OverlayNativeWindow::OverlayNativeWindow(SprdPrimaryPlane *displayPlane)
    : mDisplayPlane(displayPlane),
      mWidth(1), mHeight(1), mFormat(-1),
      mNumBuffers(NUM_FRAME_BUFFERS),
      mNumFreeBuffers(NUM_FRAME_BUFFERS), mBufferHead(0),
      mUpdateOnDemand(false)
{

}

bool OverlayNativeWindow::Init()
{
    DisplayPlane = mDisplayPlane;

    DisplayPlane->getPlaneGeometry(&mWidth, &mHeight, &mFormat);

    Width = &mWidth;
    Height = &mHeight;
    Format = &mFormat;

    for (unsigned int i = 0; i < mNumBuffers; i++)
    {
        gGFXBufferInfo[i] = &(mGFXBufferInfo[i]);
    }

    ANativeWindow::setSwapInterval = setSwapInterval;
    ANativeWindow::dequeueBuffer = dequeueBuffer;
    ANativeWindow::lockBuffer = lockBuffer;
    ANativeWindow::queueBuffer = queueBuffer;
    ANativeWindow::query = query;
    ANativeWindow::perform = perform;

    return true;
}

OverlayNativeWindow::~OverlayNativeWindow()
{
    int bufferCount = NUM_FRAME_BUFFERS;

    for (int i = 0; i < bufferCount; i++)
    {
        DestroyGraphicBuffer(gGFXBufferInfo[i]);
    }
}

private_handle_t *OverlayNativeWindow::wrapBuffer(unsigned int w, unsigned int h, int format, GraphicBufferInfo *GFXBufferInfo)
{
    private_handle_t *pH;

    ump_handle ump_h = NULL;

    uint32_t size;
    uint32_t stride;

    getSizeStride(w, h, format, size, stride);

    ump_h = ump_handle_create_from_phys_block(GFXBufferInfo->phyAddr, size);
    if (ump_h == NULL)
    {
        ALOGE("ump_h create fail");
        return NULL;
    }
    ALOGI("create phy buffer 0x%x, w is %d  h is %d size is %d line is %d",
        GFXBufferInfo->phyAddr, w, h, size, __LINE__);

    pH = new private_handle_t(private_handle_t::PRIV_FLAGS_USES_UMP,
                             size, (unsigned int)(GFXBufferInfo->virAddr),
                             private_handle_t::LOCK_STATE_MAPPED,
                             ump_secure_id_get(ump_h),
                             ump_h);
    pH->width = stride;
    pH->height = h;
    pH->format = format;
    pH->phyaddr = GFXBufferInfo->phyAddr;

    return pH;

}

void OverlayNativeWindow::unWrapBuffer(private_handle_t *h)
{
    ump_free_handle_from_mapped_phys_block((ump_handle)h->ump_mem_handle);
}

sp<NativeBuffer> OverlayNativeWindow::CreateGraphicBuffer(GraphicBufferInfo *GFXBufferInfo, struct bufferInfo *buffer)
{
    sp<NativeBuffer> nativeBuffer = NULL;

    GFXBufferInfo->phyAddr = buffer->phyAddr;
    GFXBufferInfo->virAddr = static_cast<void *>(buffer->virAddr);
    GFXBufferInfo->bufferSize = buffer->bufSize;

    GFXBufferInfo->bufHandle = wrapBuffer(*Width, *Height, *Format, GFXBufferInfo);
    if (GFXBufferInfo->bufHandle == NULL)
    {
        ALOGE("wrapBuffer Failed, can NOT get wrap handle");
        return NULL;
    }

    nativeBuffer = new NativeBuffer(GFXBufferInfo->bufHandle->width,
                                    GFXBufferInfo->bufHandle->height,
                                    GFXBufferInfo->bufHandle->format, NULL);
    nativeBuffer->handle = GFXBufferInfo->bufHandle;
    nativeBuffer->stride = GFXBufferInfo->bufHandle->width;

    return nativeBuffer;
}

void OverlayNativeWindow::DestroyGraphicBuffer(GraphicBufferInfo *GFXBuffer)
{
    if (GFXBuffer != NULL)
    {
        unWrapBuffer(GFXBuffer->bufHandle);
    }
}


int OverlayNativeWindow::dequeueBuffer(ANativeWindow* window,
        ANativeWindowBuffer** buffer)
{
    OverlayNativeWindow* self = getSelf(window);
    Mutex::Autolock _l(self->mutex);
    int index = -1;

    // wait for a free buffer
    while (!self->mNumFreeBuffers) {
        self->mCondition.wait(self->mutex);
    }

    struct bufferInfo *IONBuffer = DisplayPlane->dequeueBuffer();
    if (buffer == NULL)
    {
        ALOGE("Failed to get the Display plane buffer");
        return NULL;
    }

    index = DisplayPlane->getPlaneBufferIndex();
    // get this buffer
    self->mNumFreeBuffers--;
    self->mCurrentBufferIndex = index;

    if (self->buffers[index] == NULL)
    {
        self->buffers[index] = CreateGraphicBuffer(gGFXBufferInfo[index], IONBuffer);
        if (self->buffers[index] == NULL)
        {
            ALOGE("Failed to CreateGraphicBuffer");
            return -1;
        }
    }
    *buffer = self->buffers[index].get();

    queryDebugFlag(&mDebugFlag);
    ALOGI_IF(mDebugFlag, "OverlayNativeWindow::dequeueBuffer phy addr:%p", (void *)(gGFXBufferInfo[index]->phyAddr));
    return 0;
}


int OverlayNativeWindow::queueBuffer(ANativeWindow* window,
        ANativeWindowBuffer* buffer)
{
    uint32_t current_overlay_paddr = 0;
    OverlayNativeWindow* self = getSelf(window);
    Mutex::Autolock _l(self->mutex);

    DisplayPlane->queueBuffer();

    DisplayPlane->display(NULL, DisplayPlane->getPrimaryLayer());

    //postSem();

    const int index = self->mCurrentBufferIndex;
    self->front = static_cast<NativeBuffer*>(buffer);
    self->mNumFreeBuffers++;
    self->mCondition.broadcast();


    queryDebugFlag(&mDebugFlag);
    ALOGI_IF(mDebugFlag, "OverlayNativeWindow::queueBuffer phy addr:%p", (void *)(gGFXBufferInfo[index]->phyAddr));
    return 0;
}

int OverlayNativeWindow::lockBuffer(ANativeWindow* window,
        ANativeWindowBuffer* buffer)
{
    OverlayNativeWindow* self = getSelf(window);
    Mutex::Autolock _l(self->mutex);

    const int index = self->mCurrentBufferIndex;

    // wait that the buffer we're locking is not front anymore
    while (self->front == buffer) {
        self->mCondition.wait(self->mutex);
    }

    return 0;
}

int OverlayNativeWindow::query(const ANativeWindow* window,
        int what, int* value)
{
    const OverlayNativeWindow* self = getSelf(window);
    Mutex::Autolock _l(self->mutex);


    //ALOGI("%s %d",__func__,__LINE__);

    switch (what) {
        case NATIVE_WINDOW_WIDTH:
            *value = *Width;
            return NO_ERROR;
        case NATIVE_WINDOW_HEIGHT:
            *value = *Height;
            return NO_ERROR;
        case NATIVE_WINDOW_FORMAT:
            *value = *Format;
            return NO_ERROR;
        case NATIVE_WINDOW_CONCRETE_TYPE:
            *value = NATIVE_WINDOW_FRAMEBUFFER;
            return NO_ERROR;
        case NATIVE_WINDOW_QUEUES_TO_WINDOW_COMPOSER:
            *value = 0;
            return NO_ERROR;
        case NATIVE_WINDOW_DEFAULT_WIDTH:
            *value = *Width;
            return NO_ERROR;
        case NATIVE_WINDOW_DEFAULT_HEIGHT:
            *value = *Height;
            return NO_ERROR;
        case NATIVE_WINDOW_TRANSFORM_HINT:
            *value = 0;
            return NO_ERROR;
    }
    *value = 0;
    return BAD_VALUE;

}

int OverlayNativeWindow::perform(ANativeWindow* window,
        int operation, ...)
{
    switch (operation) {
        case NATIVE_WINDOW_CONNECT:
        case NATIVE_WINDOW_DISCONNECT:
        case NATIVE_WINDOW_SET_USAGE:
        case NATIVE_WINDOW_SET_BUFFERS_GEOMETRY:
        case NATIVE_WINDOW_SET_BUFFERS_DIMENSIONS:
        case NATIVE_WINDOW_SET_BUFFERS_FORMAT:
        case NATIVE_WINDOW_SET_BUFFERS_TRANSFORM:
        case NATIVE_WINDOW_API_CONNECT:
        case NATIVE_WINDOW_API_DISCONNECT:
            // TODO: we should implement these
            return NO_ERROR;

        case NATIVE_WINDOW_LOCK:
        case NATIVE_WINDOW_UNLOCK_AND_POST:
        case NATIVE_WINDOW_SET_CROP:
        case NATIVE_WINDOW_SET_BUFFER_COUNT:
        case NATIVE_WINDOW_SET_BUFFERS_TIMESTAMP:
        case NATIVE_WINDOW_SET_SCALING_MODE:
            return INVALID_OPERATION;
    }
    return NAME_NOT_FOUND;
}


int OverlayNativeWindow::setSwapInterval(
        ANativeWindow* window, int interval)
{
    //hwc_composer_device_t* fb = getSelf(window)->hwc_dev;
  //  return fb->setSwapInterval(fb, interval);
    return 0;
}

};
