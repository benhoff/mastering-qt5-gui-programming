import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2


Window {
    visible: true
    width: 640
    height: 480

    Rectangle {
        radius: 25
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        /*
        onPositionChanged: {
            var x = mouseX;
            var y = mouseY;
            console.log(x, y)
            // y_animation.set_mouse(y);
            // x_animation.set_mouse(x);
            // my_object.x = x - 25
            // my_object.y = y - 25
            x_animation.to = x - 25
            y_animation.to = y - 25
            x_animation.duration = Math.max(200, .5 * Math.abs(x-my_object.x))
            y_animation.duration = Math.max(200, .5 * Math.abs(y - my_object.y))
            x_animation.running = true
            y_animation.running = true
        }
        */
    }


    // https://stackoverflow.com/questions/5833207/qml-animation-with-both-velocity-and-infinite-loops
    // https://www.youtube.com/watch?v=rwLj-P2Oofw

    Rectangle {
        id: my_object
        width: 50
        height: 50
        color: "green"
        opacity:  1

        NumberAnimation on x{
            id: x_animation
            from: my_object.x

            function set_mouse(mouse_x)
            {
                var x_obj = my_object.x;
                duration = 50 * Math.abs(x_obj - mouse_x);
                to = mouse_x;
            }
        }

        // https://stackoverflow.com/questions/40467872/dynamically-change-from-to-values-used-in-animations
        NumberAnimation on y{
            id: y_animation
            from: my_object.y

            function set_mouse(mouse_y)
            {
                var y_obj = my_object.y;
                duration = 50 * Math.abs(y_obj - mouse_y);
                from = y_obj
                to = mouse_y;
            }
        }

    }
}
