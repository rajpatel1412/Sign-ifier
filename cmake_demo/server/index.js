const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);
const startRouter = require('./routers/page.js');
const {SERVER_PORT: port = 3000} = process.env;
const child = require('child_process');

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

server.listen({ port }, () => {
        console.log(`🚀 Server ready at http://0.0.0.0:${port}`);
});
