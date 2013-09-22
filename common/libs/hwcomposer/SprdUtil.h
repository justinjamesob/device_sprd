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
 ** File: SprdUtil.cpp                DESCRIPTION                             *
 **                                   Transform or composer Hardware layers   *
 **                                   when display controller cannot deal     *
 **                                   with these function                     *
 ******************************************************************************
 ******************************************************************************
 ** Author:         zhongjun.chen@spreadtrum.com                              *
 *****************************************************************************/


#ifndef _SPRD_UTIL_H_
#define _SPRD_UTIL_H_

#include "Thread.h"
#include <utils/RefBase.h>
#include <cutils/log.h>
#include <semaphore.h>

#include "SprdHWLayerList.h"
#include "SprdBufferManager.h"
#include "gralloc_priv.h"

#ifdef PROCESS_VIDEO_USE_GSP
#include "sc8830/gsp_hal.h"
#endif

//#ifdef TRANSFORM_USE_DCAM
#include "sc8825/dcam_hal.h"
//#endif

#ifdef TRANSFORM_USE_GPU
#include "sc8810/gpu_transform.h"
#endif

using namespace android;

/*
 * FrameBuffer information.
 * */
typedef struct {
    int fbfd;
    int fb_width;
    int fb_height;
    void *fb_virt_addr;
    char *pFrontAddr;
    char *pBackAddr;
    int format;
} FrameBufferInfo;


#ifdef TRANSFORM_USE_DCAM
/*
 *  Transform OSD layer.
 * */
class OSDTransform: public Thread
{
public:
    OSDTransform();
    ~OSDTransform();

    void onStart(SprdHWLayer *l, struct bufferInfo *buffer);
    void onWait();

private:
    SprdHWLayer *mL;
    FrameBufferInfo *mFBInfo;
    struct bufferInfo *mBuffer;
    bool mInitFLag;
    int mDebugFlag;


    /*
     * OSDTransform thread info.
     * In order to accerate the OSD transform speed,
     * need start a new thread to the transform work,
     * Parallel with video transform work.
     * */
#ifdef _PROC_OSD_WITH_THREAD
    sem_t startSem;
    sem_t doneSem;

    virtual status_t readyToRun();
    virtual void onFirstRef();
    virtual bool threadLoop();
#endif

    int transformOSD(); 
};
#endif


/*
 *  SprdUtil is responsible for transform or composer HW layers with
 *  hardware devices, such as DCAM, GPU or GSP.
 * */
class SprdUtil
{
public:
    SprdUtil(SprdBufferManager *bufferManager, FrameBufferInfo *fbInfo)
        : mBM(bufferManager),
          mFBInfo(fbInfo),
          tmpBuffer(NULL),
#ifdef TRANSFORM_USE_DCAM
          mOSDTransform(NULL),
#endif
#ifdef PROCESS_VIDEO_USE_GSP
          mGspDev(NULL),
#endif
          mInitFlag(0),
          mDebugFlag(0)
    {

    }
    ~SprdUtil();

    bool transformLayer(SprdHWLayer *l1, SprdHWLayer *l2, 
                        struct bufferInfo *buffer1, struct bufferInfo *buffer2);

#ifdef PROCESS_VIDEO_USE_GSP
    int composerLayers(SprdHWLayer *l1, SprdHWLayer *l2, struct bufferInfo *buffer1, struct bufferInfo *buffer2);
#endif

private:
    SprdBufferManager *mBM;
    FrameBufferInfo *mFBInfo;
    struct bufferInfo *tmpBuffer;
#ifdef TRANSFORM_USE_DCAM
    sp<OSDTransform>  mOSDTransform;s
#endif
#ifdef PROCESS_VIDEO_USE_GSP
    gsp_device_t *mGspDev;
#endif
    int mInitFlag;
    int mDebugFlag;

#ifdef TRANSFORM_USE_GPU
    int getTransformInfo(SprdHWLayer *l1, SprdHWLayer *l2,
                         struct bufferInfo *buffer1, struct bufferInfo *buffer2,
                         gpu_transform_info_t *transformInfo);
#endif
#ifdef PROCESS_VIDEO_USE_GSP
    int openGSPDevice();
#endif
};


#endif
