import QtQuick 2.0
import QtQuick.Window 2.2


Window {
    visible: true
    width: 500
    height: 400
    title: "Visible Model"

    ListView {
        id: color_view
        model: photo_model
        delegate: Rectangle {
            id: colored_rectangle
            color: decoration

            width: parent.width
            height: 80

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    color_dialog._delegate_clicked = true
                    color_dialog.color = decoration
                    color_dialog.index = index
                    color_dialog.open()
                    color_dialog._delegate_clicked = false
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
