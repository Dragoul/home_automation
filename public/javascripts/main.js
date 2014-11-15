
$(function() {
    $('#results').html("test1");
    $('#search').on('keyup', function(e) {
	$('#results').html("test2");
	if(e.keyCode === 13) {
	    $('#results').html("test3");
	    var parameters = {
		house: $("#house").val(),
		unit: $("#unit").val(),
		state: $("#state").val()
	    };
	    $('#results').html("test4");
	    $.post( '/', parameters, function(data) {
		$('#results').html("test5");
		$('#results').html("Setting unit " + data.unit);
	    });
	};
    });
 });