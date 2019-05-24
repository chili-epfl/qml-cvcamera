import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 2.5
import QtMultimedia 5.0

ApplicationWindow {
    visible: true

    //Width and height stuff is for desktop only, they seem to be ignored on Android (this is expected good behavior)
    width: camerax.size.width
    height: camerax.size.height
    maximumHeight: camerax.size.height
    maximumWidth: camerax.size.width

    

    VideoOutput {
        anchors.top: imageSizeField.top
        id: output
        source: camerax
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
        model: camerax.deviceList
    }
}
