var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/searching', function(req, res) {
    var val = req.query.search;
    console.log(val);
});

//router.param('times', '/^\d$/');

sendMessage = function (house, unit, state, times) {
    var exec  = require('child_process').exec, child;

    child = exec('/root/work/433.out '
		 + house + ' '
		 + unit + ' '
		 + state + ' '
		 + times,
		 function(error, stdout, stderr) {
		     console.log('stdout: ' + stdout);
		     console.log('stderr: ' + stderr);
		     
		     if(error !== null) {
			 console.log('exec error: ' + error);
		     }
		 }
		)
}

router.post('/', function(req, res) {
//    console.log(req);
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
