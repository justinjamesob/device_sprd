
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
 ** File: SprdDisplayPlane.cpp        DESCRIPTION                             *
 **                                   Abstract class, father class of         *
 **                                   SprdPrimaryPlane and SprdOverlayPlane,  *
 **                                   provide some public methods and         *
 **                                   interface.                              *
 ******************************************************************************
 ******************************************************************************
 ** Author:         zhongjun.chen@spreadtrum.com                              *
 *****************************************************************************/

#include "SprdDisplayPlane.h"

using namespace android;

SprdDisplayPlane::SprdDisplayPlane(SprdBufferManager *bm)
    : mBM(bm),
      mWidth(1), mHeight(1), mFormat(-1),
      InitFlag(false), mContext(NULL),
      mBufferCount(PLANE_BUFFER_NUMBER),
      mDisplayBufferIndex(-1),
      mDebugFlag(0)
{
    mContext = (PlaneContext *)malloc(sizeof(PlaneContext));
    if (mContext == NULL)
    {
        ALOGE("Failed to malloc overlay_setting");
        exit(-1);
    }
}

SprdDisplayPlane::~SprdDisplayPlane()
{
    if (mContext)
    {
        free(mContext);
        mContext = NULL;
    }
}

void SprdDisplayPlane::setGeometry(unsigned int width, unsigned int height, int format)
{
    mWidth = width;
    mHeight = height;
    mFormat = format;
}

struct bufferInfo *SprdDisplayPlane::dequeueBuffer()
{
    int found = -1;

    queryDebugFlag(&mDebugFlag);

    for(int i = 0; i < mBufferCount; i++)
    {
        const int state = mSlots[i].mBufferState;

        if (state == BufferSlot::FREE)
        {
            found = i;
            continue;
        }
        //else if (state == BufferSlot::QUEUEED)
        //{
        //    /*
        //     *  Wait availeable buffer
        //     **/

        //}
        //else
        //{
        //    ALOGE("Cannot find the available ION buffer");
        //    return -ENOMEM;
        //}
    }

    mSlots[found].mBufferState = BufferSlot::DEQUEUEED;
    struct bufferInfo *buffer = mSlots[found].mIonBuffer;

    if (buffer == NULL && mWidth > 1 && mHeight > 1)
    {
        void *BufHandle = NULL;

        BufHandle = mBM->alloc(mWidth, mHeight, mFormat);
        if (BufHandle == NULL)
        {
            ALOGE("SprdDisplayPlane cannot alloc buffer");
            return NULL;
        }

        mSlots[found].mIonBuffer = static_cast<struct bufferInfo *>(BufHandle);
    }

    /*
     * The avaiable buffer has been found, restore the prevous buffer state to
     * BufferSlot::FREE
     * */
    if (mDisplayBufferIndex >= 0)
    {
        mSlots[mDisplayBufferIndex].mBufferState = BufferSlot::FREE;
    }

    mDisplayBufferIndex = found;

    //ALOGI_IF(mDebugFlag, "DisplayPlane dequeueBuffer buffer phy addr:%p",
    //         (void *)(mSlots[found].mIonBuffer->phyAddr));

    return (mSlots[found].mIonBuffer);
}

int SprdDisplayPlane::queueBuffer()
{
    int bufferIndex = mDisplayBufferIndex;

    mSlots[bufferIndex].mBufferState = BufferSlot::QUEUEED;

    return 0;
}

bool SprdDisplayPlane::flush()
{
    return true;
}

//bool SprdDisplayPlane::display()
//{
//    return true;
//}

bool SprdDisplayPlane::open()
{
    for (int i = 0; i < mBufferCount; i++)
    {
        mSlots[i].mBufferState = BufferSlot::FREE;
    }

    return true;
}

bool SprdDisplayPlane::close()
{
    for (int i = 0; i < mBufferCount; i++)
    {
        void *bufferHandle = static_cast<void *>(mSlots[i].mIonBuffer);
        mBM->free(bufferHandle);
        bufferHandle = NULL;
        mSlots[i].mBufferState = BufferSlot::RELEASE;
    }

    return true;
}

struct bufferInfo *SprdDisplayPlane::getPlaneBuffer()
{
    return NULL;
}

void SprdDisplayPlane::getPlaneGeometry(unsigned int *width, unsigned int *height, int *format)
{

}
