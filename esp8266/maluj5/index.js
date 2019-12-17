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

board.on("ready", function () {
    console.log("Ready event. Repl instance auto-initialized!");

    var led = new five.Led(2);
    const servo = new five.Servo(5);

    this.repl.inject({
        // Allow limited on/off control access to the
        // Led instance from the REPL.
        on: function () {
            led.on();
        },
        off: function () {
            led.off();
        },
        g0: function () {
            servo.to(0);
        },
        g1: function () {
            servo.to(180);
        },
    });
});


// "etherport-client": "^0.1.4",
// "firmata": "^2.0.0",
// "johnny-five": "^1.3.1"