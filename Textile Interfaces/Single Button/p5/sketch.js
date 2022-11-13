// This code comes from an ITP PComp lab found here: 
// https://itp.nyu.edu/physcomp/labs/labs-serial-communication/lab-webserial-input-to-p5-js/


// variable to hold an instance of the p5.webserial library:
const serial = new p5.WebSerial();

// HTML button object:
let portButton;
let inData;  // for incoming serial data
let outByte = 0;  // for outgoing data

let buttonState = 1;

// Set up all the sound things!
let laughSound;

function preload() {
  soundFormats('mp3', 'ogg');
  laughSound = loadSound('assets/laugh.mp3');
}

function setup() {
  // load the sound
  laughSound.setVolume(1);
  createCanvas(400, 400);
  // Check to see if serial is available:
  if (!navigator.serial) {
    alert("WebSerial is not supported in this browser. Try Chrome or MS Edge.");
  }
  
  // if serial is available, add connect/disconnect listeners:
  navigator.serial.addEventListener("connect", portConnect);
  navigator.serial.addEventListener("disconnect", portDisconnect);
  // check for any ports that are available:
  serial.getPorts();
  // if there's no ports chosen, choose one:
  serial.on("noport", makePortButton);
  // open whatever port is available:
  serial.on("portavailable", openPort);
  // handle serial errors:
  serial.on("requesterror", portError);
  // handle any incoming serial data:
  serial.on("data", serialEvent);
  serial.on("close", makePortButton);
}

function draw() {
  background(0);
  fill(255);
  text("sensor value: " + inData, 30, 50);
  if (inData == 0) {
    console.log("hello!");
    if (!laughSound.isPlaying()) {
      laughSound.play();
    }
  }
}

// if there's no port selected, 
// make a port select button appear:
function makePortButton() {
  // create and position a port chooser button:
  portButton = createButton("choose port");
  portButton.position(10, 10);
  // give the prot button a mousepressed handler:
  portButton.mousePressed(choosePort);
}

// make the port selector window appear:
function choosePort(){
  if (portButton) portButton.show();
  serial.requestPort();
}

// open the selected port, and make the port button invisible:
function openPort() {
  // wait for the serial.open promise to return,
  // then call the initiateSerial function
  serial.open().then(initiateSerial);
  
  // once the port opens, let the user know:
  function initiateSerial() {
    console.log("port open");
  }
  
  // hide the port button once a port is chosen:
  if (portButton) portButton.hide();
}

// pop up an alert if there's a port error:
function portError(err) {
  alert("Serial port error: " + err);
}

// read any incoming data as a string
// (assumes a newline at the end of it)
function serialEvent() {
  inData = Number(serial.read());
}

// try to connect if a new serial port gets added (ie. plugged in via USB)
function portConnect() {
  console.log("port connected");
  serial.getPorts();
}

// if a port is disconnected:
function portDisconnect() {
  serial.close();
  console.log("port disconnected");
}

function closePort() {
  serial.close();
}

