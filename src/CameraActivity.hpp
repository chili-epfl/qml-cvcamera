/*
 * IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
 *
 * By downloading, copying, installing or using the software you agree to this license.
 * If you do not agree to this license, do not download, install,
 * copy or use the software.
 *
 *
 *                        Intel License Agreement
 *                For Open Source Computer Vision Library
 *
 * Copyright (C) 2000, Intel Corporation, all rights reserved.
 * Third party copyrights are property of their respective owners.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   * Redistribution's of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   * Redistribution's in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   * The name of Intel Corporation may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * This software is provided by the copyright holders and contributors "as is" and
 * any express or implied warranties, including, but not limited to, the implied
 * warranties of merchantability and fitness for a particular purpose are disclaimed.
 * In no event shall the Intel Corporation or contributors be liable for any direct,
 * indirect, incidental, special, exemplary, or consequential damages
 * (including, but not limited to, procurement of substitute goods or services;
 * loss of use, data, or profits; or business interruption) however caused
 * and on any theory of liability, whether in contract, strict liability,
 * or tort (including negligence or otherwise) arising in any way out of
 * the use of this software, even if advised of the possibility of such damage.
 */

/**
 * @file CameraActivity.hpp
 * @brief We need this header because it's not installed by OpenCV
 * @author OpenCV
 * @version Taken from commit 2c4bbb313c29daf0d10c34128cd6eac1de8111de
 */

#ifndef CAMERAACTIVITY_H
#define CAMERAACTIVITY_H

#include "CameraProperties.h"

class CameraActivity {

public:

    enum ErrorCode {
        NO_ERROR=0,
        ERROR_WRONG_FRAME_SIZE,
        ERROR_WRONG_POINTER_CAMERA_WRAPPER,
        ERROR_CAMERA_CONNECTED,
        ERROR_CANNOT_OPEN_CAMERA_WRAPPER_LIB,
        ERROR_CANNOT_GET_FUNCTION_FROM_CAMERA_WRAPPER_LIB,
        ERROR_CANNOT_INITIALIZE_CONNECTION,
        ERROR_ISNT_CONNECTED,
        ERROR_JAVA_VM_CANNOT_GET_CLASS,
        ERROR_JAVA_VM_CANNOT_GET_FIELD,
        ERROR_CANNOT_SET_PREVIEW_DISPLAY,

        ERROR_UNKNOWN=255
    };

    CameraActivity();
    virtual ~CameraActivity();
    virtual bool onFrameBuffer(void* buffer, int bufferSize);

    ErrorCode connect(int cameraId = 0);
    void disconnect();
    bool isConnected() const;

    double getProperty(int propIdx);
    void setProperty(int propIdx, double value);
    void applyProperties();

    int getFrameWidth();
    int getFrameHeight();

    static void setPathLibFolder(const char* path);

private:

    void* camera;
    int frameWidth;
    int frameHeight;
};

#endif
