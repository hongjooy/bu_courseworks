$(document).ready(function() {

    console.log("DEBUG: friends.js responsfsdfsdfsed!");

    var friends_id = [];
    var friends_name = [];
    var friends_number = [];
    var friends_rank = [];
    var friends_score = [];
    //var how_many_friends = friends.length;
    for (var i=0; i<3; i++){
        friends_rank[i] = friends[i].friendship_rank
        friends_number[i] = friends[i].friend_number
        friends_id[i] = friends[i].friend_id
        friends_name[i] = friends[i].friend_name
        friends_score[i] = friends[i].friendship_score
    }

    var friend_data = {
        labels: friends_name,
        datasets: [
        {
            label: "Average Health Data",
            fillColor: "rgba(255,0,0,0.4)",
            strokeColor: "rgba(255,0,0,0.4)",
            pointColor: "rgba(255,0,0,0.4)",
            pointStrokeColor: "#fff",
            pointHighlightFill: "#fff",
            pointHighlightStroke: "rgba(220,220,220,1)",
            data: friends_score
        }]
    }

    
    var friendship  = document.getElementById("friendship").getContext("2d");
    var friendshipChart = new Chart(friendship).Radar(friend_data,{
        pointLabelFontFamily : "'Helvetica Neue', 'Helvetica', 'Arial', sans-serif",
        pointLabelFontStyle : "normal",
        pointLabelFontSize : 15,
        pointLabelFontColor : "#666",
        scaleLineWidth: 2,
        responsive: true,
    });




  

});