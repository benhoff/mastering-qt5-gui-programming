window.onload = function() {
    var socket = new WebSocket("ws://localhost:12345");

    socket.onopen = function() {
        new QWebChannel(socket, function(channel) {
            // Get access to the object we published
            window.interactive = channel.objects.interactive;

            // Connect to the `fired` signal
            interactive.fired.connect(function() {
                console.warn("Signal `fired` detected");
            });

            // Connect the HTML button `onclick` to the C++ method `launch_new_window`
            document.getElementById("myButton").onclick = interactive.launch_new_window;

            // change a C++ value
            interactive.who_changed_this_string = "The Website";

            // invoke a C++ method
            interactive.call_me_from_website();

            // Handle a return result as part of a C++ method call
            interactive.call_me_returns_string(function(return_value) {
                console.warn("I got this string: " + return_value);
            });

            // Get notified when a C++ data value changes.
            interactive.this_string_changes_in_time.connect(function(new_value) {
                console.warn("The string that has a single shot timer to change it, changed!");
                console.warn("String now reads: " + new_value);
            });
        });
    };

        socket.onclose = function() {
            console.error("web channel closed");
        };
        socket.onerror = function(error) {
            console.error("web channel error: " + error);
        };
};
