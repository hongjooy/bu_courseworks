$(document).ready(function() {

	console.log("overview started");

//------------------- Initial Ajax Call -----------------------
    $.ajax({
    type:'GET',
    url:'/json/cows/'+ cowid, //{{cowid}}
    success: function(response){
    	var Cowdata = response;
    	var data_avg = [
    		Math.round(ave_stand.standingpercentage__avg),
    		Math.round(ave_walk.walkingpercentage__avg),
    		Math.round(ave_run.runningpercentage__avg),
    		Math.round(ave_lying.lyingdownpercentage__avg),
    		Math.round(ave_noise.micaverage__avg),
    		Math.round(ave_stepcount.stepcount__avg),
    		3,
    	//	30, //temp temp
    		43 //temp pulse
    		]

    		

    	var data_atm = [
    		Math.round(Cowdata.cow_activity[Cowdata.cow_activity.length - 1].standingpercentage),
    		Math.round(Cowdata.cow_activity[Cowdata.cow_activity.length - 1].walkingpercentage),
    		Math.round(Cowdata.cow_activity[Cowdata.cow_activity.length - 1].runningpercentage),
    		Math.round(Cowdata.cow_activity[Cowdata.cow_activity.length - 1].lyingdownpercentage),
    		Math.round(Cowdata.cow_noise[Cowdata.cow_noise.length - 1].micaverage),
    		Math.round(Cowdata.cow_stepcount[Cowdata.cow_stepcount.length - 1].stepcount),
    		Math.round(Cowdata.cow_social[Cowdata.cow_social.length - 1].sociallevel),
    	//	23, //temp temp
    		40 //temp pulse
    	]
    	console.log(data_avg);
    	console.log(data_atm);
    	console.log("social",ave_sociallevel.sociallevel__avg);
		var overview_data = {
	    	labels: ["Standing","Walking", "Running", "Lyingdown", "Noise", "Exercise", "Social", "Heartbeat"],
	    	datasets: [
	        {
	            label: "Average Health Data",
	            fillColor: "rgba(220,220,220,0.2)",
	            strokeColor: "rgba(220,220,220,1)",
	            pointColor: "rgba(220,220,220,1)",
	            pointStrokeColor: "#fff",
	            pointHighlightFill: "#fff",
	            pointHighlightStroke: "rgba(220,220,220,1)",
	            data: data_avg
	        },
	        {
	            label: "Health Data at the Moment",
	            fillColor: "rgba(151,187,205,0.2)",
	            strokeColor: "rgba(151,187,205,1)",
	            pointColor: "rgba(151,187,205,1)",
	            pointStrokeColor: "#fff",
	            pointHighlightFill: "#fff",
	            pointHighlightStroke: "rgba(151,187,205,1)",
	            data: data_atm
	        }
	    	]
		};  
		
		var overview  = document.getElementById("overview").getContext("2d");
       	var overviewChart = new Chart(overview).Bar(overview_data,{
       		  
       	});
     
        
    } // end success 


    }); //

});
