// http://doc.qt.io/qt-5/qtwebchannel-javascript.html

window.onload = function() {
    var socket = new WebSocket("ws://localhost:12345");

    socket.onopen = function() {
        new QWebChannel(socket, function(channel) {
            // Connect to a signal:
            channel.objects.signaler.fired.connect(function() {
                // This callback will be invoked whenever the signal is emitted on the C++/QML side.
                console.warning("Signal detected");
            });

            // To make the object known globally, assign it to the window object, i.e.:
            window.signaler = channel.objects.signaler;

            // change a value
            signaler.who_changed_this_string = "Website";

            // invoke a method
            signaler.call_me();

            // Handle a return result as part of a method
            signaler.call_me_and_handle_string(function(return_value) {
                console.log(return_value);
            });

            // Read a property value, which is cached on the client side:
            console.log(foo.myProperty);

            // To get notified about remote property changes,
            // simply connect to the corresponding notify signal:
            foo.onMyPropertyChanged.connect(function(newValue) {
                console.log(newValue);
            });
        });
    };

        socket.onclose = function() {
            console.error("web channel closed");
        };
        socket.onerror = function(error) {
            console.error("web channel error: " + error);
        };
    }
};
