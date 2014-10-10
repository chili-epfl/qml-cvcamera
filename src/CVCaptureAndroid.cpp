/*
 * Copyright (C) 2014 EPFL
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

/**
 * @file CVCaptureAndroid.cpp
 * @brief Faster rewrite of CvCapture_Android
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2014-10-02
 */

#include"CVCaptureAndroid.h"

//*****************************************************************************
// CVCaptureAndroid implementation
//*****************************************************************************

CVCaptureAndroid::CVCaptureAndroid(){}

CVCaptureAndroid::~CVCaptureAndroid()
{
    if (activity){
        pthread_mutex_lock(&nextFrameMutex);
        dataState = NO_FRAME;
        pthread_cond_broadcast(&nextFrameCond);
        pthread_mutex_unlock(&nextFrameMutex);

        //activity->disconnect() will be automatically called inside destructor
        delete activity;
        activity = NULL;

        pthread_mutex_destroy(&nextFrameMutex);
        pthread_cond_destroy(&nextFrameCond);
    }
    delete[] rawBuffer;
}

bool CVCaptureAndroid::open(int device)
{
    delete activity;
    activity = new AndroidCameraActivity(this);
    if (activity == NULL)
        return false;

    pthread_mutex_init(&nextFrameMutex, NULL);
    pthread_cond_init (&nextFrameCond,  NULL);

    CameraActivity::ErrorCode errcode = activity->connect(device);

    if(errcode == CameraActivity::NO_ERROR)
        opened = true;
    else{
        delete activity;
        activity = NULL;
    }
    return opened;
}

bool CVCaptureAndroid::isOpened() const
{
    return opened;
}

double CVCaptureAndroid::get(int propIdx)
{
    switch (propIdx){
        case CV_CAP_PROP_FRAME_WIDTH:
            return (double)activity->getFrameWidth();
        case CV_CAP_PROP_FRAME_HEIGHT:
            return (double)activity->getFrameHeight();
        case CV_CAP_PROP_SUPPORTED_PREVIEW_SIZES_STRING:
            return (double)activity->getProperty(ANDROID_CAMERA_PROPERTY_SUPPORTED_PREVIEW_SIZES_STRING);
        case CV_CAP_PROP_PREVIEW_FORMAT:
            return (double)activity->getProperty(ANDROID_CAMERA_PROPERTY_PREVIEW_FORMAT_STRING);
        case CV_CAP_PROP_FPS:
            return (double)activity->getProperty(ANDROID_CAMERA_PROPERTY_FPS);
        case CV_CAP_PROP_EXPOSURE:
            return (double)activity->getProperty(ANDROID_CAMERA_PROPERTY_EXPOSURE);
        case CV_CAP_PROP_ANDROID_FLASH_MODE:
            return (double)activity->getProperty(ANDROID_CAMERA_PROPERTY_FLASH_MODE);
        case CV_CAP_PROP_ANDROID_FOCUS_MODE:
            return (double)activity->getProperty(ANDROID_CAMERA_PROPERTY_FOCUS_MODE);
        case CV_CAP_PROP_ANDROID_WHITE_BALANCE:
            return (double)activity->getProperty(ANDROID_CAMERA_PROPERTY_WHITE_BALANCE);
        case CV_CAP_PROP_ANDROID_ANTIBANDING:
            return (double)activity->getProperty(ANDROID_CAMERA_PROPERTY_ANTIBANDING);
        case CV_CAP_PROP_ANDROID_FOCAL_LENGTH:
            return (double)activity->getProperty(ANDROID_CAMERA_PROPERTY_FOCAL_LENGTH);
        case CV_CAP_PROP_ANDROID_FOCUS_DISTANCE_NEAR:
            return (double)activity->getProperty(ANDROID_CAMERA_PROPERTY_FOCUS_DISTANCE_NEAR);
        case CV_CAP_PROP_ANDROID_FOCUS_DISTANCE_OPTIMAL:
            return (double)activity->getProperty(ANDROID_CAMERA_PROPERTY_FOCUS_DISTANCE_OPTIMAL);
        case CV_CAP_PROP_ANDROID_FOCUS_DISTANCE_FAR:
            return (double)activity->getProperty(ANDROID_CAMERA_PROPERTY_FOCUS_DISTANCE_FAR);
        case CV_CAP_PROP_ANDROID_EXPOSE_LOCK:
            return (double)activity->getProperty(ANDROID_CAMERA_PROPERTY_EXPOSE_LOCK);
        case CV_CAP_PROP_ANDROID_WHITEBALANCE_LOCK:
            return (double)activity->getProperty(ANDROID_CAMERA_PROPERTY_WHITEBALANCE_LOCK);
        default:
            CV_Error(CV_StsOutOfRange, "Failed attempt to GET unsupported camera property.");
            return -1.0;
    }
}

