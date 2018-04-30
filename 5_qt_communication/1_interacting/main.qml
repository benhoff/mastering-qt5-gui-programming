import QtQuick 2.0
import QtQuick.Window 2.2


Window {
    visible: true
    width: 600
    height: 400
    title: "Visible Model"
    color: "lightgray"

    ListView {
        model: photo_model
        delegate: picture_delegate
        anchors.fill: parent
        clip: true
    }
    // delgate == Rectangle?
    Component {
        id: picture_delegate
        Rectangle {
            width: 100
            height: 100
            color: decoration
        }

    }
}
