function rhymeSort(res){
	//array(word, rythmPower (0-4), releatedness (0-100))
	var titleArray = ["Weak", "Medium", "Strong", "Fantastic"];
	res.sort(function(a, b){
		return ((a[1] > b[1]) ? -1 : ((a[1] < b[1]) ? 1 : (a[0]>b[0])));
	});
					
	var trackerNumber = -1;
	output = "";
	for(h = 0; h<res.length; ++h)
	{
		if(res[h][1] != trackerNumber || trackerNumber == -1){
			trackerNumber = res[h][1];
			output += "<br><i><b>" + titleArray[trackerNumber-1] + " rhymes</i></b>\n";
		}
		
		var floatredvalue = parseInt(res[h][2], 10) *253/100;
		var intredvalue = Math.floor(floatredvalue);

		redhexString = intredvalue.toString(16);
			
		output += "<a class='word' style='color:#"+redhexString+"0000'>" + res[h][0] + "</a>\n";
	}
	return output;
}
function releatedSort(res){
	res.sort(function(a, b){
	  return ((a[2] > b[2]) ? -1 : ((a[2] < b[2]) ? 1 : (a[0]>b[0])));
	});
	
	output = "<br><i><b>Relatedness</i></b>\n";
	for(h = 0; h<res.length; ++h)
	{	
		var floatredvalue = parseInt(res[h][2], 10) *253/100;
		var intredvalue = Math.floor(floatredvalue);

		redhexString = intredvalue.toString(16);
			
		output += "<a class='word' style='color:#"+redhexString+"0000'>" + res[h][0] + "</a>\n";
	}
	return output;
}
function outputResults(res){
	if(sortedByRhyme){ //sort by rhythm strength
		output = rhymeSort(res);
	}else{ //sort by relatedness
		output = releatedSort(res);
	}		
	$("#resultwrapper").html(output);
	$("#resultwrapper").fadeTo(200, 1);
	$('#loading').css("visibility", "hidden");
	$('#errorwrapper').css("display", "none");
}

var sortedByRhyme = true;
var res = false;

$(function() {
	
	
	$('#toggleButton').switchButton().change(function(){
		sortedByRhyme = !sortedByRhyme;
		if(res)
			outputResults(res);
	});
	
	var emptyWordString = 'Word that rhymes with...';
	var emptyLexicalString = 'And related to...';
	
	$('#search_form').on('submit', function(e) {
		e.preventDefault();
		
		$("#resultwrapper").fadeTo(200, 0); //$("#resultwrapper").html("");
		$("errorwrapper").fadeTo(200, 0);
		
		var data = $("#search_form :input").serializeArray();
		
		if(data[0].value == emptyWordString)
			return;
		
		if(data[1].value == emptyLexicalString)
			data[1].value = "";
		
		$.ajax({
			beforeSend: function() {
				$('#loading').css("visibility", "visible");    /*showing  a div with spinning image */
			},
			type: "POST",
			url: "scripts/getData.php",
			data: data,
			dataType: "text"
			})
			.done(function(string) {						
				try {
					res = $.parseJSON(string);
				}
				catch(e) {//Not json, probably an error message
					$('#loading').css("visibility", "hidden");
					$('#errorwrapper').css("display", "block");
					$("#errorwrapper").html("\n" + string);
					$("#resultwrapper").html("");
					return;
				}
				/*console.log(string);
				console.log(res);*/
				outputResults(res);
		});
	});
	var queryTimer = 0;
	
	function inputFunc(){
		if(queryTimer != 0) 
			window.clearTimeout(queryTimer);
		
		queryTimer = window.setTimeout(function(){$('#search_form').submit();}, 500);
	}
	
	$('#wordInput').on("input", inputFunc);
	$('#lexhintInput').on("input", inputFunc);
	
	$("input:text").focus(function(){
		$(this).css("color", "black");
		if (this.value ==  emptyWordString || this.value == emptyLexicalString) {
			this.value = '';
		}
	});
	
	$("#wordInput").blur(function(){
		if (this.value == '') {
			this.value = emptyWordString;
			$(this).css("color", "#aaa");
		}
	});
	$("#lexhintInput").blur(function(){
		if (this.value == '') {
			this.value = emptyLexicalString;
			$(this).css("color", "#aaa");
		}
	});
});