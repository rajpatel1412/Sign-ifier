const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);
const startRouter = require('./routers/page.js');
const {SERVER_PORT: port = 3000} = process.env;
const child = require('child_process');
var dgram = require('dgram');

var ffmpegCommand = require('fluent-ffmpeg');
const fs = require('fs');
// var command = ffmpeg();

app.use('/', startRouter);


// const inputUrl = 'udp://192.168.7.1:1234';

// // Create a writable stream to capture ffmpeg output
// const outputStream = fs.createWriteStream('output.jpg');

// const command = ffmpegCommand(inputUrl)
//   .inputOptions(['-re', '-y']) // Read input at native frame rate
//   .format('mjpeg')
//   .outputOptions([
//     '-preset ultrafast', // Set encoding preset
//     '-f mjpeg',          // Specify MJPEG output format
//   ])
//   .output(outputStream)  // Output to a writable stream
//   .on('start', function(commandLine) {
//     console.log('Spawned ffmpeg with command: ' + commandLine);
//   })
//   .on('stdout', function (data) {
//         var frame = Buffer.from(data).toString('base64'); //convert raw data to string
//         io.sockets.emit('canvas',frame); //send data to client
//   })
//   .on('error', function(err) {
//     console.error('Error:', err);
//   })
//   .on('end', function() {
//     console.log('Processing finished');
//   });

io.on('connection', (socket) => {
        console.log('a user connected');

        let ffmpeg = child.spawn("ffmpeg", [
                "-re", 
                "-y", 
                "-i", 
                "udp://192.168.7.1:1234",
                "-preset",  
                "ultrafast", 
                "-f", 
                "mjpeg", 
                "pipe:1"
                ]);
        

        ffmpeg.on('error', function (err) {
                console.log(err);
                throw err;
        });

        ffmpeg.on('close', function (code) {
                console.log('ffmpeg exited with code ' + code);
        });

        ffmpeg.stderr.on('data', function(data) {
                // Don't remove this
                // Child Process hangs when stderr exceed certain memory
        });

        ffmpeg.stdout.on('data', function (data) {
                var frame = Buffer.from(data).toString('base64'); //convert raw data to string
                io.sockets.emit('canvas',frame); //send data to client
        });

        console.log('streamed, about to get messages from C');


        handleCommand(socket);


        // var command = ffmpeg("udp://192.168.7.1:1234").inputOptions([
        //         "-re", 
        //         "-y",  
        //         "-preset ultrafast", 
        //         "-f mjpeg", 
        //         "pipe:1"
        // ])

        // var ffmpeg = ffmpegCommand("udp://192.168.7.1:1234")
        //                 .input("udp://192.168.7.1:1234")
        //                 .native()
        //                 .output("pipe:1")
        //                 .format("mjpeg")
        //                 .outputOptions("-preset ultrafast");
        
        // ffmpeg.on('error', function (err) {
        //         console.log(err);
        //         throw err;
        // });

        // ffmpeg.on('close', function (code) {
        //         console.log('ffmpeg exited with code ' + code);
        // });

        // // ffmpeg.stderr.on('data', function(data) {
        // //         // Don't remove this
        // //         // Child Process hangs when stderr exceed certain memory
        // // });

        // ffmpeg.on('data', function (data) {
        //         var frame = Buffer.from(data).toString('base64'); //convert raw data to string
        //         io.sockets.emit('canvas',frame); //send data to client
        // });

        // command.run()


});

// io.on('daUdpCommand', function(data) {
//         console.log("recieiving messages from C");
        // var PORT = 3001;
        // var HOST = '192.168.7.2';
        // var buffer = new Buffer(data);

        // var client = dgram.createSocket('udp4');
        // // send message to C
        // client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes) {
        //         if (err) 
        //                 throw err;
        //         console.log('UDP message sent to ' + HOST +':'+ PORT);
        // });

        // client.on('listening', function () {
        //         var address = client.address();
        //         console.log('UDP Client: listening on ' + address.address + ":" + address.port);
        // });
        // // Handle an incoming message over the UDP from the local application.

        // var udpErrorTimer = setTimeout(function() {
        //         io.emit('udpTimeoutError',
        //         // socket.emit('udpTimeoutError', 
        //         "UDP client on target failed to respond");
        // }, 1000);

        // // send message to website
        // client.on('message', function (message, remote) {
        //         console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);

        //         var reply = message.toString('utf8')
        //         // socket.emit('commandReply', reply);
        //         // io.sockets.emit('commandReply',reply); //send data to client
        //         io.emit('commandReply',reply);

        //         client.close();
        //         clearTimeout(udpErrorTimer);

        // });
        // client.on("UDP Client: close", function() {
        //         console.log("closed");
        // });
        // client.on("UDP Client: error", function(err) {
        //         console.log("error: ",err);
        // });        
// });





function handleCommand(socket) {
	// Pased string of comamnd to relay
        socket.on('daUdpCommand', function(data) {
	// socket.on('daUdpCommand', function(data) {
		// console.log('daUdpCommand command: ' + data);


                console.log("sending messages from C " + data);     
		var PORT = 3001;
		var HOST = '192.168.7.2';
		var buffer = new Buffer(data);

		var client = dgram.createSocket('udp4');
                // send message to C
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

		var udpErrorTimer = setTimeout(function() {
			io.sockets.emit('udpTimeoutError',
                        // socket.emit('udpTimeoutError', 
			"UDP client on target failed to respond");
		}, 1000);

                // send message to website
		client.on('message', function (message, remote) {
			console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);

			var reply = message.toString('utf8')
                        console.log("receiving from C " + reply);
			// socket.emit('commandReply', reply);
                        io.sockets.emit('commandReply',reply); //send data to client

			client.close();
			clearTimeout(udpErrorTimer);

		});
		client.on("UDP Client: close", function() {
			console.log("closed");
		});
		client.on("UDP Client: error", function(err) {
			console.log("error: ",err);
		});
        // };
	});
};

server.listen({ port }, () => {
        console.log(`🚀 Server ready at http://0.0.0.0:${port}`);
});
