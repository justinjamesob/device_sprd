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


#ifndef _SPRD_PRIMARY_PLANE_H_
#define _SPRD_PRIMARY_PLANE_H_

#include <cutils/log.h>
#include "SprdDisplayPlane.h"
#include "SprdBufferManager.h"
#include "SprdHWLayerList.h"
#include "SprdUtil.h"

using namespace android;

class SprdPrimaryPlane: public SprdDisplayPlane
{
public:
    SprdPrimaryPlane(FrameBufferInfo *fbInfo, SprdBufferManager *BufferManager);
    virtual ~SprdPrimaryPlane();

    /*
     *  These interfaces are from the father class SprdDisplayPlane.
     *  dequeueBuffer: gain a available buffer for SprdPrimaryPlane.
     *  queueBuffer: display a buffer.
     * */
    virtual struct bufferInfo *dequeueBuffer();
    virtual int queueBuffer();
    virtual struct bufferInfo *getPlaneBuffer();
    virtual void getPlaneGeometry(unsigned int *width, unsigned int *height, int *format);
    /**************************************************************************************/

    /*
     *  Bind OSD layer to SprdPrimaryPlane.
     * */
    void UpdatePrimaryLayer(SprdHWLayer *l);

    /*
     *  Finally display Overlay plane and Primary plane buffer.
     * */
    void display(SprdHWLayer *overlayLayer, SprdHWLayer *primaryLayer);

    /*
     * Check whether SprdPrimaryPlane is available.
     * */
    bool online();

    /*
     *  Force to disable SprdPrimaryPlane.
     * */
    void disable();

    /*
     *  enable SprdPrimaryPlane.
     * */
    void enable();

    /*
     *  Some HW layers use contiguous physcial address,
     *  not need to be transfomed.
     *  Here, let these layers displayed directly by SprdPrimaryPlane.
     * */
    bool SetDisplayParameters(hwc_layer_t *AndroidLayer);
    SprdHWLayer *getPrimaryLayer();

    int getPlaneImageFormat();
    int getPlaneBufferIndex();

    inline bool GetDirectDisplay()
    {
        return mDirectDisplayFlag;
    }

private:
    FrameBufferInfo *mFBInfo;
    SprdHWLayer *mHWLayer;
    unsigned int mPrimaryPlaneCount;
    unsigned int mFreePlaneCount;
    PlaneContext *mContext;
    struct bufferInfo *mBuffer;
    int mBufferIndex;
    int mDisplayFormat;
    bool mPlaneDisable;
    bool mDirectDisplayFlag;
    unsigned char *mPhyAddr;
    char *mVirAddr;
    int mThreadID;
    int mDebugFlag;
    int mDumpFlag;

    virtual bool flush();
    virtual bool open();
    virtual bool close();

    void InvalidatePlaneContext();

    enum PlaneFormat getPlaneFormat();


    int checkHWLayer(SprdHWLayer *l);

    inline unsigned int getFreePlane()
    {
        return mFreePlaneCount;
    }

    inline void addFreePlane()
    {
        mFreePlaneCount++;
    }

    inline void subFreePlane()
    {
        mFreePlaneCount--;
    }

};


#endif
