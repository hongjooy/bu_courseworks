$("#signup").click(function(event){

	var pass = $("#pass").val();
	var passconfirm = $("#passconfirm").val();

	if(pass != passconfirm){
		$("#wrong").html("PASSWORD NO MATCH");
		console.log("failed");
		event.preventDefault();
	} else{
		$("#signup").click();
		console.log("success");
		window.location.href = "/mooove/login";
	}

})