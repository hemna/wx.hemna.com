/**
 * This file manages updating and showing the ticker
 * above the banner image.
 */


var ticker_data;
var ci=0;
var ri=0;

function start_ticker() {

    get_data();
    setInterval(get_data, 60000);
    setInterval(update_ticker, 5000);
}

function update_ticker() {
    if (ticker_data != undefined) {
        //console.log("update ticker "+ci);
        //console.log(ticker_data.middle);
       clength = ticker_data.middle.length;
       rlength = ticker_data.right.length;
		   
       if (ci >= clength-1) {
	       ci = 0;
       }

       $('#headtopcenter').fadeOut(500,
           function() {
	           $("#headtopcenter").html(ticker_data.middle[ci]);
               $('#headtopcenter').fadeIn(500);
           });
		   
       if (ri >= rlength-1) {
	       ri = 0;
       }	

       $('#headtopright').fadeOut(500,
           function() {
	           $("#headtopright").html(ticker_data.right[ri]);
               $('#headtopright').fadeIn(500);
           });
       ci++;
       ri++;
    }
}

function get_data() {
	try {
	    var img = new Image();
	    img.src="/css/prototip_loader.gif";
	
	    //$("headtopcenter").innerHTML = img;
	    $('#headtopcenter').html(img);
	    $('#headtopright').html(img);
	} catch(err) {
		console.log(err.message);
	}

    $.ajax({
        url: "/?target=ticker",
        dataType: 'json',
        success: function(data) {
            ticker_data = data;
        }
    });
}
