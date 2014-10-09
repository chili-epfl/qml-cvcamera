import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtMultimedia 5.0
import CVCamera 1.0

Window {
    visible: true

    //Width and height stuff is for desktop only, they seem to be ignored on Android (this is expected good behavior)
    width: camera.size.width
    height: camera.size.height
    maximumHeight: camera.size.height
    maximumWidth: camera.size.width

    CVCamera {
        id: camera
        device: deviceBox.currentIndex
        size: "640x480"
    }

    VideoOutput {
        anchors.top: imageSizeField.top
        id: output
        source: camera
    }

    TextField {
        id: imageSizeField
        text: "640x480"
        placeholderText: "Enter image size"
    }

    Button {
        id: imageSizeSetButton
        text: "Set"
        anchors.left: imageSizeField.right
        onClicked: camera.size = imageSizeField.text
    }

    ComboBox {
        id: deviceBox
        width: 200
        anchors.left: imageSizeSetButton.right
        model: camera.deviceList
    }
}
