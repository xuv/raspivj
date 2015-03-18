// Defaults
var multiPlayerAddress = "127.0.0.1";
//var multiPlayerAddress = "192.168.1.30";
var multiPlayerPort = 40000;

// the folder should be in the /home/pi user directory, at the same level as the raspivj/ directory
var videoFolder = "videos"; 

// Necessary for calling shell scripts
// http://valler.ca/nodejs-execute-a-shell-script-command/
var sys = require('sys');
var exec = require('child_process').exec;
function puts(error, stdout, stderr) { sys.puts(stdout) }

var fs = require('fs');
var osc = require("osc");
var express = require('express');


// Dummy users
var clips = [];
clips = fs.readdirSync(__dirname + '/' + videoFolder + '/thumbs');
clips.sort();

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

/* Send an OSC message to multiPlayer
 *      address: "/s_new",
 *      args: ["default", 100]
 */
function sendOSC(adr, msg) {
	udpPort.send({
		address: adr,
		args: msg
	}, multiPlayerAddress, multiPlayerPort);
}

// Set the default template engine to "jade"
app.set('views', __dirname + '/views');
app.set('view engine', 'jade');

app.get('/', function(req, res){
  res.render( 'index', {clips: clips});
});

// change this to a better error handler in your code
// sending stacktrace to users in production is not good
app.use(function(err, req, res, next) {
  res.send(err.stack);
});

// Make whatever is in the /static folder available
app.use(express.static( __dirname + '/static'));
// Have the video thumbs available
app.use('/thumbs', express.static(__dirname + '/' + videoFolder + '/thumbs'));


io.on('connection', function (socket) {
  socket.emit('news', { hello: 'world' });
  
  socket.on('system', function (data) {
    console.log(data);
    switch (data.address) {
			case "/start":
				console.log('Starting video player');
				exec("sh " + __dirname + "/scripts/start-multiPlayer.sh", puts);
				break;
			case "/stop":
				console.log('Stoping video player');
				sendOSC(data.address, data.args[0]);
				break;
			case "/shutdown":
				console.log('System shutdown');
				exec("sh " + __dirname + "/scripts/shutdown.sh", puts);
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
