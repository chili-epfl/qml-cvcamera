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
 * @file CameraProperties.h
 * @brief We need this header because it's not installed by OpenCV
 * @author OpenCV
 * @version Taken from commit 0dba5bdc4a9d355e32a889501a11a4b6ec9dfe23
 */

#ifndef CAMERAPROPERTIES_H
#define CAMERAPROPERTIES_H

enum {
    ANDROID_CAMERA_PROPERTY_FRAMEWIDTH = 0,
    ANDROID_CAMERA_PROPERTY_FRAMEHEIGHT = 1,
    ANDROID_CAMERA_PROPERTY_SUPPORTED_PREVIEW_SIZES_STRING = 2,
    ANDROID_CAMERA_PROPERTY_PREVIEW_FORMAT_STRING = 3,
    ANDROID_CAMERA_PROPERTY_FPS = 4,
    ANDROID_CAMERA_PROPERTY_EXPOSURE = 5,
    ANDROID_CAMERA_PROPERTY_FLASH_MODE = 101,
    ANDROID_CAMERA_PROPERTY_FOCUS_MODE = 102,
    ANDROID_CAMERA_PROPERTY_WHITE_BALANCE = 103,
    ANDROID_CAMERA_PROPERTY_ANTIBANDING = 104,
    ANDROID_CAMERA_PROPERTY_FOCAL_LENGTH = 105,
    ANDROID_CAMERA_PROPERTY_FOCUS_DISTANCE_NEAR = 106,
    ANDROID_CAMERA_PROPERTY_FOCUS_DISTANCE_OPTIMAL = 107,
    ANDROID_CAMERA_PROPERTY_FOCUS_DISTANCE_FAR = 108,
    ANDROID_CAMERA_PROPERTY_EXPOSE_LOCK = 109,
    ANDROID_CAMERA_PROPERTY_WHITEBALANCE_LOCK = 110
};

enum {
    ANDROID_CAMERA_FLASH_MODE_AUTO = 0,
    ANDROID_CAMERA_FLASH_MODE_OFF,
    ANDROID_CAMERA_FLASH_MODE_ON,
    ANDROID_CAMERA_FLASH_MODE_RED_EYE,
    ANDROID_CAMERA_FLASH_MODE_TORCH,
    ANDROID_CAMERA_FLASH_MODES_NUM
};

enum {
    ANDROID_CAMERA_FOCUS_MODE_AUTO = 0,
    ANDROID_CAMERA_FOCUS_MODE_CONTINUOUS_VIDEO,
    ANDROID_CAMERA_FOCUS_MODE_EDOF,
    ANDROID_CAMERA_FOCUS_MODE_FIXED,
    ANDROID_CAMERA_FOCUS_MODE_INFINITY,
    ANDROID_CAMERA_FOCUS_MODE_MACRO,
    ANDROID_CAMERA_FOCUS_MODE_CONTINUOUS_PICTURE,
    ANDROID_CAMERA_FOCUS_MODES_NUM
};

enum {
    ANDROID_CAMERA_WHITE_BALANCE_AUTO = 0,
    ANDROID_CAMERA_WHITE_BALANCE_CLOUDY_DAYLIGHT,
    ANDROID_CAMERA_WHITE_BALANCE_DAYLIGHT,
    ANDROID_CAMERA_WHITE_BALANCE_FLUORESCENT,
    ANDROID_CAMERA_WHITE_BALANCE_INCANDESCENT,
    ANDROID_CAMERA_WHITE_BALANCE_SHADE,
    ANDROID_CAMERA_WHITE_BALANCE_TWILIGHT,
    ANDROID_CAMERA_WHITE_BALANCE_WARM_FLUORESCENT,
    ANDROID_CAMERA_WHITE_BALANCE_MODES_NUM
};

enum {
    ANDROID_CAMERA_ANTIBANDING_50HZ = 0,
    ANDROID_CAMERA_ANTIBANDING_60HZ,
    ANDROID_CAMERA_ANTIBANDING_AUTO,
    ANDROID_CAMERA_ANTIBANDING_OFF,
    ANDROID_CAMERA_ANTIBANDING_MODES_NUM
};

enum {
    ANDROID_CAMERA_FOCUS_DISTANCE_NEAR_INDEX = 0,
    ANDROID_CAMERA_FOCUS_DISTANCE_OPTIMAL_INDEX,
    ANDROID_CAMERA_FOCUS_DISTANCE_FAR_INDEX
};

#endif /* CAMERAPROPERTIES_H */
