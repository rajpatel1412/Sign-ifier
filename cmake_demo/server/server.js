// After creating package.json, install modules:
//   $ npm install
//
// Launch server with:
//   $ node server.js
// var PORT_NUMBER = 8088;


// var http = require('http');
// var fs   = require('fs');
// var path = require('path');
// var mime = require('mime');

// /* 
//  * Create the static web server
//  */
// var server = http.createServer(function(request, response) {
// 	var filePath = false;
	
// 	if (request.url == '/') {
// 		filePath = 'public/index.html';
// 	} else {
// 		filePath = 'public' + request.url;
// 	}
	
// 	var absPath = './' + filePath;
// 	serveStatic(response, absPath);
// });

// // listening to the client (web browser at port 8088)
// server.listen(PORT_NUMBER, function() {
// 	console.log("Server listening on port " + PORT_NUMBER);
// });

// function serveStatic(response, absPath) {
// 	fs.exists(absPath, function(exists) {
// 		if (exists) {
// 			fs.readFile(absPath, function(err, data) {
// 				if (err) {
// 					send404(response);
// 				} else {
// 					sendFile(response, absPath, data);
// 				}
// 			});
// 		} else {
// 			send404(response);
// 		}
// 	});
// }

// function send404(response) {
// 	response.writeHead(404, {'Content-Type': 'text/plain'});
// 	response.write('Error 404: resource not found.');
// 	response.end();
// }

// function sendFile(response, filePath, fileContents) {
// 	response.writeHead(
// 			200,
// 			{"content-type": mime.lookup(path.basename(filePath))}
// 		);
// 	response.end(fileContents);
// }


// /*
//  * Create the Userver to listen for the websocket
//  */

// // sending server to udp_server.js so it can listen to its response
// var udpServer = require('./lib/udp_server');
// udpServer.listen(server);


var express = require('express');
var http = require('http');
var {Server} = require("socket.io");
var startRouter = require('./lib/page.js');
var child = require('child_process');

var app = express();
var server = http.createServer(app);
var io = new Server(server);
var {SERVER_PORT: port=3000} = process.env;

app.use('/', startRouter);

io.on('connection', function(socket) {
	console.log("Connection started");

	var ffmpeg = child.spawn("ffmpeg", ["-re", "-y", "-i", "udp://192.168.7.1:1234",
										"-preset", "ultrafast", "-f", "mjpeg", "pipe:1"]);
	
	ffmpeg.on('error', function(err) {
		console.log(err);
		throw err;
	});

	ffmpeg.on('close', function(code) {
		console.log('ffmpeg exited with code: ' + code);
	});

	ffmpeg.stderr.on('data', function(data) {
		console.log("STDERR");
	});

	ffmpeg.stdout.on('data', function(data) {
		var frame = Buffer.from(data).toString('base64');
		io.sockets.emit('canvas', frame);
	});
});

server.listen({port}, function() {
	console.log("server ready at http://0.0.0.0.${port}");
});