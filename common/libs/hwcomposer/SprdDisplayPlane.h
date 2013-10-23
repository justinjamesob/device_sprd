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
 ** File: SprdDisplayPlane.h          DESCRIPTION                             *
 **                                   Abstract class, father class of         *
 **                                   SprdPrimaryPlane and SprdOverlayPlane,  *
 **                                   provide some public methods and         *
 **                                   interface.                              *
 ******************************************************************************
 ******************************************************************************
 ** Author:         zhongjun.chen@spreadtrum.com                              *
 *****************************************************************************/

#ifndef _SPRD_DISPLAY_PLANE_H_
#define _SPRD_DISPLAY_PLANE_H_

#include <hardware/hardware.h>
#include <hardware/hwcomposer.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <cutils/log.h>
#include <utils/Vector.h>

#include "sprd_fb.h"
#include "gralloc_priv.h"
#include "SprdBufferManager.h"
#include "SprdHWLayerList.h"
#include "dump.h"

using namespace android;

#define PLANE_BUFFER_NUMBER 2

/*
 *  Manage DisplayPlane buffer, including
 *  ION buffer info, buffer status.
 *  Buffer status indicates whether buffer is
 *  displayed by DisplayPlane and whether is
 *  re-write for next frame.
 * */
struct BufferSlot
{
    BufferSlot()
        : mBufferState(BufferSlot::FREE),
          mTransform(0),
          mIonBuffer(NULL)
    {

    }

    enum BufferState {
        FREE = 0,
        DEQUEUEED = 1,
        QUEUEED = 2,
        RELEASE = 4,
    };

    BufferState mBufferState;
    uint32_t mTransform;
    struct bufferInfo *mIonBuffer;
};

enum PlaneFormat{
    PLANE_FORMAT_RGB888 = 1,
    PLANE_FORMAT_RGB565 = 2,
    PLANE_FORMAT_YUV420 = 3,
    PLANE_FORMAT_YUV422 = 4,
    PLANE_FORMAT_NONE = 5,
};

typedef struct DisplayPlaneContext{
    struct overlay_setting BaseContext;
    //struct overlay_display_setting DisplayContext;

} PlaneContext;

/* SprdDisplayPlane is a abstract class, responsible for manage
 * display plane
 * */
class SprdDisplayPlane
{
public:
    SprdDisplayPlane(SprdBufferManager *bm);
    virtual ~SprdDisplayPlane();

    /*
     *  Gain a available buffer for SprdDisplayPlane.
     * */
    virtual struct bufferInfo *dequeueBuffer();

    /*
     *  Display a buffer filled with content to SprdDisplayPlane.
     * */
    virtual int queueBuffer();


protected:
    virtual bool open();
    virtual bool close();

    /*
     *  Update SprdDisplayPlane display registers.
     * */
    virtual bool flush();
    //virtual bool display();

    inline PlaneContext *getPlaneContext()
    {
        return mContext;
    }

    virtual struct bufferInfo *getPlaneBuffer();
    virtual void getPlaneGeometry(unsigned int *width, unsigned int *height, int *format);

    void setGeometry(unsigned int width, unsigned int height, int format);

private:
    SprdBufferManager *mBM;
    unsigned int mWidth;
    unsigned int mHeight;
    int mFormat;
    bool InitFlag;
    PlaneContext *mContext;
    int mBufferCount;
    BufferSlot mSlots[PLANE_BUFFER_NUMBER];
    int mDisplayBufferIndex;
    //typedef Vector<int> FIFO;
    //FIFO mQueue;
    int mDebugFlag;


    inline bool InitCheck()
    {
        return InitFlag;
    }
};


#endif
