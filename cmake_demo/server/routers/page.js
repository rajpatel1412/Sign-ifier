// Source: adapted from Team Solar streaming Guide
// https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/links/files/2022-student-howtos/StreamingWebcamFromBeagleBoneToNodeJSServer.pdf

const express = require('express');
const router = express.Router();
const path = require('path');
const filePath = "/../public/";

router.get('/', function(req, res) {
	res.sendFile(path.join(__dirname + filePath + "homepage.html"));
});

router.get('/script.js', function(req, res) {
	res.sendFile(path.join(__dirname + filePath + "script.js"));
});

module.exports = router;
