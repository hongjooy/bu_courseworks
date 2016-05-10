$(document).ready(function() {

	console.log("noise started");


  console.log("now: ",cowid);

//------------------- Initial Ajax Call -----------------------
    $.ajax({
    type:'GET',
    url:'/json/cows/'+ cowid, //{{cowid}}
    success: function(response){
/****************************************************************************************/        
        var Cowdata = response;
        /* Noise Level via Line Chart */

        var noise_data_size = Cowdata.cow_noise.length;
        var noiselevel_list=[];
        var noisetime_list=[];
        var year_list=[];
        var month_list=[];
        var day_list=[];
        var hour_list=[];
        var min_list=[];
        var sec_list=[];
        var temp=[]; // temp list for time parsing 
        for (var i = 0; i < 39; i++){
         if (typeof Cowdata.cow_noise[noise_data_size-39+i]!= 'undefined'){
           noiselevel_list[i] = Cowdata.cow_noise[noise_data_size-39+i].micaverage;
           noisetime_list[i] = Cowdata.cow_noise[noise_data_size-39+i].timemeasured;
         }
         else{
          noiselevel_list=0;
          noisetime_list=' ';
         }
       	}

       	var chart_label = ['','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','',''];

       	var noise_data = {
       		labels: chart_label,
       		datasets:[
       		{
       			label: "Current Noise",
	            fillColor: "rgba(220,220,220,0.2)",
              strokeColor: "rgba(220,220,220,1)",
              pointColor: "rgba(220,220,220,1)",
              pointStrokeColor: "#fff",
              pointHighlightFill: "#fff",
              pointHighlightStroke: "rgba(220,220,220,1)",
	            data: noiselevel_list
       		},
          
          ]
       	}
       	var noise = document.getElementById("noise").getContext("2d");
       	var noiseChart = new Chart(noise).Line(noise_data,{
              scaleOverride : true,
              scaleSteps : 10,
              scaleStepWidth : 50,
              scaleStartValue : 0, 

       	});


    } //success function 

    }); //

});





 