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
 * @file CameraThread.cpp
 * @brief Listens to the camera in a separate thread
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2014-09-23
 */

#include"CameraThread.h"

//*****************************************************************************
// CameraTask implementation
//*****************************************************************************

CameraTask::CameraTask(BetterVideoCapture* camera, QVideoFrame* videoFrame, unsigned char* cvImageBuf, int width, int height)
{
    this->running = true;
    this->camera = camera;
    this->videoFrame = videoFrame;
    this->cvImageBuf = cvImageBuf;
    this->width = width;
    this->height = height;
}

CameraTask::~CameraTask()
{
    //Leave camera and videoFrame alone, they will be destroyed elsewhere
}

void CameraTask::stop()
{
    running = false;
}

void CameraTask::convertUVsp2UVp(unsigned char* __restrict srcptr, unsigned char* __restrict dstptr, int stride)
{
    for(int i=0;i<stride;i++){
        dstptr[i]           = srcptr[i*2];
        dstptr[i + stride]  = srcptr[i*2 + 1];
    }
}

void CameraTask::doWork()
{

#if defined(QT_DEBUG) && !defined(ANDROID)
    QElapsedTimer timer;
    float fps = 0.0f;
    int millisElapsed = 0;
    int millis;
    timer.start();
#endif

    if(videoFrame)
        videoFrame->map(QAbstractVideoBuffer::ReadOnly);

#ifndef ANDROID //Assuming desktop, RGB camera image and RGBA QVideoFrame
    cv::Mat screenImage;
    if(videoFrame)
        screenImage = cv::Mat(height,width,CV_8UC4,videoFrame->bits());
#endif

    while(running && videoFrame != NULL && camera != NULL){
        if(!camera->grabFrame())
            continue;
        unsigned char* cameraFrame = camera->retrieveFrame();

        //Get camera image into screen frame buffer
        if(videoFrame){

#ifdef ANDROID //Assume YUV420sp camera image and YUV420p QVideoFrame

            //Copy over Y channel
            memcpy(videoFrame->bits(),cameraFrame,height*width);

            //Convert semiplanar UV to planar UV
            convertUVsp2UVp(cameraFrame + height*width, videoFrame->bits() + height*width, height/2*width/2);

#else //Assuming desktop, RGB camera image and RGBA QVideoFrame
            cv::Mat tempMat(height,width,CV_8UC3,cameraFrame);
            cv::cvtColor(tempMat,screenImage,cv::COLOR_RGB2RGBA);
#endif

        }

        //Export camera image
        if(cvImageBuf){

#ifdef ANDROID //Assume YUV420sp camera image
            memcpy(cvImageBuf,cameraFrame,height*width*3/2);
#else //Assuming desktop, RGB camera image
            memcpy(cvImageBuf,cameraFrame,height*width*3);
#endif

        }

        emit imageReady();

#if defined(QT_DEBUG) && !defined(ANDROID)
        millis = (int)timer.restart();
        millisElapsed += millis;
        fps = CAM_FPS_RATE*fps + (1.0f - CAM_FPS_RATE)*(1000.0f/millis);
        if(millisElapsed >= CAM_FPS_PRINT_PERIOD){
            qDebug("Camera is running at %f FPS",fps);
            millisElapsed = 0;
        }
#endif

    }
}

//*****************************************************************************
// CameraThread implementation
//*****************************************************************************

CameraThread::CameraThread(BetterVideoCapture* camera, QVideoFrame* videoFrame, unsigned char* cvImageBuf, int width, int height)
{
    task = new CameraTask(camera,videoFrame,cvImageBuf,width,height);
    task->moveToThread(&workerThread);
    connect(&workerThread, SIGNAL(started()), task, SLOT(doWork()));
    connect(task, SIGNAL(imageReady()), this, SIGNAL(imageReady()));
}

CameraThread::~CameraThread()
{
    stop();
    delete task;
}

void CameraThread::start()
{
    workerThread.start();
}

void CameraThread::stop()
{
    if(task != NULL)
        task->stop();
    workerThread.quit();
    workerThread.wait();
}

