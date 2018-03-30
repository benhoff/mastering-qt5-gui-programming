window.onload = function() {
    var socket = new WebSocket("ws://localhost:12345");

    socket.onopen = function() {
        new QWebChannel(socket, function(channel) {
            // Get access to the object we published
            var interactive = channel.objects.interactive;

            // invoke a C++ slot
            interactive.call_me_from_website();

            // Handle a return result as part of a C++ method/slot call
            interactive.call_me_returns_string(function(return_value) {
                console.log("My method returned this string: " + return_value);
            });

            // Connect to the `string_changed` signal
            interactive.string_changed.connect(function() {
                console.log("String change detected! Who changed this string: " + interactive.who_changed_this_string);
            });

            console.log("Who set this string? The " + interactive.who_changed_this_string);

            // change a C++ value
            interactive.who_changed_this_string = "The Website";

            // Add an event listener to the HTML button's `click` method which calls the C++ method `launch_new_window`
            document.getElementById("myButton").addEventListener("click", function() {
                interactive.launch_new_window();
            }, false);

        });
    };

        socket.onclose = function() {
            console.error("web channel closed");
        };
        socket.onerror = function(error) {
            console.error("web channel error: " + error);
        };
};
