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
 * @file cvcameraplugin.cpp
 * @brief Implementation of the object that exposes the CVCamera plugin components as QML objects
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2014-09-22
 */

#include"CVCameraPlugin.h"

void CVCameraPlugin::registerTypes(const char* uri)
{
    qmlRegisterType<CVCamera>(uri, 1, 0, "CVCamera");
}
