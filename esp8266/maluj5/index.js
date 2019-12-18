'use strict';
const { Servo } = require("johnny-five");
const keypress = require("keypress");
const {
  EtherPortClient
} = require('etherport-client');
const five = require('johnny-five');
keypress(process.stdin);

const board = new five.Board({
  port: new EtherPortClient({
    host: '192.168.11.19',
    port: 3030
  }),
  repl: false
});

board.on("ready", () => {

  console.log("Use Up and Down arrows for CW and CCW respectively. Space to stop.");

  const servo = new Servo.Continuous(5);

  const led = new five.Led(2);

  const servo2 = new five.Servo({
    pin: 4,
    startAt: 20
  });

  process.stdin.resume();
  process.stdin.setEncoding("utf8");
  process.stdin.setRawMode(true);

  process.stdin.on("keypress", (ch, key) => {

    if (!key) {
      return;
    }

    if (key.name === "q") {
      console.log("Quitting");
      process.exit();
    } else if (key.name === "up") {
      console.log("CW");
      servo.cw();
    } else if (key.name === "down") {
      console.log("CCW");
      servo.ccw();
    } else if (key.name === "space") {
      console.log("Stopping");
      servo.stop();
    } else if (key.name === "o") {
      console.log("on");
      led.on();
    } else if (key.name === "f") {
      console.log("off");
      led.off();
    } else if (key.name === "i") {
      console.log("iiii");
      servo2.to(170);
    } else if (key.name === "u") {
      console.log("uuuu");
      servo2.home();
    }
  });
});
