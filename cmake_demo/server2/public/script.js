const socket = io();
socket.on("connect", (socket) => { //confirm connection with NodeJS server
console.log("Connected");
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
};
});
});