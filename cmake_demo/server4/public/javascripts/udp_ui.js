"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();
$(document).ready(function() {

	// check if server is running or not
	// https://stackoverflow.com/questions/8028294/how-can-i-detect-when-the-connection-of-the-client-gets-interrupted-in-socket-i
	socket.on('connect', function() {
		console.log('Connected to server');
		if($("#error-box").show()) {
			$("#error-box").hide();
		}
	});
	socket.on('disconnect', function() {
		console.log('Disconnected from server');
		$("#error-text").html("No response from server");
		$("#error-box").show();
	});

	// continously update volume, uptime, beat mode and tempo
	window.setInterval(function() {sendCommandViaUDP("uptime")}, 1000);
	window.setInterval(function() {sendCommandViaUDP("volume")}, 1000);
	window.setInterval(function() {sendCommandViaUDP("tempo")}, 1000);
	window.setInterval(function() {sendCommandViaUDP("beat")}, 1000);


	$('#modeNone').click(function(){
		sendCommandViaUDP("beat0");
		console.log("Mode 0: none")
	});
	$('#modeRock1').click(function(){
		sendCommandViaUDP("beat1");
	});
	$('#modeRock2').click(function(){
		sendCommandViaUDP("beat2");
	});
	$('#volumeDown').click(function(){
		sendCommandViaUDP("vol-");
	});
	$('#volumeUp').click(function(){
		sendCommandViaUDP("vol+");
	});
	$('#tempoDown').click(function(){
		sendCommandViaUDP("tempo-");
	});
	$('#tempoUp').click(function(){
		sendCommandViaUDP("tempo+");
	});
	$('#Hi-Hat').click(function(){
		sendCommandViaUDP("hi_hat");
	});
	$('#Snare').click(function(){
		sendCommandViaUDP("snare");
	});
	$('#Base').click(function(){
		sendCommandViaUDP("base");
	});
	$('#stop').click(function(){
		sendCommandViaUDP("stop");
	});

	socket.on('commandReply', function(result) {
		console.log(result);

		var results = result.split(' ');

		// parse reply to update volume, uptime, tempo and beat mode
		if(results[0] == 'uptime') {
			var time = new Date((results[2]) * 1000);
			// https://www.geeksforgeeks.org/how-to-convert-seconds-to-time-string-format-hhmmss-using-javascript/
			var hours = time.getUTCHours();
			var minutes = time.getUTCMinutes();
			var seconds = time.getUTCSeconds();
			$("#status").html("Device up for: " + hours + ":" + minutes +
								":" + seconds + "(H:M:S)");
			
		}
		if(results[0] == 'volume') {
			$("#volumeid").val(results[2]);
		}
		if(results[0] == 'tempo') {
			$("#tempoid").val(results[2]);
		}
		if(results[0] == 'beat') {
			if(results[2] == 0) {
				$("#modeid").html("None");
			} else if (results[2] == 1) {
				$("#modeid").html("Rock 1");
			} else {
				$("#modeid").html("We Will Rock You");
			}
		}

		if($("#error-box").show()) {
			// setTimeout(5000);
			$("#error-box").hide();
		}
	});

	socket.on('udpTimeoutError', function(result) {
		$("#error-text").html(result);
		$("#error-box").show();
	});
	
});

function sendCommandViaUDP(message) {
	socket.emit('daUdpCommand', message);
};
