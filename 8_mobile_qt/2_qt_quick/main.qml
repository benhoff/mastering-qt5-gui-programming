import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.3
import QtQuick.Controls 1.2
import ColorPickers 1.0


Window{
    // NOTE: might not need this anymore
    id: window
    visible: true
    width: 500
    height: 400

    signal request_new_colors(string urgency)

    function _log_the_urgency(urgency)
    {
        console.log(urgency);
    }

    Component.onCompleted: {
        window.request_new_colors.connect(window._log_the_urgency);
    }


    ListView {
        model: photo_model
        anchors.fill: parent
        Component.onCompleted: {
            function my_data_changed_slot(upper_index, lower_index, role)
            {
                if (upper_index == lower_index)
                    console.log("Only one index changed!");
                else
                    console.log("Many indexes changed!");
            }
            photo_model.dataChanged.connect(my_data_changed_slot);
        }

        header: Rectangle {
            height: 50
            width: parent.width

            Timer {
                id: timer
                onTriggered: {
                    // TODO: Emit our signal `request_new_colors`
                    window.request_new_colors("now!");


                    rectangle_text.text = "Click any item to change it's color!";
                    running = false;
                }

                running: false
                repeat: false
                interval: 2000
            }

            MouseArea {
                onClicked: parent.change_header_text();
                anchors.fill: parent
            }
            Text {
                id: rectangle_text
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: "Click any item to change it's color!"
            }

            function change_header_text()
            {
                rectangle_text.text = "Changing colors now"
                timer.running = true
            }

        }

        delegate: Rectangle {
            color: decoration
            width: parent.width
            height: 80

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    color_dialog.index = index;
                    color_dialog.set_color(decoration);
                    color_dialog.open();
                }
            }
        }
    }

    Item {
        id: dialog_item
        anchors.fill: parent
        // This dialog helps our users change the color
        Dialog {
            id: color_dialog
            property int index
            property color _color
            modality: Qt.WindowModal

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
    }
}
