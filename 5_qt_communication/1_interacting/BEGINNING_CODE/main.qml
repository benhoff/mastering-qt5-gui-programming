import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.3
import QtQuick.Controls 1.2
import ColorPickers 1.0


Window {
    visible: true
    width: 500
    height: 400

    ListView {
        model: photo_model
        delegate: Rectangle {
            color: decoration

            width: parent.width
            height: 80

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    color_dialog.set_color(decoration)
                    color_dialog.index = index
                    color_dialog.open()
                }
            }
        }
        anchors.fill: parent
    }

    Dialog {
        id: color_dialog
        property int index
        property color _color

        // NOTE: This guard prevents us from moving the crosshairs
        // twice whenever we're dragging the crosshairs around
        property bool _mouse_area_clicked: false

        // This code moves the interactive image
        // from the above `onClicked` code
        on_ColorChanged: {
            if (_mouse_area_clicked) {
                crosshairs.y = color_picker.color_to_y(_color)
            }
        }

        // This is a setter for our property `_color`
        function set_color(new_color)
        {
            _mouse_area_clicked = true;
            _color = new_color;
            _mouse_area_clicked = false;
        }

        function get_color()
        {
            return _color;
        }

        width: 200
        height: 400

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
                            color_dialog._color = parent.y_to_color(mouse.y)
                        }
                    }
                    onPressed: positionChanged(mouse)
                }
                Image {
                    anchors.horizontalCenter: parent.horizontalCenter
                    id: crosshairs
                    property int radius: width / 2 // truncated to int
                    source: "crosshairs.png"
                    y: color_picker.color_to_y(color_dialog.get_color())
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
                    var qt_index = photo_model.index(color_dialog.index, 0)
                    photo_model.setData(qt_index, color_dialog.get_color(), "decoration")
                    color_dialog.close()
                }
            }
        }
    }
}
