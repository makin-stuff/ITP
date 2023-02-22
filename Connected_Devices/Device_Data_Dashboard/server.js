const express = require('express');
const app = express();

const portNum = 8181;

// Serve all the static files in the public folder:
app.use('/sun', express.static("public"));  // with server

function serverStart() {
    console.log("Server started on port " + portNum);
}

// Listen for requests:
app.listen(portNum, serverStart);