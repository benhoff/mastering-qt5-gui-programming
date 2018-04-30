import QtQuick 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Controls 1.2
import ColorPickers 1.0

Dialog {
    property color color: "white"
    property int index: -1

    property bool _delegate_clicked: false
    property bool _user_accepted: false

    onColorChanged: {
        if (_delegate_clicked) {
            crosshairs.y = color_picker.color_to_y(color)
        }
    }

    on_User_acceptedChanged: {
        if (_user_accepted) {
            var qt_index = photo_model.index(index, 0)
            photo_model.setData(qt_index, color, "decoration")
            _user_accepted = false
        }
    }

    contentItem: Item {
        width: color_dialog.width
        height: color_dialog.height
        ColorPicker {
            id: color_picker
            width: color_dialog.width
            height: 300
            anchors.left: parent.left
            anchors.top: parent.top

            MouseArea {
                anchors.fill: parent
                onPositionChanged: {
                    if (pressed) {
                        crosshairs.y = mouse.y - crosshairs.radius
                        color_dialog.color = parent.y_to_color(mouse.y)
                    }
                }
                onPressed: positionChanged(mouse)
            }
            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                id: crosshairs
                property int radius: width / 2 // truncated to int
                source: "crosshairs.png"
                y: color_picker.color_to_y(color_dialog.color)
                z: 1
            }

        }
        Button {
            anchors.top: color_picker.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            text: "Ok"
            onClicked: {
                color_dialog._user_accepted = true
                color_dialog.close()
            }
        }
    }

    // UI information
    title: "Please select a color"
    width: 200
    height: 400

}
