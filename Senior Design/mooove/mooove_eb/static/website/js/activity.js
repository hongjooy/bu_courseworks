$(document).ready(function() {

//------------------- Ajax Call -----------------------
    $.ajax({
    type:'GET',
    url:'/json/cows/'+ cowid, //{{cowid}}
    success: function(response){
/****************************************************************************************/        
        var Cowdata = response;
        /* Activity Level Data via Donut Chart */
        var act_data_size = Cowdata.cow_activity.length;
        var act_stand = Cowdata.cow_activity[act_data_size-1].standingpercentage;
        var act_walk = Cowdata.cow_activity[act_data_size-1].walkingpercentage;
        var act_run = Cowdata.cow_activity[act_data_size-1].runningpercentage;
        var act_lying = Cowdata.cow_activity[act_data_size-1].lyingdownpercentage;
        var in_heat = Cowdata.cow_activity[act_data_size-1].inheat;
        
        var act_chart_data = [
            {
                value: act_stand,
                color:"#F7464A",
                highlight: "#FF5A5E",
                label: "Standing (%)"
            },
            { 
                value: act_walk,
                color: "#46BFBD",
                highlight: "#5AD3D1",
                label: "Walking (%)"
            },
            {
                value: act_run,
                color: "#FDB45C",
                highlight: "#FFC870",
                label: "Running (%)"
            },
            {
                value: act_lying,
                color: "#949FB1",
                highlight: "#A8B3C5",
                label: "Lying (%)"
            }
        ] 
        var activity = document.getElementById("activity").getContext("2d");
        var activityChart = new Chart(activity).Doughnut(act_chart_data,{
        })
        document.getElementById('act-legend').innerHTML = activityChart.generateLegend();
    } 
    }); //
});

