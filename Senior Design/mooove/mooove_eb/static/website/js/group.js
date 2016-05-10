$(document).ready(function() {

	$.ajax({
    type:'GET',
    url:'/json/groups/', 
    success: function(response){
    	var Groups = response;
    	//console.log(Groups);
    	var group_size = Groups.length;
    	console.log("groupstuff!!: ", Groups[3].idcow.idcow);
    	console.log("groupstuff!!: ", Groups[3].idcow.cowname);
    	console.log("dhislf", Groups[3].cowgroupnum);


    	var nodes = [];
    	var exist_already=[];

    	for (var i=0; i < group_size; i++){
    		if (Groups[i].cowgroupnum in exist_already){

    		}
    		else{
    			exist_already.push(Groups[i].cowgroupnum);
    			nodes.push({id:Groups[i].cowgroupnum, label:Groups[i].cowgroupnum, group: Groups[i].cowgroupnum});
    		}
    		nodes.push({id:Groups[i].idcow.idcow, label:Groups[i].idcow.cowname, group:Groups[i].cowgroupnum });
    	}

    	var len = nodes.length;

    	var edges = [];
    	for (var i=0; i<group_size; i++){
    		edges.push({from:Groups[i].idcow.idcow, to:Groups[i].cowgroupnum});
    	}

    	            // create a network
            var container = document.getElementById('group');
            var data = {
                nodes: nodes,
                edges: edges
            };
            var options = {
                nodes: {
                    shape: 'dot',
                    size: 16
                },
                physics: {
                    forceAtlas2Based: {
                        gravitationalConstant: -26,
                        centralGravity: 0.005,
                        springLength: 230,
                        springConstant: 0.18
                    },
                    maxVelocity: 146,
                    solver: 'forceAtlas2Based',
                    timestep: 0.35,
                    stabilization: {iterations: 150}
                }
            };
            var network = new vis.Network(container, data, options);


    } //success done

    }); //ajax done

}); //