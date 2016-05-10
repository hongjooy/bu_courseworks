$(document).ready(function() {


//------------------- Initial Ajax Call -----------------------
    $.ajax({
    type:'GET',
    url:'/json/cows/'+ cowid, //{{cowid}}
    success: function(response){
/****************************************************************************************/        
        var Cowdata = response;
        /* Pulse Level via Line Chart */

        var step_data_size = Cowdata.cow_stepcount.length;
        var stepcount_list=[];
        var steptime_list=[];
        var year_list=[];
        var month_list=[];
        var day_list=[];
        var hour_list=[];
        var min_list=[];
        var sec_list=[];
        var temp=[]; // temp list for time parsing 
        //console.log("come on workworkwork!!", Cowdata.cow_pulse[0].pulselevel);

        for (var i = 0; i < 20; i++){
          if (typeof Cowdata.cow_stepcount[step_data_size-20+i]!= 'undefined'){
            stepcount_list[i] = Cowdata.cow_stepcount[step_data_size-20+i].stepcount;
            steptime_list[i] = Cowdata.cow_stepcount[step_data_size-20+i].datastarttime;
          }
          else{
             stepcount_list[i] = 0;
             steptime_list[i] = ' ';
          }

        }
      
        var chart_label = ['','','','','','','','','','','','','','','','','','','',''];
        var step_data = {
          labels: chart_label,
          datasets:[
          {
            label: "Steps",
              fillColor: "rgba(255,255,0,0.2)",
              strokeColor: "rgba(255,255,0,0.8)",
              pointColor: "rgba(255,255,0,0.8)",
              pointStrokeColor: "#fff",
              pointHighlightFill: "#fff",
              pointHighlightStroke: "rgba(220,220,220,1)",
              data: stepcount_list
          },
          
          ]
        }
        var step = document.getElementById("step").getContext("2d");
        var stepChart = new Chart(step).Line(step_data,{

        });


    } //success function 

    }); //

});
