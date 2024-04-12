var socket = io();

socket.on("connect", function(socket) {
    console.log("Connected");
});

$(document).ready(function() {
    socket.on('canvas', function(data) {
        var canvas = $("#videostream");
        var context = canvas[0].getContext('2d');
        var image = new Image();
        image.src = "data:image/jpeg;base64,"+data;
        image.onload = function() {
            context.height = image.height;
            context.width = image.width;
            context.drawImage(image, 0, 0, context.width, context.height);
        };
    });
});