var express = require('express');
var path = require('path');

var router = express.Router();
var filePath = "/../public/";

router.get('/', function(req, res) {
    res.sendFile(path.join(__dirname + filePath + "index.html"));
});

router.get('/javascripts/script.js', function(req, res) {
    res.sendFile(path.join(__dirname + filePath + "/javascripts/script.js"));
});

module.exports = router;