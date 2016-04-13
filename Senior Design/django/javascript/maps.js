
$(document).ready(function() {

//------------ Ajax Function -------------
    function ajax_call(){

        $.ajax({
        type:'GET',
        url:'/json/locations/'+ cowid, //{{cowid}}
        success: function(response){
            var location = response;
            console.log("location:",location);
            }
        });
    };
    setInterval(function(){ ajax_call(); }, 60000); // get data every minute

//------------- Google Map API ---------------
    //var cowIcon = '{% static "website/img/cowmarker.png" %}';
    var myLatLng = {lat: 42.3509605, lng: -71.1088587} //static
      var map = new google.maps.Map(document.getElementById('map'), {
        center: myLatLng,
        zoom: 18
      });
      var marker = new google.maps.Marker({
        position: myLatLng,
        map: map,
        icon: cowIcon
      })


});