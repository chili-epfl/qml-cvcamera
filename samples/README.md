qml-cvcamera Samples
--------------------

These samples demonstrate the usage of CVCamera. To build a sample:

  - Build and install CVCamera itself, see [../README.md](../README.md)
  - Go inside a sample's directory and follow the instructions below

Desktop Build
-------------

```
mkdir build-desktop
cd build-desktop
/qt/install/root/5.3/gcc_64/bin/qmake ..
make -j 5
```

To launch the sample, run the created executable in the build directory.

Android Build
-------------

Connect your device and run the following:

```
mkdir build-android
cd build-android
/qt/install/root/5.3/android_armv7/bin/qmake ..
make -j 5 install INSTALL_ROOT=.
/qt/install/root/5.3/android_armv7/bin/androiddeployqt \
    --input *.json \
    --output .
ant debug install
```

To launch the sample, run the installed app on your device; it will have the same name as the sample.

**Important note:** Currently, you have to change the native camera library according to the Android version of your device. For this, go to a sample's `.pro` file and adapt the

```
$$(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/share/opencv/sdk/native/libs/armeabi-v7a/libnative_camera_r4.4.0.so
```

line to your device's version by choosing the library with the closest but lower version. To see the available library versions, run:

```
ls $ANDROID_STANDALONE_TOOLCHAIN/sysroot/usr/share/opencv/sdk/native/libs/armeabi-v7a/ | grep libnative_camera
```

For example, my device uses Android `4.1.2`, so I should choose `libnative_camera_r4.1.1.so` here.

This is an issue with the native camera library chooser of OpenCV that normally depends on OpenCV Manager. We're not going to be using that anytime soon and we are currently working on a solution.

QtCreator Build for Desktop and Android
---------------------------------------

If you're using `.bashrc` to export your environment variables, make sure you launch QtCreator with `bash -i -c /path/to/qtcreator` so that it can find your environment variables.

Load the sample in QtCreator and add:

  - *Qt 5.3 for Android armv7* kit for Android
  - *Desktop Qt 5.3 GCC 64bit* kit for Desktop

Building and running the sample should now be as easy as clicking the *Run* button.

See the above note about native camera library versions for the Android build.

