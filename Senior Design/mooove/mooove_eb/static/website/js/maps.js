$(document).ready(function() {


//------------------- Initial Ajax Call -----------------------
    $.ajax({
    type:'GET',
    url:'/json/cows/'+ cowid, //{{cowid}}
    success: function(response){
        var Cowdata = response;
        var newLatLng = {lat:0, lng:0};
        
        var location_size = Cowdata.cow_location.length;     
        newLatLng.lat = Cowdata.cow_location[location_size-1].latitude;
        newLatLng.lng = Cowdata.cow_location[location_size-1].longitude;
        //console.log(newLatLng);
        var map = new google.maps.Map(document.getElementById('map'), {
        center: newLatLng,
        zoom: 16
        });
        var marker = new google.maps.Marker({
        position: newLatLng,
        map: map,
        icon: cowIcon
        });
    } //success function end

    }); //ajax end

    function ajax_call(){

        $.ajax({
        type:'GET',
        url:'/json/cows/'+ cowid, //{{cowid}}
        success: function(response){
            var newLatLng = {lat:0, lng:0};
            var Cowdata = response;
            var location_size = Cowdata.cow_location.length;
            newLatLng.lat = Cowdata.cow_location[location_size-1].latitude;
            newLatLng.lng = Cowdata.cow_location[location_size-1].longitude;
            var map = new google.maps.Map(document.getElementById('map'), {
            center: newLatLng,
            zoom: 18
            });
            var marker = new google.maps.Marker({
            position: newLatLng,
            map: map,
            icon: cowIcon
            });
        //console.log("location update!")
        } //success function end
//----------------- Map Update -----------------------------

        }); //ajax end
    } //ajax_call() end

    setInterval(function(){ ajax_call(); }, 10000); //calls every 1 mins


}); //document ready end 