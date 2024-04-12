const path = require('path')
const express = require('express');
const app = express();
const server = require('http').Server(app);
const io = require('socket.io')(server);

app.get('/', function(rew, res) {
    res.sendFile(path.join(__dirname, 'index.html'));
});

setInterval( function() {
    io.emit('image', data);
}, 1000);

server.listen(3000);