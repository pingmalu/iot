var five = require("johnny-five");
var EtherPortClient = require("etherport-client").EtherPortClient;
// update host to the IP address for your ESP board
var board = new five.Board({
    port: new EtherPortClient({
        host: "192.168.11.19",
        port: 3030
    }),
    timeout: 1e5,
    repl: true
});

board.on("ready", () => {
    const servo = new five.Servo(5);
  
    // Servo alternate constructor with options
    /*
    var servo = new five.Servo({
      id: "MyServo",     // User defined id
      pin: 10,           // Which pin is it attached to?
      type: "standard",  // Default: "standard". Use "continuous" for continuous rotation servos
      range: [0,180],    // Default: 0-180
      fps: 100,          // Used to calculate rate of movement between positions
      invert: false,     // Invert all specified positions
      startAt: 90,       // Immediately move to a degree
      center: true,      // overrides startAt if true and moves the servo to the center of the range
    });
    */
  
    // Add servo to REPL (optional)
    board.repl.inject({
      servo
    });
    servo.min(); 
    servo.max(); 
    
    // Servo API
  
    // min()
    //
    // set the servo to the minimum degrees
    // defaults to 0
    //
    // eg. servo.min();
  
    // max()
    //
    // set the servo to the maximum degrees
    // defaults to 180
    //
    // eg. servo.max();
  
    // center()
    //
    // centers the servo to 90°
    //
    // servo.center();
  
    // to( deg )
    //
    // Moves the servo to position by degrees
    //
    // servo.to( 90 );
  
    // step( deg )
    //
    // step all servos by deg
    //
    // eg. array.step( -20 );
  
    servo.sweep();
  });


// board.on("ready", function () {
//     console.log("Ready event. Repl instance auto-initialized!");

//     var led = new five.Led(2);
//     const servo = new five.Servo(5);

//     this.repl.inject({
//         // Allow limited on/off control access to the
//         // Led instance from the REPL.
//         on: function () {
//             led.on();
//         },
//         off: function () {
//             led.off();
//         },
//         g0: function () {
//             servo.to(0);
//         },
//         g1: function () {
//             servo.to(180);
//         }
//     });
//     servo.sweep();
// });


// "etherport-client": "^0.1.4",
// "firmata": "^2.0.0",
// "johnny-five": "^1.3.1"