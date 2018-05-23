import QtQuick 2.0


Item {
    property alias shared_data: land_speed_records
    property int max_x: 1227
    property int min_x: 655

    function get_max_date()
    {
        return Date.fromLocaleString(Qt.locale(), "October 15, 1997", "MMMM d, yyyy")
    }

    function get_min_date()
    {
        return Date.fromLocaleString(Qt.locale(), "August 5, 1963", "MMMM d, yyyy")
    }


    ListModel {
        id: land_speed_records
        ListElement{speed: 655.722; driver: "Craig Breedlove"; date: "August 5, 1963"}
        ListElement{speed: 665; driver: "Tom Green"; date: "October 2, 1964"}
        ListElement{speed: 698.5; driver: "Art Arfons"; date: "October 5, 1964"}
        ListElement{speed: 754.33; driver: "Craig Breedlove"; date: "October 13, 1964"}
        ListElement{speed: 846.961; driver: "Craig Breedlove"; date: "October 15, 1964"}
        ListElement{speed: 863.751; driver: "Art Arfons"; date: "October 27, 1964"}
        ListElement{speed: 893.966; driver: "Craig Breedlove"; date: "November 2, 1965"}
        ListElement{speed: 927.872; driver: "Art Arfons"; date: "November 7, 1965"}
        ListElement{speed: 966.574; driver: "Craig Breedlove"; date: "November 15, 1965"}
        ListElement{speed: 1001.667; driver: "Gary Gabelich"; date: "October 23, 1970"}
        ListElement{speed: 1019.47; driver: "Richard Noble"; date: "October 4, 1983"}
        ListElement{speed: 1149.303; driver: "Andy Green"; date: "September 25, 1997"}
        ListElement{speed: 1227; driver: "Andy Green"; date: "October 15, 1997"}

    }
}
