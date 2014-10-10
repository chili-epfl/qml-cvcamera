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
 * @file BetterVideoCapture.h
 * @brief A wrapper for either cv::VideoCapture for desktop or CVCaptureAndroid for Android
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2014-10-02
 */

#ifndef BETTERVIDEOCAPTURE_H
#define BETTERVIDEOCAPTURE_H

#include<opencv2/videoio.hpp>

#ifdef ANDROID
#include"CVCaptureAndroid.h"
#endif

/**
 * @brief A better cv::VideoCapture for desktop and Android
 */
class BetterVideoCapture {

public:

    BetterVideoCapture();
    virtual ~BetterVideoCapture();

    bool open(int device);
    double getProperty(int propIdx);
    bool setProperty(int propIdx, double propVal);
    bool grabFrame();
    unsigned char* retrieveFrame();
    bool isOpened() const;

private:

#ifdef ANDROID
    CVCaptureAndroid* capture;
#else
    cv::VideoCapture* capture;
    cv::Mat rawImage;
#endif
};

#endif /* BETTERVIDEOCAPTURE_H */
