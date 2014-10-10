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
 * @file CVCaptureAndroid.h
 * @brief Faster rewrite of CvCapture_Android
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2014-10-02
 */

#ifndef CVCAPTUREANDROID_H
#define CVCAPTUREANDROID_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio_c.h>

#include <pthread.h>

#include "CameraActivity.hpp"

class AndroidCameraActivity;

/**
 * @brief Gets camera frames from the Android camera API in an orderly manner
 */
class CVCaptureAndroid {

public:

    /**
     * @brief Creates a new capture
     */
    CVCaptureAndroid();

    /**
     * @brief Destroys this capture, stopping and deleting the camera API communicator on the way
     */
    virtual ~CVCaptureAndroid();

    /**
     * @brief Connects to a new camera
     *
     * @param device Camera ID
     *
     * @return Whether connection was successful
     */
    bool open(int device);

    /**
     * @brief Gets a low-level camera property
     *
     * @param propIdx ID of the property
     *
     * @return Value of the property
     */
    double get(int propIdx);

    /**
     * @brief Sets a low-level camera property
     *
     * @param propIdx Property ID
     * @param propVal New property value
     *
     * @return Whether setting the property was succesful
     */
    bool set(int propIdx, double propVal);

    /**
     * @brief Prepares a new frame for retrieval if it's ready
     *
     * @return Whether grabbing was successful
     */
    bool grab();

    /**
     * @brief Gets the address of the new frame
     *
     * @return Address of the new frame
     */
    unsigned char* retrieveFrame();

    /**
     * @brief Gets whether a camera is open
     *
     * @return Whether a camera is open
     */
    bool isOpened() const;

protected:

    CameraActivity* activity = NULL;            ///< Camera API communicator
    int width = -1;                             ///< Width of this capture's buffer
    int height = -1;                            ///< Height of this capture's buffer
    unsigned char* rawBuffer = NULL;            ///< Pointer to the raw frame buffers
    cv::Mat frameYUV420;                        ///< CV image that points to the current frame
    cv::Mat frameYUV420next;                    ///< CV image that points to the place that the next frame will be copied to

    /**
     * @brief Updates this capture's image buffers with the new frame data
     *
     * @param buffer New frame
     * @param bufferSize Number of bytes in the new frame
     */
    void setFrame(const void* buffer, int bufferSize);

private:

    /**
     * @brief Describes the state of the capture
     */
    enum DataState
    {
        NO_FRAME = 0,
        HAS_NEW_FRAME_UNGRABBED,
        HAS_FRAME_GRABBED
    };

    volatile DataState dataState = NO_FRAME;    ///< Current capture state

    bool opened = false;                        ///< Whether the camera is currently open
    bool cameraParamsChanged = false;           ///< Indicates that the buffer should be reallocated

    pthread_mutex_t nextFrameMutex;             ///< Locks the access to the image buffer
    pthread_cond_t nextFrameCond;               ///< Waiting condition for the image buffer
    volatile bool waitingNextFrame = false;     ///< Whether the capture is waiting on the condition for the image buffer
    volatile bool shouldAutoGrab = false;       ///< Whether the Camera API communicator should pass the buffer to this object automatically

    /**
     * @brief Frees the old raw image buffers and allocates new raw image buffers for the new image size
     */
    void reallocateBuffers();

    friend class AndroidCameraActivity;
};

/**
 * @brief Implementation of the Android camera API communicator with the thread-safe code that provides us with the image buffer
 */
class AndroidCameraActivity : public CameraActivity {

public:

    /**
     * @brief Creates a new Android camera API communicator
     *
     * @param capture Object to give the raw image buffer to
     */
    AndroidCameraActivity(CVCaptureAndroid* capture);

    /**
     * @brief Called each time a new frame arrives
     *
     * @param buffer Raw frame buffer
     * @param bufferSize Size of the buffer
     *
     * @return Whether buffer was passed to the Capture object successfully
     */
    virtual bool onFrameBuffer(void* buffer, int bufferSize);

private:

    CVCaptureAndroid* capture; ///< Object to privde the raw image buffer to
};

#endif /* CVCAPTUREANDROID_H */
