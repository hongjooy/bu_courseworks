
$(document).ready(function() {

//------------ Ajax Function -------------
    function ajax_call(){

        $.ajax({
        type:'GET',
        url:'/json/cows/'+ cowid, //{{cowid}}
        success: function(response){
            var jsondata = response;
            console.log("jasondata:",jsondata);
            }
        });
    };
    setInterval(function(){ ajax_call(); }, 60000*5); // get data every 5 minutes 



//--------------- Chartjs Global Setting ---------------------

    Chart.defaults.global.animationSteps = 50;
    Chart.defaults.global.tooltipYPadding = 10;
    Chart.defaults.global.tooltipCornerRadius = 4;
    Chart.defaults.global.tooltipTitleFontStyle = "normal";
    Chart.defaults.global.tooltipFillColor = "rgba(0,160,0,0.8)";
    Chart.defaults.global.animationEasing = "easeOutBounce";
    Chart.defaults.global.responsive = true;
    Chart.defaults.global.scaleLineColor = "black";
    Chart.defaults.global.scaleFontSize = 10;
    Chart.defaults.global.pointDotRadius = 4;


//-------------- Line Sample -------------------------    
  var lineChartData = {
        labels: ["05:00", "06:00", "07:00", "08:00", "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00"],
        datasets: [{
            fillColor: "rgba(220,220,220,0.2)",
            strokeColor: "rgba(220,220,220,1)",
            pointColor: "rgba(220,220,220,1)",
            pointStrokeColor: "#fff",
            pointHighlightFill: "#fff",
            pointHighlightStroke: "rgba(220,220,220,1)",
            data: [20, 30, 80, 20, 40, 10, 60,86,93,57,24,65]
        }, {
            fillColor: "rgba(151,187,205,0.2)",
            strokeColor: "rgba(151,187,205,1)",
            pointColor: "rgba(151,187,205,1)",
            pointStrokeColor: "#fff",
            pointHighlightFill: "#fff",
            pointHighlightStroke: "rgba(151,187,205,1)",
            data: [60, 10, 40, 30, 80, 30, 20,54,76,97,54,90]
        }]

    }

    var ctx = document.getElementById("line").getContext("2d");
    var LineChartDemo = new Chart(ctx).Line(lineChartData, {
        pointDotRadius: 10,
        bezierCurve: false,
        scaleShowVerticalLines: false,
        scaleGridLineColor: "black"
    });

//---------------------- Radar Sample -----------------------
    var radar_data = {
    labels: ["Milking", "Walking", "Running", "LyingDown", "Eating", "Sleeping", "Standing"],
    datasets: [
        {
            label: "My First dataset",
            fillColor: "rgba(220,220,220,0.2)",
            strokeColor: "rgba(220,220,220,1)",
            pointColor: "rgba(220,220,220,1)",
            pointStrokeColor: "#fff",
            pointHighlightFill: "#fff",
            pointHighlightStroke: "rgba(220,220,220,1)",
            data: [65, 59, 90, 81, 56, 55, 40]
        },
        {
            label: "My Second dataset",
            fillColor: "rgba(151,187,205,0.2)",
            strokeColor: "rgba(151,187,205,1)",
            pointColor: "rgba(151,187,205,1)",
            pointStrokeColor: "#fff",
            pointHighlightFill: "#fff",
            pointHighlightStroke: "rgba(151,187,205,1)",
            data: [28, 48, 40, 19, 96, 27, 100]
        }
    ]
};  

    var ctx1 = document.getElementById("radar").getContext("2d");
    var myRadarChart = new Chart(ctx1).Radar(radar_data, {
        scaleShowLine : true,

        angleShowLineOut : true,

        //Boolean - Whether to show labels on the scale
        scaleShowLabels : false,

        // Boolean - Whether the scale should begin at zero
        scaleBeginAtZero : true,

        //String - Colour of the angle line
        angleLineColor : "rgba(0,0,0,.1)",

        //Number - Pixel width of the angle line
        angleLineWidth : 1,

        //String - Point label font declaration
        pointLabelFontFamily : "'Arial'",

    });
//------------------------- Donut Sample -----------------------
var donut_data = [
    {
        value: 300,
        color:"#F7464A",
        highlight: "#FF5A5E",
        label: "Red"
    },
    {
        value: 50,
        color: "#46BFBD",
        highlight: "#5AD3D1",
        label: "Green"
    },
    {
        value: 100,
        color: "#FDB45C",
        highlight: "#FFC870",
        label: "Yellow"
    }
]
var ctx2 = document.getElementById("donut").getContext("2d");
var myDoughnutChart = new Chart(ctx2).Doughnut(donut_data,{
    //Boolean - Whether we should show a stroke on each segment
    segmentShowStroke : true,

    //String - The colour of each segment stroke
    segmentStrokeColor : "#fff",

    //Number - The width of each segment stroke
    segmentStrokeWidth : 2,

    //Number - The percentage of the chart that we cut out of the middle
    percentageInnerCutout : 50, // This is 0 for Pie charts

    //Number - Amount of animation steps
    animationSteps : 100,

    //String - Animation easing effect
    animationEasing : "easeOutBounce",

    //Boolean - Whether we animate the rotation of the Doughnut
    animateRotate : true,

    //Boolean - Whether we animate scaling the Doughnut from the centre
    animateScale : false,

    //String - A legend template
    legendTemplate : "<ul class=\"<%=name.toLowerCase()%>-legend\"><% for (var i=0; i<segments.length; i++){%><li><span style=\"background-color:<%=segments[i].fillColor%>\"></span><%if(segments[i].label){%><%=segments[i].label%><%}%></li><%}%></ul>"

});

});