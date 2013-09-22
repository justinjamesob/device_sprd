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
 ** File: SprdHWComposer.h            DESCRIPTION                             *
 **                                   comunicate with SurfaceFlinger and      *
 **                                   other class objects of HWComposer       *
 ******************************************************************************
 ******************************************************************************
 ** Author:         zhongjun.chen@spreadtrum.com                              *
 *****************************************************************************/


#ifndef _SPRD_HWCOMPOSER_H
#define _SPRD_HWCOMPOSER_H

#include <hardware/hardware.h>
#include <hardware/hwcomposer.h>
#include <fcntl.h>
#include <errno.h>

#include <EGL/egl.h>

#include <utils/RefBase.h>
#include <cutils/properties.h>
#include <cutils/atomic.h>
#include <cutils/log.h>

#include "SprdBufferManager.h"
#include "SprdHWLayerList.h"
#include "SprdDisplayPlane.h"
#include "SprdOverlayPlane.h"
#include "SprdPrimaryPlane.h"
#include "SprdVsyncEvent.h"
#include "SprdUtil.h"

#ifdef OVERLAY_COMPOSER_GPU
#include "OverlayComposer/OverlayComposer.h"
#endif

#include "dump.h"

using namespace android;


class SprdHWComposer: public hwc_composer_device_t
{
public:
    SprdHWComposer()
        : mBufferManager(0), mLayerList(0),
          mOverlayPlane(0), mPrimaryPlane(0),
#ifdef OVERLAY_COMPOSER_GPU
          mOverlayComposer(NULL),
#endif
          mVsyncEvent(0), mUtil(0), mPostFrameBuffer(true),
          mForceOverlayFlag(false),
          mInitFlag(0),
          mDebugFlag(0),
          mDumpFlag(0)
    {

    }

    ~SprdHWComposer();

    /*
     *  Allocate and initialize the local objects used by HWComposer
     * */
    bool Init();

    /*
     *  Traversal layer list, and find layers which comply with SprdDisplayPlane
     *  and mark them as HWC_OVERLAY.
     * */
    int prepare(hwc_layer_list_t *list);

    /*
     *  Post layers to SprdDisplayPlane.
     * */
    int commit(hwc_display_t dpy, hwc_surface_t sur, hwc_layer_list_t* list);

    /*
     *  Registor a callback from Android Framework.
     * */
    void registerProcs(hwc_procs_t const* procs);

    /*
     *  Control vsync event, enable or disable.
     * */
    bool eventControl(int enabled);


private:
    SprdBufferManager *mBufferManager;
    SprdHWLayerList   *mLayerList;
    SprdOverlayPlane  *mOverlayPlane;
    SprdPrimaryPlane  *mPrimaryPlane;
#ifdef OVERLAY_COMPOSER_GPU
    sp<OverlayComposer> mOverlayComposer;
#endif
    sp<SprdVsyncEvent>  mVsyncEvent;
    SprdUtil          *mUtil;
    bool mPostFrameBuffer;
    bool mForceOverlayFlag;
    int mInitFlag;
    int mDebugFlag;
    int mDumpFlag;


    FrameBufferInfo *mFBInfo;
    FrameBufferInfo *getFrameBufferInfo();
    void freeFrameBufferInfo();

    inline bool PostFrameBufferOnline();


    inline sp<SprdVsyncEvent> getVsyncEventHandle()
    {
        return mVsyncEvent;
    }

    inline SprdBufferManager *getBufferManager()
    {
        return mBufferManager;
    };

    inline SprdHWLayerList *getHWLayerList()
    {
        return mLayerList;
    }

    inline SprdOverlayPlane *getOverlayPlane()
    {
        return mOverlayPlane;
    }

    inline SprdPrimaryPlane *getPrimaryPlane()
    {
        return mPrimaryPlane;
    }

    inline SprdUtil *getSprdUtilHandle()
    {
        return mUtil;
    }

    inline unsigned int roundUpToPageSize(unsigned int x)
    {
        return (x + (PAGE_SIZE-1)) & ~(PAGE_SIZE-1);
    }
};

#endif
