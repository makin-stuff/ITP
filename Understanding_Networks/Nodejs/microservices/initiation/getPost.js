/*
        Express.js GET/POST example
        Shows how to get the parameters of a GET vs a POST request
        in Express.js 4.0

        created 10 Feb 2015
  modified 4 Feb 2018
        by Tom Igoe
*/

var express = require('express');                           // include express.js
var server = express();                                                 // a local instance of it
var bodyParser = require('body-parser');        // include body-parser
server.use('/',express.static('public')); // serve static files from /public

// you need a couple of parsers for the body of a POST request:
server.use(bodyParser.json());                                            // for  application/json
server.use(bodyParser.urlencoded({extended: false})); // for application/x-www-form-urlencoded

// this runs after the server successfully starts:
function serverStart() {
  var port = this.address().port;
  console.log('Server listening on port '+ port);
}

function start(request, response) {
        response.write("Greetings. Priyanka is online.\n");
        response.write("Please enter your name and a number into the URL above.");
        response.end();
}

function congrats(request, response) {
        response.write("Congratulations " + request.params.name + ", initiation is now complete. As y>
        response.write("I am online.\n");
        response.write("You are online.\n");
        response.write("Priyanka is online.\n");
        response.end();
}

// start the server:
server.listen(process.env.PORT || 8080, serverStart);
server.get('/initiation', start);    // GET request listener
server.get('/initiation/:name/:num', congrats); // GET request listener 