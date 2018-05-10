import QtQuick 2.9
import QtQuick.Window 2.2
import QtCharts 2.0
import QtQuick.Layouts 1.3

// https://doc.qt.io/qt-5.10/qtdatavisualization-qmlmultigraph-example.html

// TODO: create a ListModel
// with ListElements

Window {
    visible: true
    width: 640
    height: 480
    GridLayout {
        id: grid_layout
        columns: 2
        Layout.fillHeight: true
        Layout.fillWidth: true
        anchors.fill: parent

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            border.color: surface_graph.theme.gridLineColor
            border.width: 2

        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            border.color: surface_graph.theme.gridLineColor
            border.width: 2

        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            border.color: surface_graph.theme.gridLineColor
            border.width: 2

        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            border.color: surface_graph.theme.gridLineColor
            border.width: 2

        }
    }
}
