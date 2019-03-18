function start_hc() {

    (function tempdewpworker() {
        $.ajax({
            url: "/?target=temp-dew-point-high-charts-graph&dataOnly=1",
            type: 'post',
            dataType: 'json',
            success: function(data) {
                tempdewpointoptions.series = data;
                tempdewpoint = new Highcharts.Chart(tempdewpointoptions);
            },
            complete: function() {
                setTimeout(tempdewpworker, 60000);
            }
        });
    })();

    (function insidetempworker() {
        $.ajax({
            url: "/?target=inside-temp-high-charts-graph&dataOnly=1",
            type: 'post',
            dataType: 'json',
            success: function(data) {
                itempoptions.series.pop();
                itempoptions.series.push(data);
                insidetemp = new Highcharts.Chart(itempoptions);
            },
            complete: function() {
                setTimeout(insidetempworker, 60000);
            }
        });
    })();

    (function pressureworker() {
        $.ajax({
            url: "/?target=pressure-high-charts-graph&dataOnly=1",
            dataType: 'json',
            success: function(data) {
                pressureoptions.series = data;
                pressuregraph = new Highcharts.Chart(pressureoptions);
            },
            complete: function() {
                setTimeout(pressureworker, 60000);
            }
        });
    })();

    (function windlineworker() {
        $.ajax({
            url: "/?target=wind-speed-high-charts-graph&dataOnly=1",
            dataType: 'json',
            success: function(data) {
                windlineoptions.series = data;
                windlinegraph = new Highcharts.Chart(windlineoptions);
            },
            complete: function() {
                setTimeout(windlineworker, 60000);
            }
        });
    })();

    (function rainlineworker() {
        $.ajax({
            url: "/?target=rain-high-charts-graph&dataOnly=1",
            dataType: 'json',
            success: function(data) {
                rainlineoptions.series.pop();
                rainlineoptions.series.push(data);
                rainlinegraph = new Highcharts.Chart(rainlineoptions);
            },
            complete: function() {
                setTimeout(rainlineworker, 60000);
            }
        });
    })();

    (function winddirworker() {
        $.ajax({
            url: "/?target=wind-direction-high-charts-gauge&dataOnly=1",
            type: 'post',
            dataType: 'json',
            success: function(data) {
                winddiroptions.series = data;
                winddirgraph = new Highcharts.Chart(winddiroptions);
            },
            complete: function() {
                setTimeout(winddirworker, 60000);
            }
        });
    })();
    (function windspeedworker() {
        $.ajax({
            url: "/?target=wind-speed-high-charts-gauge&dataOnly=1",
            type: 'post',
            dataType: 'json',
            success: function(data) {
                windspeedoptions.series = data;
                windspeedgraph = new Highcharts.Chart(windspeedoptions);
            },
            complete: function() {
                setTimeout(windspeedworker, 60000);
            }
        });
    })();


}

// This is for all plots, change Date axis to local timezone
Highcharts.setOptions({  
    global : {
        useUTC : false
    }
});
