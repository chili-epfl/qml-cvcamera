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
 * @file BetterVideoCapture.cpp
 * @brief A wrapper for either cv::VideoCapture for desktop or CVCaptureAndroid for Android
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2014-10-02
 */

#include"BetterVideoCapture.h"

BetterVideoCapture::BetterVideoCapture()
{
#ifdef ANDROID
    capture = new CVCaptureAndroid();
#else
    capture = new cv::VideoCapture();
#endif
}

BetterVideoCapture::~BetterVideoCapture()
{
    delete capture;
}

bool BetterVideoCapture::open(int device)
{
    return capture->open(device);
}

double BetterVideoCapture::getProperty(int propIdx)
{
    return capture->get(propIdx);
}

bool BetterVideoCapture::setProperty(int propIdx,double propVal)
{
    return capture->set(propIdx,propVal);
}

bool BetterVideoCapture::grabFrame()
{
    return capture->grab();
}

unsigned char* BetterVideoCapture::retrieveFrame()
{
#ifdef ANDROID
    return capture->retrieveFrame();
#else
    capture->retrieve(rawImage);
    return rawImage.ptr();
#endif
}

bool BetterVideoCapture::isOpened() const
{
    return capture->isOpened();
}

