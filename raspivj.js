// Defaults

var multiPlayerAddress = "127.0.0.1";
//var multiPlayerAddress = "192.168.1.30";
var multiPlayerPort = 40000;


// Necessary for calling shell scripts
// http://valler.ca/nodejs-execute-a-shell-script-command/
var sys = require('sys');
var exec = require('child_process').exec;
function puts(error, stdout, stderr) { sys.puts(stdout) }

var osc = require("osc");
var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io').listen(http);

// Create an osc.js UDP Port listening on port 50000.
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

// Send an OSC message to multiPlayer
// address: "/s_new",
// args: ["default", 100]
function sendOSC(adr, msg) {
	udpPort.send({
		address: adr,
		args: msg
	}, multiPlayerAddress, multiPlayerPort);
}

app.get('/', function(req, res){
  res.sendfile( __dirname + '/index.html');
});

// Make whatever is in the /static folder available
app.use(express.static( __dirname + '/static'));


io.on('connection', function (socket) {
  socket.emit('news', { hello: 'world' });
  
  socket.on('system', function (data) {
    console.log(data);
    switch (data.address) {
			case "/start":
				console.log('Starting video player');
				exec("sh ~/raspivj/scripts/start-multiPlayer.sh", puts);
				break;
			case "/stop":
				console.log('Stoping video player');
				sendOSC(data.address, data.args[0]);
				break;
			case "/shutdown":
				console.log('System shutdown');
				exec("sh ~/raspivj/scripts/shutdown.sh", puts);
				break;
	}
  });
  
  socket.on('message', function (data) {
	console.log(data);
	sendOSC(data.address, data.args[0]);
  });
  
});

http.listen(3000, function(){
  console.log('listening on *:3000');
  console.log('__dirname: ' + __dirname);
});
