import QtQuick 2.9
import QtQuick.Controls 2.2
import QtCharts 2.0
import "."


ApplicationWindow{
    visible: true
    width: 640
    height: 480

    header: ToolBar {
        ToolButton {
            text: "Add Scatter"
            property bool _already_created: false
            onClicked: {
                if (_already_created)
                    return;

                var scatter_x = chart_view.axisX(line_series);
                var scatter_y = chart_view.axisY(line_series);
                var scatter = chart_view.createSeries(ChartView.SeriesTypeScatter, "scatter", scatter_x, scatter_y)
                _add_data_to_series(scatter)
                _already_created = true
            }
        }
    }

    Data {
        id: data
    }

    function _add_data_to_series(series){
        for (var i =0; i < data.shared_data.count; i++) {
            var list_element = data.shared_data.get(i);

            var speed = list_element.speed;
            var year = list_element.date;

            series.append(speed, year)
        }
    }

    ChartView {
        id: chart_view
        title: "Land Speed Records"
        anchors.fill: parent

        ValueAxis {
            id: x_axis
            max: data.max_x
            min: data.min_x
        }

        DateTimeAxis {
            id: y_axis
            min: data.get_min_date()
            max: data.get_max_date()
            format: "MMM yyyy"
        }

        LineSeries {
            id: line_series
            axisX: x_axis
            axisY: y_axis
        }

        Component.onCompleted: {
            data.initialize_data()
            _add_data_to_series(line_series)
        }
    }

}
