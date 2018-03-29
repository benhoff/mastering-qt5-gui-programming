window.onload = function() {
    var socket = new WebSocket("ws://localhost:12345");

    socket.onopen = function() {
        new QWebChannel(socket, function(channel) {
            // Get access to the object we published

            // Connect to the `fired` signal

            // Connect the HTML button `onclick` to the C++ method `launch_new_window`

            // change a C++ value

            // invoke a C++ method

            // Handle a return result as part of a C++ method call

            // Get notified when a C++ data value changes.
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
