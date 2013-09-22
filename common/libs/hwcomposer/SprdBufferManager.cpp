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
 ** File: SprdBufferManager.cpp       DESCRIPTION                             *
 **                                   Manage HWC buffer allocation and free.  *
 ******************************************************************************
 ******************************************************************************
 ** Author:         zhongjun.chen@spreadtrum.com                              *
 *****************************************************************************/


#include "SprdBufferManager.h"

using namespace android;

MutexLock::MutexLock()
{
    pthread_mutex_init(&mMutex, NULL);
}

MutexLock::~MutexLock()
{
    pthread_mutex_destroy(&mMutex);
}

void MutexLock::Lock()
{
    pthread_mutex_lock(&mMutex);
}

void MutexLock::UnLock()
{
    pthread_mutex_unlock(&mMutex);
}


SprdBufferManager::SprdBufferManager()
{

}

SprdBufferManager::~SprdBufferManager()
{
    for (BufferList::iterator it = mList.begin();
         it != mList.end(); it++)
    {
        struct bufferInfo *buffer = *it;
        free((void *)buffer);
        buffer = NULL;
    }
    mList.clear();
}

void SprdBufferManager::Lock()
{
    mLock.Lock();
}

void SprdBufferManager::UnLock()
{
    mLock.UnLock();
}

unsigned int SprdBufferManager::CalculateBufferSize(unsigned int width, unsigned int height, int format)
{
    unsigned int size = 0;
    float bytesPerPixel;

    switch(format)
    {
        case HAL_PIXEL_FORMAT_RGBA_8888:
            bytesPerPixel = 4;
            break;
        case HAL_PIXEL_FORMAT_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_YCrCb_420_SP:
            bytesPerPixel = 1.5;
            break;
        case HAL_PIXEL_FORMAT_YCbCr_422_SP:
        case HAL_PIXEL_FORMAT_YCbCr_422_P:
        case HAL_PIXEL_FORMAT_YCbCr_422_I:
        case HAL_PIXEL_FORMAT_CbYCrY_422_I:
        case HAL_PIXEL_FORMAT_YCrCb_422_SP:
            bytesPerPixel = 2;
            break;
        case PRIVATE_HALF_BUFFER:
            bytesPerPixel = 0.5;
            break;
        default:
            bytesPerPixel = 0;
    }

    size = RoundUpToPageSize((float)(width * height) * bytesPerPixel);

    return size;
}

void *SprdBufferManager::alloc(unsigned int width, unsigned int height, int format)
{
    unsigned int bufferSize;
    struct bufferInfo *buffer = NULL;

    Lock();

    bufferSize = CalculateBufferSize(width, height, format);

    buffer = (struct bufferInfo *)malloc(sizeof(struct bufferInfo));
    if (buffer == NULL)
    {
        ALOGE("malloc buffer failed");
        UnLock();
        return NULL;
    }

    buffer->IonHeap = new MemoryHeapIon(SPRD_ION_DEV, bufferSize,
                              MemoryHeapBase::NO_CACHING, (1 << ION_HEAP_CARVEOUT_ID1));
    if (buffer->IonHeap == NULL)
    {
        ALOGE("Failed to get IonHeap");
        return NULL;
    }

    int fd = buffer->IonHeap->getHeapID();
    if (fd < 0)
    {
        ALOGE("Failed to get the heap ION fd");
        UnLock();
        return NULL;
    }

    int size;
    int ret = buffer->IonHeap->get_phy_addr_from_ion(&buffer->phyAddr, &size);
    if (ret)
    {
        ALOGE("Failed to get the ION phy addr");
        UnLock();
        return NULL;
    }

    buffer->virAddr = buffer->IonHeap->base();
    buffer->bufSize = bufferSize;
    buffer->format = format;

    mList.push_back(buffer);

    ALOGI("Allocate ION buffer phy addr:%p", (void *)buffer->phyAddr);

    UnLock();

    return (void *)buffer;
}

void SprdBufferManager::free(void *bufHandle)
{
    struct bufferInfo *buffer = NULL;

    Lock();

    buffer = (struct bufferInfo *)bufHandle;

    if (buffer == NULL)
    {
        ALOGE("Buffer is NULL, no need free");
        UnLock();
        return;
    }

    buffer->IonHeap.clear();

    free(buffer);

    ALOGI("Free buffer phy addr:%p", (void *)buffer->phyAddr);

    UnLock();
}
