import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.0


Window {
    visible: true
    width: 600
    height: 400
    title: "Visible Model"

    ListView {
        model: photo_model
        delegate: picture_delegate
        anchors.fill: parent
        clip: true
        flickableDirection: Flickable.AutoFlickDirection
    }

    Component {
        id: picture_delegate
        Rectangle {
            width: parent.width
            height: 125
            color: decoration

            Dialog {
                y: 0; width: 100; height: 50
                id: color_dialog
                title: "Please select a color"
                onAccepted: {}
                onRejected: {}

                LinearGradient {
                    y: 0; width: 100; height: 50
                    start: Qt.point(0, 50)
                    end: Qt.point(100, 50)
                    gradient: Gradient {
                        GradientStop{ position: 0.0; color: "white" }
                        GradientStop{ position: 1.0; color: "blue"}
                    }
                }

            }

            MouseArea {
                anchors.fill: parent
                onClicked: color_dialog.open()
            }
        }
    }

}
