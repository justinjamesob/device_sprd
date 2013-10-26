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
 ** 22/09/2013    Hardware Composer   Responsible for processing some         *
 **                                   Hardware layers. These layers comply    *
 **                                   with display controller specification,  *
 **                                   can be displayed directly, bypass       *
 **                                   SurfaceFligner composition. It will     *
 **                                   improve system performance.             *
 ******************************************************************************
 ** File: SprdPrimaryPlane.h          DESCRIPTION                             *
 **                                   display RGBA format Hardware layer      *
 ******************************************************************************
 ******************************************************************************
 ** Author:         zhongjun.chen@spreadtrum.com                              *
 *****************************************************************************/



#include "SprdPrimaryPlane.h"
#include "dump.h"


using namespace android;

SprdPrimaryPlane::SprdPrimaryPlane(FrameBufferInfo *fbInfo, SprdBufferManager *BufferManager)
    : SprdDisplayPlane(BufferManager),
      mFBInfo(fbInfo),
      mHWLayer(NULL),
      mPrimaryPlaneCount(1),
      mFreePlaneCount(1),
      mContext(NULL),
      mBuffer(NULL),
      mBufferIndex(-1),
      mDisplayFormat(-1),
      mPlaneDisable(false),
      mDirectDisplayFlag(false),
      mPhyAddr(NULL),
      mVirAddr(NULL),
      mThreadID(-1),
      mDebugFlag(0),
      mDumpFlag(0)
{
    mDisplayFormat = HAL_PIXEL_FORMAT_RGBA_8888;

    SprdDisplayPlane::setGeometry(mFBInfo->fb_width, mFBInfo->fb_height, mDisplayFormat);

    mContext = SprdDisplayPlane::getPlaneContext();

    open();

    mThreadID = gettid();
}

SprdPrimaryPlane::~SprdPrimaryPlane()
{
    close();
}

struct bufferInfo *SprdPrimaryPlane::dequeueBuffer()
{
    bool ret = false;

    queryDebugFlag(&mDebugFlag);
    queryDumpFlag(&mDumpFlag);

    mBuffer = SprdDisplayPlane::dequeueBuffer();
    if (mBuffer == NULL)
    {
        ALOGE("SprdPrimaryPlane cannot get ION buffer");
        return NULL;
    }


    mFreePlaneCount = 1;

    mBufferIndex = (mBufferIndex + 1) % PLANE_BUFFER_NUMBER;


    enable();

    ALOGI_IF(mDebugFlag, "SprdPrimaryPlane::dequeueBuffer phy addr:%p", (void *)(mBuffer->phyAddr));
    return mBuffer;
}

int SprdPrimaryPlane::queueBuffer()
{
    SprdDisplayPlane::queueBuffer();

    flush();

    mFreePlaneCount = 0;


    return 0;
}

void SprdPrimaryPlane::UpdatePrimaryLayer(SprdHWLayer *l)
{
    int ret = checkHWLayer(l);

    if (ret != 0)
    {
        ALOGE("Check Sprd PrimaryLayer failed");
        return;
    }
    
    mHWLayer = l;

    mDirectDisplayFlag = false;

    mPhyAddr = NULL;

    mVirAddr = NULL;
}

bool SprdPrimaryPlane::SetDisplayParameters(hwc_layer_t *AndroidLayer)
{
    if (AndroidLayer == NULL)
    {
        ALOGI_IF(mDebugFlag, "SprdHWLayer is NULL");
        mDirectDisplayFlag = false;
        return false;
    }

    const native_handle_t *pNativeHandle = AndroidLayer->handle;
    struct private_handle_t *privateH = (struct private_handle_t *)pNativeHandle;

    if (privateH->format == HAL_PIXEL_FORMAT_YCbCr_420_SP ||
        privateH->format == HAL_PIXEL_FORMAT_YCrCb_420_SP ||
        privateH->format == HAL_PIXEL_FORMAT_YV12)
    {
        mDirectDisplayFlag = false;
        return false;
    }

    if (privateH && (!(privateH->flags) & (private_handle_t::PRIV_FLAGS_USES_PHY)))
    {
        ALOGI("Current transform device and display device cannot support virtual adress");
        mDirectDisplayFlag = false;
    }

    if (privateH && privateH->format == HAL_PIXEL_FORMAT_RGB_565)
    {
        mDisplayFormat = HAL_PIXEL_FORMAT_RGB_565;
    }
    else if (privateH && privateH->format == HAL_PIXEL_FORMAT_RGBA_8888)
    {
        mDisplayFormat = HAL_PIXEL_FORMAT_RGBA_8888;
    }

    if (AndroidLayer->transform != 0)
    {
        ALOGI_IF(mDebugFlag, "This layer need to be transformed");
        mDirectDisplayFlag = false;
        return false;
    }

    mDirectDisplayFlag = true;

    mPhyAddr = (unsigned char *)(privateH->phyaddr);

    mVirAddr = (char *)(privateH->base);

    return mDirectDisplayFlag;
}

