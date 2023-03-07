/*
  Bare minimum client example for mqtt.js
  On document load, this script gets two divs from the HTML
  for local and remote messages. Then it attempts
  to connect to the broker. Once every two seconds, 
  it sends the local time if it's connected. 
  The publish button allows you to turn on and off publishing status.
  created 29 Dec 2022
  modified 5 Feb 2023
  by Tom Igoe
*/

// All these brokers work with this code.
// Uncomment the one you want to use.

////// emqx. Works in both basic WS and TLS WS:
// const broker = 'wss://broker.emqx.io:8084/mqtt'
// const broker = 'ws://broker.emqx.io:8083/mqtt'

//////// shiftr.io desktop client. 
// Fill in your desktop IP address for localhost:
// const broker = 'ws://localhost:1884';     

//////// shiftr.io, requires username and password 
// (see options variable below):
const broker = 'wss://public.cloud.shiftr.io';

//////// test.mosquitto.org, uses no username and password:
// const broker = 'ws://test.mosquitto.org:8080';

// MQTT client:
let client;

// connection options:
let options = {
    // Clean session
    clean: true,
    // connect timeout in ms:
    connectTimeout: 10000,
    // Authentication
    clientId: 'mqttJsClient',
    // add these in for public.cloud.shiftr.io:
    username: 'public',
    password: 'public'
}
// topic to subscribe to when you connect:
let topic = 'conndev/makin-stuff';
// divs to show messages:
let localDiv, remoteDiv;
// whether the client should be publishing or not:
let publishing = true;

// Create arrays for rgb values
const redArray = [];
const greenArray = [];
const blueArray = [];

var gridSize = 100;

function setup() {
    // put the divs in variables for ease of use:
    localDiv = document.getElementById('local');
    remoteDiv = document.getElementById('remote');

    // set text of localDiv:
    localDiv.innerHTML = 'trying to connect';
    // attempt to connect:
    client = mqtt.connect(broker, options);
    // set listeners:
    client.on('connect', onConnect);
    client.on('close', onDisconnect);
    client.on('message', onMessage);
    client.on('error', onError);
}

function loop() {

}

// handler for the mqtt connect event:
function onConnect() {
    // update localDiv text:
    localDiv.innerHTML = 'Connected to broker. Subscribing...'
    // subscribe to the topic:
    client.subscribe(topic, onSubscribe);
}

// handler for mqtt disconnect event:
function onDisconnect() {
    // update localDiv text:
    // localDiv.innerHTML = 'Disconnected from broker.'
    console.log("Disconnected from broker.");
}

// handler for mqtt error event:
function onError(error) {
    // update localDiv text:
    localDiv.innerHTML = error;
}

// handler for mqtt subscribe event:
function onSubscribe(response, error) {
    if (!error) {
        // update localDiv text:
        localDiv.innerHTML = 'Subscribed to broker.';
    } else {
        // update localDiv text with the error:
        localDiv.innerHTML = error;
    }
}

// handler for mqtt message received event:
function onMessage(topic, payload, packet) {
    // This code comes from Bianca's dashboard:
    // https://github.com/biancaagan/QuestionableThesisClock/blob/main/old/script.js
    // She's the coolest, idk anything about making webpages
    
    // FOR TESTING:
    console.log("Got a new message!");

    // Message is a buffer, convert to a string:
    let payloadStr = payload.toString();

    // Convert to JSON:
    let payloadJson = JSON.parse(payloadStr);

    // List out the color values:
    let red = payloadJson.red;
    let green = payloadJson.green;
    let blue = payloadJson.blue;

    let result = 'Red: ' + red;
    result += ' Green: ' + green;
    result += ' Blue: ' + blue;

    let time = new Date();
    let hours = time.getHours();
    let minutes = time.getMinutes();
    let seconds = time.getSeconds();
    result += " at " + hours + ":" + minutes + ":" + seconds;

    // result += Date.getTime();

    // TEST: this line makes the page color the color of the sensor reading
    // document.body.style.background = rgb(red, green, blue);

    // Add the values to the array
    redArray.push(red);
    greenArray.push(green);
    blueArray.push(blue);

    // Keep the arrays within a max size so as not to overload the program
    if (redArray.length > gridSize) {
        redArray.shift();   // removes the oldest reading in the array
        greenArray.shift();
        blueArray.shift();
    } 

    // Access rendering context
    const canvas = document.getElementById("myCanvas");
    const ctx = canvas.getContext("2d");

    // FOR TESTING:
    // const tempArray = [15, 45, 75, 105, 135, 165, 195, 225, 255];
    // var i = tempArray.length-1;

    var i = redArray.length-1;
    var xvalue = 0;
    var yvalue = 0;

    for (var x = 0; x < gridSize; x++) {
        if (i < 0) {
            ctx.fillStyle = "rgb(255, 255, 255)";   // Fill squares with no rgb values with white
        } else { 
            ctx.fillStyle = rgb(redArray[i], greenArray[i], blueArray[i]);
        }
        ctx.fillRect(xvalue, yvalue, 50, 50);

        // // FOR TESTING:
        // ctx.fillStyle = rgb(tempArray[i], tempArray[i], tempArray[i]);
        // console.log(tempArray[i]);
        // ctx.fillRect(xvalue, yvalue, 50, 50);
        // FOR TESTING:
        // ctx.strokeRect(xvalue, yvalue, 50, 50);

        xvalue += 50;
        if ((x+1) % 10 == 0) {
            yvalue += 50;
            xvalue = 0;
        }
        // update i which is running through the arrays
        i--;
    }

    remoteDiv.innerHTML = result;
}

function rgb(r, g, b) {
    return "rgb("+r+","+g+","+b+")";
}

// on page load, call the setup function:
document.addEventListener('DOMContentLoaded', setup);
// run a loop every 2 seconds:
// setInterval(loop, 2000);