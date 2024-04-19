
// Source: adapted from Team Solar streaming Guide
// https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/links/files/2022-student-howtos/StreamingWebcamFromBeagleBoneToNodeJSServer.pdf

const socket = io();
socket.on("connect", (socket) => { //confirm connection with NodeJS server
    console.log("Connected");
    sendCommandViaUDP("send");
});

$( document ).ready(function() {

    // display video
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

    // constantly ask for inferences (every 0.5 seconds)
    window.setInterval(function() {sendCommandViaUDP("infer")}, 500);

    // updating the display inference string
    socket.on('commandReply', function(data) {
        console.log(data);

        var output = data.split(' ');
        $("#outputid").val(output);
    });
});

function sendCommandViaUDP(message) {
	socket.emit('daUdpCommand', message);
};
