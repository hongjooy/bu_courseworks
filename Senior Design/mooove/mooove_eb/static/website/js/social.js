$(document).ready(function() {

  /*-------------  memo ------------------------------------      
    sociallevel of the cow --> how big the circle is 
    cowid(pk) --> the node id
    friend's name --> label of each node
    friendship score --> how think the link between two cows
  ----------------------------------------------------------*/
var ajax1 = $.ajax({ 
  type:'GET',
  url:'/json/friends', //{{cowid}}
  async: true,
  success: function(result) {}                     
});


var ajax2 = $.ajax({ 
  type:'GET',
  url:'/json/socials', 
  async: true,
  success: function(result) {}  
});

$.when( ajax1 , ajax2  ).done(function( a1, a2 ) {

  var FriendsList = a1[2].responseJSON; // has friends relationships
  var SocialLevelList = a2[2].responseJSON; // has cow number and each cow's social level
  var size_friendlist = FriendsList.length;
  var size_sociallevellist = SocialLevelList.length;

  var cow_nodes = [];
  for (var i = 0; i < size_sociallevellist; i++){
    cow_nodes.push({id:SocialLevelList[i].idcow.idcow, value: SocialLevelList[i].sociallevel, label: SocialLevelList[i].idcow.cowname})
  }
  console.log("cownodes:",cow_nodes);
  var cow_links = [];
  var already_list = [];
  for (var j = 0; j < size_friendlist; j++){
      already_list.push(FriendsList[j].idcoworiginal.idcow);

      if (FriendsList[j].idcowfriend.idcow in already_list){
          
      }
      else{
        cow_links.push({from:FriendsList[j].idcoworiginal.idcow, to:FriendsList[j].idcowfriend.idcow, value:FriendsList[j].friendshipscore, title: 'score'});
      }
  }

  var container = document.getElementById('mynetwork');
  var data = {
    nodes: cow_nodes,
    edges: cow_links
  };

  var options = {
    nodes: {
      shape: 'dot',
    }
  }

  network = new vis.Network(container, data, options);


});


      
}); //



 






