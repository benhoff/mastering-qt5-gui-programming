import QtQuick 2.0

import QtQuick.Dialogs 1.3
import QtQuick.Controls 1.2
import ColorPickers 1.0


// This dialog helps our users change the color
Dialog {
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

    // getter function for the property `_color`
    function get_color()
    {
        return _color;
    }

    width: 200
    height: 400

    // create the content item for the dialog manually to
    // prempt the automatic margin/button generation
    // as per the recomendation in the third paragraph
    // of: http://doc.qt.io/qt-5/qml-qtquick-dialogs-dialog.html#details
    contentItem: Item {
        width: color_dialog.width
        height: color_dialog.height

        // ColorPicker actually uses Qt's painting system by
        // being a subclass of `QQuickPaintedItem`
        // The type presents the colored photo with all the options
        ColorPicker {
            id: color_picker
            width: color_dialog.width
            height: 300
            anchors.left: parent.left
            anchors.top: parent.top

            // this mouse area allows us to drag the crosshairs around
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

            // The image that we drag around
            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                id: crosshairs
                property int radius: width / 2 // truncated to int
                source: "crosshairs.png"
                y: color_picker.color_to_y(color_dialog.get_color())
                z: 1
            }

        }
        // Our "OK" button
        Button {
            onClicked: {
                // --------------------------------------------------------------
                // TODO: use photo_model's `setData` method.
                // Method call: `setData(index, value, role)`
                var column_number = 0;
                var q_model_index = photo_model.index(color_dialog.index, column_number);
                photo_model.setData(q_model_index, color_dialog.get_color(), "decoration");





                color_dialog.close()
            }
            text: "Ok"
            anchors.top: color_picker.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
        }
    }
}
