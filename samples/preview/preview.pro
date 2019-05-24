TEMPLATE = app

QT += qml quick multimedia

SOURCES += src/main.cpp

RESOURCES += qml.qrc

INCLUDEPATH += $$(OPENCV_DIR)/include/opencv4
INCLUDEPATH += ../../src
LIBS += -L$$(OPENCV_DIR)/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio
LIBS += -L/Users/jg/Code/opensource/qt/5.12.3/clang_64/qml/CVCamera/ -lcvcameraplugin


android {
    target.path = /libs/armeabi-v7a
    export(target.path)
    INSTALLS += target
    export(INSTALLS)

    ANDROID_EXTRA_LIBS = \
        $$(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/share/opencv/sdk/native/libs/armeabi-v7a/libopencv_core.so \
        $$(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/share/opencv/sdk/native/libs/armeabi-v7a/libopencv_imgproc.so \
        $$(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/share/opencv/sdk/native/libs/armeabi-v7a/libopencv_imgcodecs.so \
        $$(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/share/opencv/sdk/native/libs/armeabi-v7a/libopencv_videoio.so \
        $$(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/share/opencv/sdk/native/libs/armeabi-v7a/libopencv_highgui.so \
        $$(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/share/opencv/sdk/native/libs/armeabi-v7a/libnative_camera_r4.4.0.so \ #Adapt this line to your device's Android version
        $$[QT_INSTALL_QML]/CVCamera/libcvcameraplugin.so

    ANDROID_PERMISSIONS += \
        android.permission.CAMERA

    ANDROID_FEATURES += \
        android.hardware.camera
}
