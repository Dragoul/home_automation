var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/searching', function(req, res) {
    var val = req.query.search;
    console.log(val);
});

sendMessage = function (house, unit, state, times, fun) {
    var exec  = require('child_process').exec, child;

    var cmd = '/www/myapp/src/light '
	+ house + ' '
	+ unit + ' '
	+ state + ' '
	+ times;
    console.log('Running command: ' + cmd);
    var child = exec(cmd);

    child.stdout.on('data', function(data) {
	console.log(data);	
    });

    child.stderr.on('data', function(data) {
	console.log('stderr: ' + data);	
    });

    child.on('close', function(code) {
	console.log('Exit code: ' + code);
	fun();
    });
}
router.post('/lights', function(req, res) {
    console.log(req.body);
    console.log('Peer address: ' + req.socket._peername.address);
    console.log('House: ' + req.body.house);
    console.log('Unit: ' + req.body.unit);
    console.log('State: ' + req.body.state);

    var house = req.body.house;
    var unit = req.body.unit;
    var state = req.body.state;

    sendMessage(house, unit, state, 5, function() {
	res.send({house: house, unit: unit, state: state});
    });
});

router.post('/', function(req, res) {
    console.log(req.body);
    console.log('Peer address: ' + req.socket._peername.address);
    console.log('House: ' + req.body.house);
    console.log('Unit: ' + req.body.unit);
    console.log('State: ' + req.body.state);

    var house = req.body.house;
    var unit = req.body.unit;
    var state = req.body.state;

    sendMessage(house, unit, state, 5, function() {
	res.render('index',
		   {
		       title: 'Lamp controller',
		       house: house,
		       unit: unit,
		       state: state
		   });
    });
});


router.get('/', function(req, res) {    
    res.render('index',
	       {
		   title: 'Lamp controller',
		   house: req.params.house,
		   unit: req.params.unit,
		   state: req.params.state
	       });
});

module.exports = router;
