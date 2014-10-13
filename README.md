qml-cvcamera
============

CVCamera is a QML wrapper for fast camera access using OpenCV. It works on desktop and Android.

The following is required for CVCamera to work:

  - Ubuntu `14.04`
  - Qt `5.3.2` (also tested on `5.3.1`)
  - OpenCV `3.0.0-alpha`(master build should also just work)

QML API
-------

The QML API consists of the following:

>  - **device** : `int` - ID of the camera that will be opened, usually goes like 0, 1, ...
>  - **size** : `QSize` - Size of the camera image
>  - **deviceList** : `QStringList` - Names of the detected cameras, indices corresponding to IDs
>  - **cvImage** : `QVariant` - Contains the camera image as a `cv::Mat`

The full API (API above + non-user parts) can be seen in [cvcamera.types](cvcamera.types).

CVCamera can be used in one or both of two ways:

  - Set a `CVCamera` object as the `source` property of a `VideoOutput` in order to display in on the screen
  - Set a `CVCamera` object's `cvImage` property as some property of a QML object to export the camera image as a cv::Mat. The format of this image is usually RGB in desktop and YUV420sp (a.k.a YUV NV21) on Android.

Desktop Build
-------------

1. Install `libv4l-dev`.

2. Build and install OpenCV:

  ```
  cd platforms
  mkdir build-desktop
  cd build-desktop
  cmake ../.. -DCMAKE_INSTALL_PREFIX=/usr
  ```

  At this point, OpenCV components other than `calib3d`, `core`, `features2d`, `flann`, `highgui`, `imgcodecs`, `imgproc`, `video` and `videoio` are not needed and can be disabled. **Note:** I had to disable `WITH_GSTREAMER` manually for it to compile without errors. It is not needed and can be disabled. These options can be accessed with `ccmake ../..`.

  ```
  make -j 5
  make install
  ```

3. Build and install CVCamera:

  ```
  mkdir build-desktop
  cd build-desktop
  /qt/install/root/5.3/gcc_64/bin/qmake ..
  make -j 5
  make install
  ```

  Now the CVCamera QML plugin is installed alongside Qt's QML plugins and can be used similar to any other plugin.

Android Build
-------------

In addition to the OS, Qt and OpenCV requirements, you need:

  - Android SDK and Android API 14 (Android version 4.0)

  - Android NDK r9d

These instructions assume `armv7-a` target architecture. For other architectures, adapt the instructions to your liking.

1. Export a standalone NDK toolchain:

  ```
  cd /path-to-android-ndk-r9d
  ./build/tools/make-standalone-toolchain.sh \
      --platform=android-14 \
      --install-dir=/desired/path/to/android/standalone/toolchain \
      --toolchain=arm-linux-androideabi-4.8
  ```

  Be aware that if you don't have write access to `/desired/path/to/android/standalone/toolchain`, the script fails silently.

2. Set up the following environment variables:

  ```
  export ANDROID_HOME=/path/to/android/sdk/linux
  export ANDROID_SDK_ROOT=$ANDROID_HOME
  export ANDROID_SDK=$ANDROID_SDK_ROOT
  export ANDROID_NDK_ROOT=/path-to-android-ndk-r9d
  export ANDROID_NDK=$ANDROID_NDK_ROOT
  export ANDROID_NDK_STANDALONE_TOOLCHAIN=/path/to/android/standalone/toolchain
  export ANDROID_STANDALONE_TOOLCHAIN=$ANDROID_NDK_STANDALONE_TOOLCHAIN
  export ANDROID_ABI=armeabi-v7a
  export ANDROID_NATIVE_API_LEVEL=14
  export ANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.8
  ```

3. Build and install OpenCV:

  ```
  cd platforms
  mkdir build-android
  cd build-android
  cmake ../.. \
      -DCMAKE_TOOLCHAIN_FILE=../android/android.toolchain.cmake \
      -DCMAKE_INSTALL_PREFIX=$ANDROID_STANDALONE_TOOLCHAIN/sysroot/usr/share/opencv/
  ```

  At this point, enable `BUILD_SHARED_LIBS`.

  OpenCV components other than `calib3d`, `core`, `features2d`, `flann`, `highgui`, `imgcodecs`, `imgproc`, `video` and `videoio` are not needed and can be disabled. Leave these options untouched if you are unsure what to do.

  The options can be accessed with `ccmake ../..` after running `cmake`.


  ```
  make -j 5
  make install
  ```

4. Build and install CVCamera:

  ```
  mkdir build-android
  cd build-android
  /qt/install/root/5.3/android_armv7/bin/qmake ..
  make -j 5
  make install
  ```

  Now the CVCamera QML plugin is installed alongside Qt's QML plugins and can be used similar to any other plugin.
  
  Note that you still need to bundle the required OpenCV libraries with `ANDROID_EXTRA_LIBS` and give camera access to your app with `ANDROID_PERMISSIONS` and `ANDROID_FEATURES` in the project that uses CVCamera. See [samples/preview/preview.pro](samples/preview/preview.pro) for an example.

Running Samples
---------------

See [samples/README.md](samples/README.md).

