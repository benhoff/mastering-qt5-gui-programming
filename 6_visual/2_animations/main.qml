import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2


Window {
    id: window
    visible: true
    width: 640
    height: 480

    property int wiggle_value: 0

    Timer {
        interval: 1500
        onTriggered: {
            wiggle_value = 5
            console.log('hasdfasd')
        }
        running: true
    }

    Rectangle {
        id: my_rect
        property int max_height: window.height / 5
        property int min_height: 2 * window.height / 5
        radius: 50
        anchors.horizontalCenter: parent.horizontalCenter
        color: "blue"
        width: 50
        height: 50
        y: min_height

        Component.onCompleted: {
            my_animation.stopped.connect(function(){ready_button.visible = true;});
        }

        SequentialAnimation on y {
            id: my_animation
            // loops: Animation.Infinite
            loops: 3

            NumberAnimation {
                from: my_rect.min_height
                to: my_rect.max_height
                easing.type: Easing.OutExpo
                duration: 300
            }
            NumberAnimation {
                from: my_rect.max_height
                to: my_rect.min_height
                easing.type: Easing.OutBounce;
                duration: 1000
            }

            PauseAnimation {
                duration: 500
            }
        }
    }

    Button {
        // anchors.horizontalCenter: parent.horizontalCenter
        id: ready_button
        y: window.height / 3
        anchors.horizontalCenter: parent.horizontalCenter
        // visible: false
        text: "Launch App"
        transform: Rotation {
            id: my_rotation
            origin: Item.Center
            angle: window.wiggle_value
            Behavior on angle {
                SequentialAnimation {
                    NumberAnimation {
                        duration: 60
                    }
                    NumberAnimation {
                        from: 5
                        to: 0
                        duration: 60
                    }

                    PauseAnimation {
                        duration: 200
                    }
                    NumberAnimation {
                        from: 0
                        to: -5
                        duration: 60
                    }
                    NumberAnimation {
                        from: -5
                        to: 0
                        duration: 60
                    }
                }
            }
        }
    }

}
