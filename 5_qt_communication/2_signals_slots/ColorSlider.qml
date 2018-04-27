import QtQuick 2.2
import QtQuick.Controls.Private 1.0
import Colors 1.0

MyPaintedItem {
    id: colorSlider

    property real value: 1
    property real maximum: 1
    property real minimum: 0
    property string text: ""
    property bool pressed: mouseArea.pressed
    property bool integer: false
    property Component trackDelegate
    // FIXME
    property string handleSource: "../images/slider_handle.png"

    width: parent.width
    height: handle.height + textText.implicitHeight

    function updatePos() {
        if (maximum > minimum) {
            var pos = (track.width - 10) * (value - minimum) / (maximum - minimum) + 5;
            return Math.min(Math.max(pos, 5), track.width - 5) - 10;
        } else {
            return 5;
        }
    }

    SystemPalette { id: palette }

    Column {
        id: column
        width: parent.width
        spacing: 12
        Text {
            id: textText
            anchors.horizontalCenter: parent.horizontalCenter
            text: colorSlider.text
            anchors.left: parent.left
            color: palette.windowText
            renderType: Settings.isMobile ? Text.QtRendering : Text.NativeRendering
        }

        Item {
            id: track
            height: 8
            anchors.left: parent.left
            anchors.right: parent.right

            Loader {
                sourceComponent: trackDelegate
                width: parent.height
                height: parent.width
                y: width
            }

            BorderImage {
                // FIXME
                source: "../images/sunken_frame.png"
                border.left: 8
                border.right: 8
                border.top:8
                border.bottom: 8
                anchors.fill: track
                anchors.margins: -1
                anchors.topMargin: -2
                anchors.leftMargin: -2
            }

            Image {
                id: handle
                anchors.verticalCenter: parent.verticalCenter
                smooth: true
                // FIXME
                source: "../images/slider_handle.png"
                x: updatePos() - 8
                z: 1
            }

            MouseArea {
                id: mouseArea
                anchors {left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter}
                height: handle.height
                width: handle.width
                preventStealing: true

                onPressed: {
                    var handleX = Math.max(0, Math.min(mouseX, mouseArea.width))
                    var realValue = (maximum - minimum) * handleX / mouseArea.width + minimum;
                    value = colorSlider.integer ? Math.round(realValue) : realValue;
                }

                onPositionChanged: {
                    if (pressed) {
                        var handleX = Math.max(0, Math.min(mouseX, mouseArea.width))
                        var realValue = (maximum - minimum) * handleX / mouseArea.width + minimum;
                        value = colorSlider.integer ? Math.round(realValue) : realValue;
                    }
                }
            }
        }
    }
}
