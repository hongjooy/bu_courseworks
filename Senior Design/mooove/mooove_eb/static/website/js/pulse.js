$(document).ready(function() {

  console.log("pulse started");
  console.log("please: ",cow_id);

//------------------- Initial Ajax Call -----------------------
    $.ajax({
    type:'GET',
    url:'/json/cows/'+ cowid, //{{cowid}}
    success: function(response){
/****************************************************************************************/        
        var Cowdata = response;
        /* Pulse Level via Line Chart */

        var pulse_data_size = Cowdata.cow_pulse.length;
        var pulselevel_list=[];
        var pulsetime_list=[];
        var year_list=[];
        var month_list=[];
        var day_list=[];
        var hour_list=[];
        var min_list=[];
        var sec_list=[];
        var temp=[]; // temp list for time parsing 
        //console.log("come on workworkwork!!", Cowdata.cow_pulse[0].pulselevel);
        if (typeof Cowdata.cow_pulse[44] == 'undefined'){
          console.log("?????");
        }
        for (var i = 0; i < 20; i++){
          if (typeof Cowdata.cow_pulse[pulse_data_size-20+i]!= 'undefined'){
            pulselevel_list[i] = Cowdata.cow_pulse[pulse_data_size-20+i].pulselevel;
            pulsetime_list[i] = Cowdata.cow_pulse[pulse_data_size-20+i].timemeasured;
          }
          else{
             pulselevel_list[i] = 0;
             pulsetime_list[i] = ' ';
          }

        pulselevel_list = [52,55,48,50,50,57,50,51,51,52,52,55,55,60,61,62,62,63,53,54];

        year_list[i] = temp[0];
        month_list[i] = temp[1];
        day_list[i] = temp[2];
        hour_list[i] = temp[3];
        min_list[i] = temp[4];
        sec_list[i] = temp[5];
        }
  
        var chart_label = ['','','','','','','','','','','','','','','','','','','',''];
        var pulse_data = {
          labels: chart_label,
          datasets:[
          {
            label: "Current Noise",
              fillColor: "rgba(255,0,0,0.2)",
              strokeColor: "rgba(255,0,0,0.8)",
              pointColor: "rgba(255,0,0,0.8)",
              pointStrokeColor: "#fff",
              pointHighlightFill: "#fff",
              pointHighlightStroke: "rgba(220,220,220,1)",
              data: pulselevel_list
          },
          
          ]
        }
        var pulse = document.getElementById("pulse").getContext("2d");
        var pulseChart = new Chart(pulse).Line(pulse_data,{

        });


    } //success function 

    }); //

});





 