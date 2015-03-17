// SETUP
var socket = io.connect();

// alpha & black sliders
$('.slider').slider().on('slide', function(){
	var message = {
		address: "/" + $(this).attr('id'),
		args: [ $(this).slider().data('slider').getValue() ]
	}
	socket.emit('message', message);
});

// players
$('#monitor img').click(function(){
	if( $(this).hasClass('playing') ){
		$(this).removeClass('playing');
		var message = {
			address: "/" + $(this).attr('id') + "/playing",
			args: [ 0 ]
		}
		socket.emit('message', message);
	} else {
		$(this).addClass('playing');
		var message = {
			address: "/" + $(this).attr('id') + "/playing",
			args: [ 1 ]
		}
		socket.emit('message', message);
	}
});

// news
socket.on('news', function (data) {
	console.log('Received Data');
	console.log(data);
	socket.emit('my other event', { my: 'data' });
});

// select a clip to play
$('#clips li a').click( function(e){
	e.preventDefault(); // Don't update page
	var playerID = $('#monitor form input:checked').val();
	var message = {
		address: "/player" + playerID ,
		args: [ parseFloat( $(this).attr('id').substr(4) ) ]
	}
	socket.emit('message', message);
	$('#player' + playerID ).attr('src', $(this).children('img').attr('src') );
});

// system commands
$('.nav a').click(function(){
	var message = {
		address: "/" + $(this).attr('id') ,
		args: [ 1 ]
	}
	if( confirm( $(this).attr('title') ) ) {
		socket.emit('system', message);
	}

});