void SprdPrimaryPlane::display(SprdHWLayer *overlayLayer, SprdHWLayer *primaryLayer)
{
    int PlaneType = 0;
    struct overlay_display_setting displayContext;

    displayContext.display_mode = SPRD_DISPLAY_OVERLAY_ASYNC;

    if (overlayLayer)
    {
        PlaneType |= SPRD_LAYERS_IMG;
    }

    if (primaryLayer && mPlaneDisable == false)
    {
        PlaneType |= SPRD_LAYERS_OSD;
        if (GetDirectDisplay())
        {
            displayContext.display_mode = SPRD_DISPLAY_OVERLAY_SYNC;
        }
    }

    displayContext.layer_index = PlaneType;
    displayContext.rect.x = 0;
    displayContext.rect.y = 0;
    displayContext.rect.w = mFBInfo->fb_width;
    displayContext.rect.h = mFBInfo->fb_height;

    ALOGI_IF(mDebugFlag, "SPRD_FB_DISPLAY_OVERLAY %d", PlaneType);

    ioctl(mFBInfo->fbfd, SPRD_FB_DISPLAY_OVERLAY, &displayContext);

}

void SprdPrimaryPlane::disable()
{
    mPlaneDisable = true;
}

void SprdPrimaryPlane::enable()
{
    mPlaneDisable = false;
}

bool SprdPrimaryPlane::online()
{

    if (mPlaneDisable)
    {
        ALOGI_IF(mDebugFlag, "SprdPrimaryPlane has been disabled");
        return false;
    }

    if (mFreePlaneCount == 0)
    {
        ALOGI_IF(mDebugFlag, "SprdPrimaryPlanens is not avaiable");
        return false;
    }

    if (mHWLayer == NULL)
    {
        ALOGI("SprdPrimaryPlane SprdHWLayer is NULL");
        return false;
    }

    return true;
}

int SprdPrimaryPlane::checkHWLayer(SprdHWLayer *l)
{
    if (l == NULL)
    {
        ALOGE("SprdPrimaryPlane Failed to check the list, SprdHWLayer is NULL");
        return -1;
    }

    if (l->getPlaneType() != PLANE_PRIMARY)
    {
        ALOGI("Cannot find PLANE_OVERLAY flag layer");
        return -1;
    }


    return 0;
}

enum PlaneFormat SprdPrimaryPlane::getPlaneFormat()
{
    enum PlaneFormat format;
    int displayFormat = mDisplayFormat;

    switch(displayFormat)
    {
        case HAL_PIXEL_FORMAT_RGBX_8888:
        case HAL_PIXEL_FORMAT_RGBA_8888:
            format = PLANE_FORMAT_RGB888;
            break;
        case HAL_PIXEL_FORMAT_RGB_565:
            format = PLANE_FORMAT_RGB565;
            break;
        default:
            format = PLANE_FORMAT_NONE;
            break;
    }

    return format;
}

