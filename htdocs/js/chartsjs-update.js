function start_chartsjs() {

    (function tempdewpworker() {
        $.ajax({
            url: "/?target=temp-dew-point-charts-j-s-graph&dataOnly=1",
            type: 'post',
            dataType: 'json',
            success: function(data) {
		        tempdewchart.data.datasets[0].data = data[0]['data'];
		        tempdewchart.data.datasets[1].data = data[1]['data'];
		        tempdewchart.update();
            },
            complete: function() {
                setTimeout(tempdewpworker, 60000);
            }
        });
    })();

    (function insidetempworker() {
        $.ajax({
            url: "/?target=inside-temp-charts-j-s-graph&dataOnly=1",
            type: 'post',
            dataType: 'json',
            success: function(data) {
                insidechart.data.datasets[0].data = data['data'];
                insidechart.update();
            },
            complete: function() {
                setTimeout(insidetempworker, 60000);
            }
        });
    })();

    (function pressureworker() {
        $.ajax({
            url: "/?target=pressure-charts-j-s-graph&dataOnly=1",
            dataType: 'json',
            success: function(data) {
                pressurechart.data.datasets[0].data = data[0]['data'];
                pressurechart.data.datasets[1].data = data[1]['data'];
                pressurechart.update();
            },
            complete: function() {
                setTimeout(pressureworker, 60000);
            }
        });
    })();

    (function windlineworker() {
        $.ajax({
            url: "/?target=wind-speed-charts-j-s-graph&dataOnly=1",
            dataType: 'json',
            success: function(data) {
                windc.data.datasets[0].data = data[0]['data'];
                windc.data.datasets[1].data = data[1]['data'];
                windc.update();
            },
            complete: function() {
                setTimeout(windlineworker, 60000);
            }
        });
    })();

    (function rainlineworker() {
        $.ajax({
            url: "/?target=rain-charts-j-s-graph&dataOnly=1",
            dataType: 'json',
            success: function(data) {
                rainc.data.datasets[0].data = data['data'];
                rainc.update();
            },
            complete: function() {
                setTimeout(rainlineworker, 60000);
            }
        });
    })();

    (function humidworker() {
        $.ajax({
            url: "/?target=humidity-charts-j-s-graph&dataOnly=1",
            dataType: 'json',
            success: function(data) {
                humidchart.data.datasets[0].data = data['data'];
                humidchart.update();
            },
            complete: function() {
                setTimeout(humidworker, 60000);
            }
        });
    })();

}
