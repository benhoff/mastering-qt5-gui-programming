import QtQuick 2.0


Item {
    property alias shared_data: land_speed_records
    property bool initialized: false
    property int max_x
    property int min_x
    property string _min_date
    property string _max_date

    function initialize_data()
    {
        if (initialized)
            return;

        _min_date = "August 5, 1963";
        _max_date = "October 15, 1997";

        land_speed_records.append({"speed": 655.722, "driver": "Craig Breedlove", "date": _get_data(_min_date)});
        land_speed_records.append({"speed": 665, "driver": "Tom Green", "date": _get_data("October 2, 1964")});
        land_speed_records.append({"speed": 698.5, "driver": "Art Arfons", "date": _get_data("October 5, 1964")});
        land_speed_records.append({"speed": 754.33, "driver": "Craig Breedlove", "date": _get_data("October 13, 1964")});
        land_speed_records.append({"speed": 846.961, "driver": "Craig Breedlove", "date": _get_data("October 15, 1964")});
        land_speed_records.append({"speed": 863.751, "driver": "Art Arfons", "date": _get_data("October 27, 1964")});
        land_speed_records.append({"speed": 893.966, "driver": "Craig Breedlove", "date": _get_data("November 2, 1965")});
        land_speed_records.append({"speed": 927.872, "driver": "Art Arfons", "date": _get_data("November 7, 1965")});
        land_speed_records.append({"speed": 966.574, "driver": "Craig Breedlove", "date": _get_data("November 15, 1965")});
        land_speed_records.append({"speed": 1001.667, "driver": "Gary Gabelich", "date": _get_data("October 23, 1970")});
        land_speed_records.append({"speed": 1019.47, "driver": "Richard Noble", "date": _get_data("October 4, 1983")});
        land_speed_records.append({"speed": 1149.303, "driver": "Andy Green", date: _get_data("September 25, 1997")});
        land_speed_records.append({"speed": 1227, "driver": "Andy Green", "date": _get_data(_max_date)});

        max_x = 1227
        min_x = 655

    }

    function get_max_date()
    {
        return _get_data(_max_date)

    }

    function get_min_date()
    {
        return _get_data(_min_date)
    }

    function _get_data(date_string)
    {
        return Date.fromLocaleString(Qt.locale(), date_string, "MMMM d, yyyy")
    }

    ListModel {
        id: land_speed_records

    }
}
