import QtQuick 2.0
import QtQuick.Window 2.2


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
                    color_dialog.index = index
                    color_dialog.set_color(decoration)
                    color_dialog.open()
                }
            }
        }
        anchors.fill: parent
        clip: true
    }

    ColorChangeDialog {
        id: color_dialog

    }

}
