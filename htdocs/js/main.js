
var orig_time = new Date();
var orig_ts = Math.round(orig_time.getTime()/1000);

function main_init() {
    $("#idMetar").bind("mouseover", metar_show);
    $("#idMetar").bind("mouseout", metar_hide);
    metar_show();
}

function metar_show() {
	try {
        $.ajax({
            url: "/?target=rmetar&junk="+get_junk(),
            dataType: 'json',
            success: function(data) {
                var metar = data;
			    try {
				   var str = metar.metar + " " + metar.season + " " + metar.tod + " " + metar.wx + " ("+ metar.banner +")";
                   if (metar.metar) {
				       $("#idMetar").html(str);
                   } else {
                       console.log("inValid metar data");
                       console.log(metar);
                   }
				 } catch(err) {
					 //console.log(err.message);
				 }
           }
        })
	} catch(err) {
		//console.log("err: "+err.message);
	}
}

function metar_hide() {
	try {
		$("#idMetar").html("");
	} catch(err) {
		//console.log("err: "+err.message);
	}
}

function update_cctime() {
	var time = new Date();
	var new_ts = Math.round( (time.getTime())/1000);
	var conds = new Array();
	conds['ageh'] = time.getHours() - orig_time.getHours();
	conds['agem'] = time.getMinutes() - orig_time.getMinutes();
	conds['ages'] = time.getSeconds()  - orig_time.getSeconds();
	$('#idUpdateSeconds').html(convertTime(new_ts - orig_ts)); 
}

function update_imgs() {
    $('#idRadarImg').fadeOut(400,
            function() {
                $('#idRadarImg').attr('src',
                    "https://radblast.wunderground.com/cgi-bin/radar/WUNIDS_map?station=FCX&brand=wui&num=6&delay=15&type=N0R&frame=0&scale=0.5&noclutter=0&showstorms=0&mapx=400&mapy=240&centerx=170&centery=316&transx=-230&transy=76&showlabels=1&severe=0&rainsnow=0&lightning=1&smooth=0&rand=25471752&lat=37.34&lon=-78.83&label=Appomattox&junk="+get_junk());

                //img = new Image();
                //img.src = "http://radblast-mi.wunderground.com/cgi-bin/radar/WUNIDS_map?station=DAX&brand=wui&num=1&delay=15&type=N0R&frame=0&scale=0.326&noclutter=0&t=1235942643&lat=38.81977081&lon=-121.02596283&label=Cool%2C+CA&showstorms=0&map.x=400&map.y=240&centerx=145&centery=359&transx=-255&transy=119&showlabels=1&severe=0&rainsnow=0&lightning=0&smooth=0&junk="+junk;
                //img.id='idRadarImg';
                //img.width=480;
                //img.height=320;
                //$('#idRadardiv').update();
                //$('#idRadardiv').appendChild(img);      
                $('#idRadarImg').fadeIn(200);
            }
    );

    $('#idSatImg').fadeOut(400,
            function() {
                $('#idSatImg').attr('src',
                    "http://images.intellicast.com/WxImages/SatelliteLoop/hiatlsat_None_anim.gif?junk="+get_junk());
                $('#idSatImg').fadeIn(200);
            }
    );
}

function update_webcam_img() {
    $('#idWebCamImg').attr('src', "http://wx.hemna.com/cam/cam.jpg?junk="+get_junk());
}

function start_updates() {
		
	set_orig_time(new Date());
    setInterval(update_cctime, 2000);
    setInterval(update_imgs, 65000);
    setInterval(update_webcam_img, 15000);

    (function updatecc() {
        $.ajax({
            url: '/?ajax=1&ajaxid=idCurrentConditions&target=current-conditions&junk='+get_junk(),
            success: function(data) {
                $('#idCurrentConditions').fadeOut(200, 
                    function() {
                        $('#idCurrentConditions').html(data);
                        $('#idCurrentConditions').fadeIn(200);
                        var time = new Date();
                        var new_ts = Math.round((time.getTime())/1000);
                        set_orig_time_ts(new_ts);
                    }
                  );
            },
            complete: function() {
                setTimeout(updatecc, 65000);
            }
        });
    })();

    (function updatewebcam() {
        $.ajax({
            url: '/?ajax=1&ajaxid=idWebCam&target=web-cam&junk='+get_junk(),
            success: function(data) {
                $('#idWebCam').fadeOut(400,
                    function() {
                        $('#idWebCam').html(data);
                        $('#idWebCam').fadeIn(200);
                    }
                );
            },
            complete: function() {
                setTimeout(updatewebcam, 300000);
            }
        });
    })();

    (function updatetimelapse() {
        $.ajax({
            url: '/?ajax=1&ajaxid=idTimeLapse&target=time-lapse&junk='+get_junk(),
            success: function(data) {
                $('#idTimeLapse').fadeOut(400,
                    function() {
                        $('#idTimeLapse').html(data);
                        $('#idTimeLapse').fadeIn(200);
                    }
                );
            },
            complete: function() {
                setTimeout(updatetimelapse, 300000);
            }
        });
    })();
}



function get_junk() {
  var d = new Date();  
  var junk=d.getMonth()+1+''+d.getDay()+''+d.getHours()+''+d.getMinutes()+''+d.getSeconds();
  return junk;
}

function set_orig_time(new_time) {
  orig_time = new_time;
  orig_ts = Math.round(orig_time.getTime()/1000);
}

function set_orig_time_ts(ts) {
  orig_ts = ts;
}

function convertTime(ts) {
	var ageh = Math.floor(ts / (60*60));
	var agem = Math.floor(ts / 60);
	var ages = ts;
	
	if (ageh > 0) ages = ts - (ageh * 60 * 60);
	if (agem > 0) ages = ages  - (agem * 60);

	newHTML = "";
	if (ageh>0)
	{
		newHTML = ageh+" hr "+agem+" min "+ages+" sec ago";
	}
	else if (agem>0)
	{
		newHTML = agem+" min "+ages+" sec ago";
	}
	else
	{
		newHTML = ages+" sec ago";
	}

	return newHTML;
}