bool CVCaptureAndroid::set(int propIdx, double propValue)
{
    if(opened){
        switch (propIdx){
            case CV_CAP_PROP_FRAME_WIDTH:
                activity->setProperty(ANDROID_CAMERA_PROPERTY_FRAMEWIDTH, propValue);
                break;
            case CV_CAP_PROP_FRAME_HEIGHT:
                activity->setProperty(ANDROID_CAMERA_PROPERTY_FRAMEHEIGHT, propValue);
                break;
            case CV_CAP_PROP_AUTOGRAB:
                shouldAutoGrab = (propValue != 0);
                break;
            case CV_CAP_PROP_EXPOSURE:
                activity->setProperty(ANDROID_CAMERA_PROPERTY_EXPOSURE, propValue);
                break;
            case CV_CAP_PROP_ANDROID_FLASH_MODE:
                activity->setProperty(ANDROID_CAMERA_PROPERTY_FLASH_MODE, propValue);
                break;
            case CV_CAP_PROP_ANDROID_FOCUS_MODE:
                activity->setProperty(ANDROID_CAMERA_PROPERTY_FOCUS_MODE, propValue);
                break;
            case CV_CAP_PROP_ANDROID_WHITE_BALANCE:
                activity->setProperty(ANDROID_CAMERA_PROPERTY_WHITE_BALANCE, propValue);
                break;
            case CV_CAP_PROP_ANDROID_ANTIBANDING:
                activity->setProperty(ANDROID_CAMERA_PROPERTY_ANTIBANDING, propValue);
                break;
            case CV_CAP_PROP_ANDROID_EXPOSE_LOCK:
                activity->setProperty(ANDROID_CAMERA_PROPERTY_EXPOSE_LOCK, propValue);
                break;
            case CV_CAP_PROP_ANDROID_WHITEBALANCE_LOCK:
                activity->setProperty(ANDROID_CAMERA_PROPERTY_WHITEBALANCE_LOCK, propValue);
                break;
            default:
                CV_Error( CV_StsOutOfRange, "Failed attempt to SET unsupported camera property." );
                return false;
        }

        //Only changes in frame size require camera restart
        if ((propIdx == CV_CAP_PROP_FRAME_WIDTH) || (propIdx == CV_CAP_PROP_FRAME_HEIGHT))
            cameraParamsChanged = true;

        return true;
    }
    return false;
}

bool CVCaptureAndroid::grab()
{
    if(!opened)
        return false;

    bool res = false;
    pthread_mutex_lock(&nextFrameMutex);
    if(cameraParamsChanged){
        activity->applyProperties();
        cameraParamsChanged = false;
        dataState = NO_FRAME;
    }

    if(dataState != HAS_NEW_FRAME_UNGRABBED){
        waitingNextFrame = true;
        pthread_cond_wait(&nextFrameCond, &nextFrameMutex);
    }

    if(dataState == HAS_NEW_FRAME_UNGRABBED){
        cv::swap(frameYUV420, frameYUV420next);
        dataState = HAS_FRAME_GRABBED;
        res = true;
    }

    if(pthread_mutex_unlock(&nextFrameMutex))
        return false;

    return res;
}

unsigned char* CVCaptureAndroid::retrieveFrame()
{
    if(!opened)
        return NULL;
    else
        return frameYUV420.ptr();
}

void CVCaptureAndroid::setFrame(const void* buffer, int bufferSize)
{
    int width = activity->getFrameWidth();
    int height = activity->getFrameHeight();
    int expectedSize = width*height*3/2;

    if(expectedSize != bufferSize) //Something wrong with the raw camera buffer, maybe we didn't get YUV420sp
        return;

    if(width != this->width || height != this->height){
        this->width = width;
        this->height = height;
        reallocateBuffers();
    }

    memcpy(frameYUV420next.ptr(), buffer, bufferSize);

    dataState = HAS_NEW_FRAME_UNGRABBED;
    waitingNextFrame = false;
}

void CVCaptureAndroid::reallocateBuffers()
{
    delete[] rawBuffer;

    //Allocate contiguous memory to reduce cache misses
    rawBuffer = new unsigned char[2*height*width*3/2];

    frameYUV420 = cv::Mat(height*3/2, width, CV_8UC1, rawBuffer);
    frameYUV420next = cv::Mat(height*3/2, width, CV_8UC1, rawBuffer + height*width*3/2);
}

//*****************************************************************************
// AndroidCameraActivity implementation
//*****************************************************************************

AndroidCameraActivity::AndroidCameraActivity(CVCaptureAndroid* capture)
{
    this->capture = capture;
}

bool AndroidCameraActivity::onFrameBuffer(void* buffer, int bufferSize)
{
    if(isConnected() && buffer != NULL && bufferSize > 0){
        if (capture->waitingNextFrame || capture->shouldAutoGrab){
            pthread_mutex_lock(&capture->nextFrameMutex);

            capture->setFrame(buffer, bufferSize);

            pthread_cond_broadcast(&capture->nextFrameCond);
            pthread_mutex_unlock(&capture->nextFrameMutex);
        }
        return true;
    }
    return false;
}

