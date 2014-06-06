var osc = require("osc");
var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io').listen(http);

// Create an osc.js UDP Port listening on port 57121.
var udpPort = new osc.UDPPort({
    localAddress: "0.0.0.0",
    localPort: 50000
});

// Listen for incoming OSC bundles.
udpPort.on("bundle", function (oscBundle) {
    console.log("An OSC bundle just arrived!", oscBundle);
    // send the packets to the browser
    io.sockets.emit('news', oscBundle.packets);
    console.log( oscBundle.packets );
});

// Open the socket.
udpPort.open();


// Send an OSC message to, say, SuperCollider
function sendOSC(adr, msg) {
	udpPort.send({
		// address: "/s_new",
		address: adr,
		//args: ["default", 100]
		args: msg
	}, '192.168.1.30', 40000);
}


app.get('/', function(req, res){
  res.sendfile('index.html');
});

// Make whatever is in the /static folder available
app.use(express.static('static'));


io.on('connection', function (socket) {
  socket.emit('news', { hello: 'world' });
  
  socket.on('my other event', function (data) {
    console.log(data);
  });
  
  socket.on('message', function (data) {
	console.log(data);
	sendOSC(data.address, data.args[0]);
  });
  
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});
