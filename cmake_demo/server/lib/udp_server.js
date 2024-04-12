"use strict";
/*
 * Respond to commands over a websocket to relay UDP commands to a local program
 */

var socketio = require('socket.io');
var io; // to listen to the server
// var {spawn} = require("child_process";)

var dgram = require('dgram');

exports.listen = function(server) {
	io = socketio.listen(server);
	io.set('log level 1');

	io.sockets.on('connection', function(socket) {
		// this socket listens and talks to the browser (udp_ui.js)
		handleCommand(socket);
	});

	io.sockets.on("disconnect", function(socket) {
		socket.emit("Error", "Server not running on target");
	});
};

// var ffmpegProcess = spawn('ffmpeg', ['-vcodec', 'mjpeg', '-i', 'pipe:0', '-f',
// 									'mjpeg', 'udp://192.168.7.1:12345']);

// ffmpegProcess.stderr.on('data', function(data) {
// 	console.error('ffmpeg stderr: $(data)');
// });

// ffmpegProcess.on('close', function(code) {
// 	console.log('ffmpeg process exited with code $(code)');
// });

// ffmpegProcess.on('error', function(error) {
// 	console.error("Error starting ffmpeg process:", err);
// });

function handleCommand(socket) {
	// Pased string of comamnd to relay
	socket.on('daUdpCommand', function(data) {
		console.log('daUdpCommand command: ' + data);

		var PORT = 12345;
		var HOST = '127.0.0.1';
		var buffer = new Buffer(data);

		var client = dgram.createSocket('udp4');
		client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes) {
			if (err) 
				throw err;
			console.log('UDP message sent to ' + HOST +':'+ PORT);
		});

		client.on('listening', function () {
			var address = client.address();
			console.log('UDP Client: listening on ' + address.address + ":" + address.port);
		});
		// Handle an incoming message over the UDP from the local application.
		// Handle error timer for when local udp stops responding
		var udpErrorTimer = setTimeout(function() {
			socket.emit('udpTimeoutError', 
			"UDP client on target failed to respond");
		}, 1000);

		client.on('message', function (message, remote) {
			console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);

			// var reply = message.toString('utf8')
			// socket.emit('commandReply', reply);

			// ffmpegProcess.on('data', function(video) {

			// })

			socket.emit('video', message);

			client.close();
			clearTimeout(udpErrorTimer);

		});
		client.on("UDP Client: close", function() {
			console.log("closed");
		});
		client.on("UDP Client: error", function(err) {
			console.log("error: ",err);
		});
	});
};
