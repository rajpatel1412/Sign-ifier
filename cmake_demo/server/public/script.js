const socket = io();
socket.on("connect", (socket) => { //confirm connection with NodeJS server
    console.log("Connected");
    sendCommandViaUDP("send");
});

$( document ).ready(function() {
    socket.on('canvas', function(data) {
        const canvas = $("#videostream");
        const context = canvas[0].getContext('2d');
        const image = new Image();
        image.src = "data:image/jpeg;base64,"+data;
        image.onload = function(){
            context.height = image.height;
            context.width = image.width;
            context.drawImage(image,0,0,context.width, context.height);
        }
    });

    window.setInterval(function() {sendCommandViaUDP("inference")}, 1000);

    socket.on('commandReply', function(data) {
        console.log(data);

		// var results = data.split(' ');
        // var current = document.getElementById('outputid');
        // var output = current + results;
        var output = data.split(' ');
        // $("#outputid").html(output);
        $("#outputid").val(output);
    });
});

function sendCommandViaUDP(message) {
	socket.emit('daUdpCommand', message);
    // console.log('daUdpCommand'+ message);
};
