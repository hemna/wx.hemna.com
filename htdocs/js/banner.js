function get_junk() {
    var d = new Date();  
    var junk=d.getMonth()+1+''+d.getDay()+''+d.getHours()+''+d.getMinutes()+''+d.getSeconds();
   return junk;
}

function start_banner() {

    (function updatebanner() {
        $.ajax({
            url: "/?target=rmetar",
            dataType: 'json',
            success: function (data) {
                metar = data;
                if (metar.banner == "yes" && 
                    metar.filename != undefined &&
                    metar.filename != null) {

                    $("#pageheader").fadeOut(500, 
                        function() {
		                    imgsrc = metar.filename+"?junk="+get_junk();
                            $("#pageheader").css('background-image',
                                'url('+imgsrc+')');
                            $("#pageheader").fadeIn(500);
                        });
                }
            },
            complete: function() {
                setTimeout(updatebanner, 60*5*1000);
            }
        });
    })();

}
