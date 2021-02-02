var switch0 = document.getElementById('switch0');
var switch1 = document.getElementById('switch1');

var buttononoff = document.getElementById("buttononoff");

// Request value rgbw
var xhttp0 = new XMLHttpRequest();
xhttp0.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var jsonArray = JSON.parse(this.responseText);
        // var jsonArray = '[{"name": "", "r": 0, "g": 0, "b": 0, "w": 0, "isTransitioning": 0},...]';
        for (var i = 0; i < jsonArray.length; i++) {
            var jsonObject = jsonArray[i];
            var vname = jsonObject.name;
            var vr = jsonObject.r;
            var vg = jsonObject.g;
            var vb = jsonObject.b;
            var vw = jsonObject.w;
            //var visTransitioning = jsonObject.isTransitioning;

            // Clear border
            for (var j = 0; j < 6; j++) {
                document.getElementById("b" + i + j).className = "notselected";
            }

            // Set border
            if (vr == 255 && vg == 0 && vb == 0 && vw <= 100) {
                document.getElementById("b" + i + "0").className = "selected";
            } else if (vr == 0 && vg == 255 && vb == 0 && vw <= 100) {
                document.getElementById("b" + i + "1").className = "selected";
            } else if (vr == 0 && vg == 0 && vb == 255 && vw <= 100) {
                document.getElementById("b" + i + "2").className = "selected";
            } else if (vr <= 100 && vg <= 100 && vb <= 100 && vw == 255) {
                document.getElementById("b" + i + "3").className = "selected";
            } else if (vr == 0 && vg == 0 && vb == 0 && vw == 0) {
                document.getElementById("b" + i + "4").className = "selected";
            } else {
                document.getElementById("b" + i + "5").className = "selected";
                document.getElementById("b" + i + "5").style.background = 'rgb(' + (vr > vw ? vr : vw) + ',' + (vg > vw ? vg : vw) + ',' + (vb > vw ? vb : vw) + ')';
            }
        }
    }
};
xhttp0.open('GET', '/sendrgbw', true);
xhttp0.send();

// Request value A
var xhttp1 = new XMLHttpRequest();
xhttp1.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var jsonArray = JSON.parse(this.responseText);
		// var jsonArray = '[{"name": "", "w": 0},...]';
		// for (var i = 0; i < jsonArray.length; i++) {
        //     var jsonObject = jsonArray[i];
        //     var vname = jsonObject.name;
		// 	var vw = jsonObject.w;

		var jsonObject = jsonArray[0];
        var vname = jsonObject.name;
		var vw = jsonObject.w;

        console.log("A: " + vw);

        switch0.checked = (vw !== 0 ? true : false);
    }
};
xhttp1.open('GET', '/sendw', true);
xhttp1.send();
// Request value B
var xhttp2 = new XMLHttpRequest();
xhttp2.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var value = parseInt(this.responseText);

        console.log("B: " + value);

        value = (value === 1 ? true : false)

        switch1.disabled = false;
        switch1.checked = value;
    }
    else if (this.readyState == 4) {
        console.log("B: offline");
    }
};
xhttp2.open('GET', 'http://192.168.178.61/state', true);
xhttp2.send();

// Request value onoff
var xhttp5 = new XMLHttpRequest();
xhttp5.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        buttononoff.value = this.responseText;
    }
};
xhttp5.open('GET', '/onoff', true);
xhttp5.send();


// Switch events
switch0.addEventListener('click', function () {
    SendW(0, (switch0.checked ? 255 : 0));
});
switch1.addEventListener('click', function () {
    SendW(1, (switch1.checked ? 255 : 0));
});



function SendRGBW(id, valR, valG, valB, valW) {
	id++;
	valR++;
	valG++;
	valB++;
	valW++;
    console.log('Send: /sendrgbw?id=' + id + '&r=' + valR + '&g=' + valG + '&b=' + valB + '&w=' + valW);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var jsonArray = JSON.parse(this.responseText);
            // var jsonArray = '[{"name": "", "r": 0, "g": 0, "b": 0, "w": 0, "isTransitioning": 0},...]';
            for (var i = 0; i < jsonArray.length; i++) {
				var jsonObject = jsonArray[i];
				var vname = jsonObject.name;
				var vr = jsonObject.r;
				var vg = jsonObject.g;
				var vb = jsonObject.b;
				var vw = jsonObject.w;
				//var visTransitioning = jsonObject.isTransitioning;

                // Clear border
                for (var j = 0; j < 6; j++) {
                    document.getElementById("b" + i + j).className = "notselected";
                }

                // Set border
                if (vr == 255 && vg == 0 && vb == 0 && vw <= 100) {
                    document.getElementById("b" + i + "0").className = "selected";
                } else if (vr == 0 && vg == 255 && vb == 0 && vw <= 100) {
                    document.getElementById("b" + i + "1").className = "selected";
                } else if (vr == 0 && vg == 0 && vb == 255 && vw <= 100) {
                    document.getElementById("b" + i + "2").className = "selected";
                } else if (vr <= 100 && vg <= 100 && vb <= 100 && vw == 255) {
                    document.getElementById("b" + i + "3").className = "selected";
                } else if (vr == 0 && vg == 0 && vb == 0 && vw == 0) {
                    document.getElementById("b" + i + "4").className = "selected";
                } else {
                    document.getElementById("b" + i + "5").className = "selected";
                    document.getElementById("b" + i + "5").style.background = 'rgb(' + (vr > vw ? vr : vw) + ',' + (vg > vw ? vg : vw) + ',' + (vb > vw ? vb : vw) + ')';
                }
            }
        }
    };
    xhttp.open('GET', '/sendrgbw?id=' + id + '&r=' + valR + '&g=' + valG + '&b=' + valB + '&w=' + valW, true);
    xhttp.send();
}

function SendW(id, valW) {
    if (id === 0) {
        var tid = id + 1;
        var tvalW = valW + 1;
        console.log('Send: /sendw?id=' + tid + '&w=' + tvalW);
        var xhttp = new XMLHttpRequest();
        xhttp.open('GET', '/sendw?id=' + tid + '&w=' + tvalW, true);
        xhttp.send();
    } else if (id === 1) {
        console.log('Send: http://192.168.178.61/' + (valW ? 'on' : 'off'));
        var xhttp = new XMLHttpRequest();
        xhttp.open('GET', 'http://192.168.178.61/' + (valW ? 'on' : 'off'), true);
        xhttp.send();
    }
}

function SendOnOff() {
    console.log('Send: /onoff');
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            buttononoff.value = this.responseText;
        }
    };
    xhttp.open('GET', '/onoff?s=1', true);
    xhttp.send();
}