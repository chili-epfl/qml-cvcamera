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
 * @file cvcamera.h
 * @brief Nice QML wrapper for camera access with OpenCV
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2014-09-22
 */

#ifndef CVCAMERA_H
#define CVCAMERA_H

#include<QQuickItem>
#include<QCameraInfo>
#include<QList>
#include<QAbstractVideoSurface>
#include<QVideoSurfaceFormat>
#include<QSize>
#include<QMutex>
#include<QWaitCondition>
#include<QVariant>

#include<opencv2/highgui/highgui.hpp>

#include"CameraThread.h"

Q_DECLARE_METATYPE(cv::Mat)

/**
 * @brief QML wrapper for OpenCV camera access
 */
class CVCamera : public QQuickItem {
Q_OBJECT
    Q_DISABLE_COPY(CVCamera)
    Q_PROPERTY(int device READ getDevice WRITE setDevice)
    Q_PROPERTY(QAbstractVideoSurface* videoSurface READ getVideoSurface WRITE setVideoSurface)
    Q_PROPERTY(QSize size READ getSize WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QStringList deviceList READ getDeviceList NOTIFY deviceListChanged)
    Q_PROPERTY(QVariant cvImage READ getCvImage NOTIFY cvImageChanged)

public:

    /**
     * @brief Creates a new cv camera with the given QML parent
     *
     * @param parent The QML parent
     */
    CVCamera(QQuickItem* parent = 0);

    /**
     * @brief Destroys this cv camera
     */
    ~CVCamera();

    /**
     * @brief Returns the device number
     *
     * @return The current device number
     */
    int getDevice() const;

    /**
     * @brief Changes the device number, releasing the old device and opening a new device
     *
     * @param device The new device number
     */
    void setDevice(int device);

    /**
     * @brief Gets the current camera resolution
     *
     * @return The current camera resolution
     */
    QSize getSize() const;

    /**
     * @brief Sets the desired camera resolution, closing and opening the device if necessary
     *
     * @param camSize The desired camera resolution
     */
    void setSize(QSize size);

    /**
     * @brief Gets the list of available devices, indices corresponding to device indice"s
     *
     * @return List of available devices
     */
    QStringList getDeviceList() const;

    /**
     * @brief Gets the video surface associated with this camera
     *
     * @return The video surface associated with this camera
     */
    QAbstractVideoSurface* getVideoSurface() const;

    /**
     * @brief Gives a video surface for this camera to draw on
     *
     * @param surface The new surface to draw on
     */
    void setVideoSurface(QAbstractVideoSurface* videoSurface);

    /**
     * @brief Gets the exported cv image, same size as the screen image but can be in different format
     *
     * @return Exported cv image
     */
    QVariant getCvImage();

signals:

    /**
     * @brief Emitted after the camera image size has changed
     */
    void sizeChanged();

    /**
     * @brief Is emitted once after the device list constructed for the first and only time
     */
    void deviceListChanged();

    /**
     * @brief Is emitted each time a new frame arrives
     */
    void cvImageChanged();

private:

#ifdef ANDROID
    const QVideoFrame::PixelFormat VIDEO_OUTPUT_FORMAT = QVideoFrame::PixelFormat::Format_YV12;
#else
    const QVideoFrame::PixelFormat VIDEO_OUTPUT_FORMAT = QVideoFrame::PixelFormat::Format_ARGB32;
#endif

    int device = 0;                                         ///< The camera device number
    QStringList deviceList;                                 ///< The list of available devices, indices corresponding to device indices
    QSize size;                                             ///< The desired camera resolution
    BetterVideoCapture* camera = NULL;                      ///< The camera object
    CameraThread* thread = NULL;                            ///< Thread to run camera image acquisition

    QVideoFrame* videoFrame = NULL;                         ///< Object that contains the image buffer to be presented to the video surface
    QAbstractVideoSurface* videoSurface = NULL;             ///< Object that presents the videoFrame to the outside world

    bool exportCvImage = false;                             ///< Whether to export the CV image
    cv::Mat cvImage;                                        ///< Buffer to export the camera image to
    unsigned char* cvImageBuf = NULL;                       ///< Actual physical buffer for cv::Mat, it is helpless without it

    /**
     * @brief Closes and opens the desired camera with possibly new parameters
     */
    void update();

    /**
     * @brief Allocates an appropriately sized cv::Mat depending on the output size
     */
    void allocateCvImage();

    /**
     * @brief Allocates an appropriately sized and formatted QVideoFrame
     */
    void allocateVideoFrame();

public slots:

    /**
     * @brief Callback for a parent change event
     *
     * @param parent New parent
     */
    void changeParent(QQuickItem* parent);

private slots:

    /**
     * @brief Callback for when image from the camera is received
     */
    void imageReceived();
};

#endif /* CVCAMERA_H */

