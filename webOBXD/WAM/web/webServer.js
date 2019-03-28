'use strict';


const express = require('express');
const app = express();

var portno = 3000;   // Port number to use

app.use(express.static(__dirname));

var server = app.listen(portno, function () {
  var port = server.address().port;
  console.log('Listening at http://localhost:' + port + ' exporting the directory ' + __dirname);
});