bool SprdPrimaryPlane::flush()
{
    int localThreadID = -1;
    enum PlaneFormat format;
    struct overlay_setting *BaseContext = &(mContext->BaseContext);
    char *CurrentOverlayVirAddr = NULL;

    queryDebugFlag(&mDebugFlag);
    queryDumpFlag(&mDumpFlag);

    InvalidatePlaneContext();

    BaseContext->layer_index = SPRD_LAYERS_OSD;

    format = getPlaneFormat();
    if (format == PLANE_FORMAT_RGB888)
    {
        BaseContext->data_type = SPRD_DATA_FORMAT_RGB888;
        BaseContext->y_endian = SPRD_DATA_ENDIAN_B0B1B2B3;
        BaseContext->uv_endian = SPRD_DATA_ENDIAN_B0B1B2B3;
        BaseContext->rb_switch = 1;
    }
    else if (format ==  PLANE_FORMAT_RGB565)
    {
        BaseContext->data_type = SPRD_DATA_FORMAT_RGB565;
        BaseContext->y_endian = SPRD_DATA_ENDIAN_B0B1B2B3;
        BaseContext->uv_endian = SPRD_DATA_ENDIAN_B0B1B2B3;
        BaseContext->rb_switch = 0;
    }

    BaseContext->rect.x = 0;
    BaseContext->rect.y = 0;
    BaseContext->rect.w = mFBInfo->fb_width;
    BaseContext->rect.h = mFBInfo->fb_height;


    /*
     *  Here, it is a workaround method.
     *  Mali posting FrameBuffer in another thread of
     *  OverlayComposer maybe access the resource released
     *  by Android framework.
     *  Just do the safely thread check.
     *  We just permit directly displaying of OSD layer
     *  is used in the same thread.
     * */
    localThreadID = gettid();

    if ((mThreadID == localThreadID) &&
        GetDirectDisplay() &&
        (mPhyAddr != NULL && mVirAddr != NULL))
    {
        BaseContext->buffer = mPhyAddr;
        CurrentOverlayVirAddr = mVirAddr;
    }
    else
    {
        if (mBuffer == NULL)
        {
            ALOGE("SprdPrimaryPlane: Cannot get the display buffer");
            return false;
        }
        BaseContext->buffer = (unsigned char *)(mBuffer->phyAddr);
        CurrentOverlayVirAddr = (char *)(mBuffer->virAddr);
    }

    ALOGI_IF(mDebugFlag, "SprdPrimaryPlane::flush  osd overlay parameter datatype = %d, x = %d, y = %d, w = %d, h = %d, buffer = 0x%08x",
             BaseContext->data_type,
             BaseContext->rect.x,
             BaseContext->rect.y,
             BaseContext->rect.w,
             BaseContext->rect.h,
             (unsigned int)BaseContext->buffer);

    if (mDumpFlag)
    {
        dumpOverlayImage(NULL, BaseContext, CurrentOverlayVirAddr, mDumpFlag);
    }

    if (ioctl(mFBInfo->fbfd, SPRD_FB_SET_OVERLAY, BaseContext) == -1)
    {
        ALOGE("fail osd SPRD_FB_SET_OVERLAY");
        ioctl(mFBInfo->fbfd, SPRD_FB_SET_OVERLAY, BaseContext);//Fix ME later
    }

    return true;
}

bool SprdPrimaryPlane::open()
{
    SprdDisplayPlane::open();
    SprdDisplayPlane::dequeueBuffer();
    SprdDisplayPlane::dequeueBuffer();

    mPrimaryPlaneCount = 1;
    mFreePlaneCount = 1;

    return true;
}

bool SprdPrimaryPlane::close()
{
    SprdDisplayPlane::close();

    mFreePlaneCount = 0;

    return true;
}

void SprdPrimaryPlane::InvalidatePlaneContext()
{
    memset(&(mContext->BaseContext), 0x00, sizeof(struct overlay_setting));
}

SprdHWLayer *SprdPrimaryPlane::getPrimaryLayer()
{
    if (mHWLayer == NULL)
    {
        ALOGE("PrimaryLayer is NULL");
        return NULL;
    }

    return mHWLayer;
}

int SprdPrimaryPlane::getPlaneBufferIndex()
{
    return mBufferIndex;
}

struct bufferInfo *SprdPrimaryPlane::getPlaneBuffer()
{
    if (mBuffer == NULL)
    {
        ALOGE("Failed to get hte SprdPrimaryPlane buffer");
        return NULL;
    }

    return mBuffer;
}


void SprdPrimaryPlane::getPlaneGeometry(unsigned int *width, unsigned int *height, int *format)
{
    if (width == NULL || height == NULL || format == NULL)
    {
        ALOGE("getPlaneGeometry, input parameters are NULL");
        return;
    }

    *width = mFBInfo->fb_width;
    *height = mFBInfo->fb_height;
    *format = mDisplayFormat;
}
