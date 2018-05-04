import QtQuick 2.0

Item {
    function hello_world() {
        console.log("hello world");
    }

    Component.onCompleted: hello_world();

}
