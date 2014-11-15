
$(function() {
    $('#btn').click(function() {
	var parameters = {
	    house: $("#house").val(),
	    unit: $("#unit").val(),
	    state: $("#state").val()
	};
	$('#results').html("Processing");
	$.post( '/lights', parameters, function(data) {
	    $('#results').html("test5");
	    $('#results').html("Unit " + data.unit + " in house " + data.house + " is now in state " + data.state);
	});
    });
});