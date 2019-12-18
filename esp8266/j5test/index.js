// const { Board, Servo } = require("johnny-five");
var five = require("johnny-five");
const keypress = require("keypress");

keypress(process.stdin);

var arduino = new five.Board();

arduino.on("ready", () => {

  console.log("Use Up and Down arrows for CW and CCW respectively. Space to stop.");

  /*Initialize pwm*/
  // var m_pwm = new five.Pin({ pin: 5, mode: five.Pin.PWM });

  var servox = new five.Servo({
    pin: 5,
    range: [0, 180],
    pwmRange: [8, 1842]
  });

  var led = new five.Led(2);

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
      led.on();
      // arduino.io.pwmWrite(m_pwm.pin, 80);
      servox.to(10);
    } else if (key.name === "down") {
      console.log("CCW");
      led.off();
      // arduino.io.pwmWrite(m_pwm.pin, 90);
      servox.to(90);
    } else if (key.name === "space") {
      console.log("Stopping");
      // servo.stop();
    }
  });
});
