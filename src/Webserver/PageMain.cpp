#include <avr/pgmspace.h>

// 16384 character limit
const char* pageMain_html PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>Led strip</title>
  <link rel='icon shortcut' type='image/png' href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAZdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjAuMjHxIGmVAAACyElEQVQ4T32R60vTURjHBxb0T0SQkNTmrUzdvKZZWo3Ssla6Iu1monazIlFLX1iaRnaPmtosMcoudDMlSGPSC52KaZYVbpk6NTdzTn3z6beDCpL2hQcO5/D9PM/zPbK5lFq7mfSGWHp7+xgeHubs1ziyO+OZep5fWt1a98Mv1ZjNZgYHhxix2bDbxxgfH2dycpLcH9r5IbGXQ0isjCTlTbTUuZffv4clsx3HlNlZhaaTnOuKJ6Nl578gzfU1JFRE0tHRQU/PL4akCZyAiYmJGYCzsj/Hkdm+azagoaGB7cUhaHURtLV9wmz6iaW/H5vVhsPhmAXJaNFwplkj7qfsMtmH+npiCoLRXFtDU2MT3d3dYg2LxYLVamV0dHQGdMIQKwJ2noW5XK/n7dsaNuX4S5AgYi8H09Rk5FtXFyaTSUAGBwawSYEmv1KTUq0m7V20CLZPaiIrLSnlSVUVEemriMr0Q52rYst5JQaDAaPRKDJxwrQlESKj/Y+iOPhkA2NjY/RLawqAKtGdwAMKgpM9CU31IuyoN+HHV7LulA9RGb5sPOvP5rwAtl0KFWvG3QlnT9l68d0SoARlogK/3XL89shR7lWgTFAQIN0F7pdqn4IgJzzJg/Bj3kRKQHWukpiLQVJejcgCDnig2ueBn1bOao0cnx1uuKtdeV2ag6HqIq3VxbTXXsU33g1VglyAgw95EnbESwBFkAFSR++ty1jiu5grWcm80GVTmHWS74bbDLTeoyj7NP3GUr68v4FXjCv+2hViMmGe1opIVzKT4qirzMP8UUegQo7911PGh16iki9ncqSGCWu19HYXedTS2eZpFWclUV2WS+ubYhYtWMCfzgpGfjxkoYsLjoHn2H8+xtJ8b27ztB4UneLZrSzeV+QJUJc0do/Uta9Rx/e6m/83T0ufn0J5fhr6/FTKC9K4X5CK/kLKHGaZ7C/+EiOzvseUTgAAAABJRU5ErkJggg=="/>
  <meta name="theme-color" content="#4286f4">

  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <link href='http://fonts.googleapis.com/css?family=Roboto:400,700' rel='stylesheet' type='text/css'>
  <!--<link href='http://fonts.googleapis.com/css?family=Roboto:400,100,100italic,300,300italic,400italic,500,500italic,700,700italic,900italic,900' rel='stylesheet' type='text/css'>-->
  <link rel="stylesheet" href="PageMain.css">
  <!--
	  Author: Daniël van der Drift

	  Usage:
	  /sendrgbw?id=(1/2)&r=(1-256)&g=(1-256)&b=(1-256)&w=(1-256)
	  /sendw?id=(1)&w=(0/1)
	  /sendanim?anim=(1-5)
	  /onoff
  -->
</head>

<body>
  <h1>Ledstrip controller</h1>
  <center>
    <div id="wrap">
      <div id="item">
        <p>Ledstrip1</p>
        <table id="t1" class="table1">
          <tr>
            <th>
              <input id="b00" class='notselected' type='button' onclick='SendRGBW(0,255,0,0,0)' style='background: #f00;' value="Red">
            </th>
            <th>
              <input id="b01" class='notselected' type='button' onclick='SendRGBW(0,0,255,0,0)' style='background: #0f0;' value="Green">
            </th>
            <th>
              <input id="b02" class='notselected' type='button' onclick='SendRGBW(0,0,0,255,0)' style='background: #00f;' value="Blue">
            </th>
          </tr>
          <tr>
            <td>
              <input id="b03" class='notselected' type='button' onclick='SendRGBW(0,0,0,0,255)' value="White">
            </td>
            <td>
              <input id="b04" class='notselected' type='button' onclick='SendRGBW(0,0,0,0,0)' value="Black">
            </td>
            <td>
              <input id="b05" class='notselected' type='button' onclick='location.href="/colorpicker?id=1"' style='font-size: 12px;' value="Pick color">
            </td>
          </tr>
        </table>
      </div>
      <div id="item">
        <p>Ledstrip2</p>
        <table id="t2" class="table1">
          <tr>
            <th>
              <input id="b10" class='notselected' type='button' onclick='SendRGBW(1,255,0,0,0)' style='background: #f00;' value="Red">
            </th>
            <th>
              <input id="b11" class='notselected' type='button' onclick='SendRGBW(1,0,255,0,0)' style='background: #0f0;' value="Green">
            </th>
            <th>
              <input id="b12" class='notselected' type='button' onclick='SendRGBW(1,0,0,255,0)' style='background: #00f;' value="Blue">
            </th>
          </tr>
          <tr>
            <td>
              <input id="b13" class='notselected' type='button' onclick='SendRGBW(1,0,0,0,255)' value="White">
            </td>
            <td>
              <input id="b14" class='notselected' type='button' onclick='SendRGBW(1,0,0,0,0)' value="Black">
            </td>
            <td>
              <input id="b15" class='notselected' type='button' onclick='location.href="/colorpicker?id=2"' style='font-size: 12px;' value="Pick color">
            </td>
          </tr>
        </table>
      </div>
    </div>
    <br>
  
    <table id="t3" class="table2">
      <tr>
        <th>
          <p>Schuineluik</p>
          <label class="switch">
            <input id="switch0" type="checkbox">
            <span class="slider"></span>
          </label>
        </th>
        <th>
          <p>Lamp2</p>
          <label class="switch">
            <input id="switch1" type="checkbox" disabled>
            <span class="slider"></span>
          </label>
        </th>
      </tr>
    </table>
    <br>
  
    <input id='buttononoff' type='button' onclick='SendOnOff()' value="Off">
  </center>
  <script src="PageMain.js"></script>
</body>
</html>
)=====";

const char* pageMain_css PROGMEM = R"=====(
p,
h1,
input {
    font-family: 'Roboto', Times, serif;
}

.table1 {
    width: 270px;
}

.table2 {
    width: 90%;
}

table.table2 th,
table.table2 td {
    padding: 8px;

}

table.table1 th,
table.table1 td {
    padding: 0px;
    width: 10%;
}

table.table1 input {
    width: 100%;
    height: 50px;
}

table.table2 th {
    font-weight: normal;
}

#wrap {
    width: 100%;
}

#item {
    display: inline-block;
    /*background-color: gray;*/
}


input#buttononoff {
    width: 70%;
    height: 50px
}

input.selected,
input.notselected,
input#buttononoff {
    border: none;
    border-radius: 8px;
    color: black;
    text-align: center;
    font-size: 16px;
    opacity: 1;
    transition-duration: 0.4s;
}

input.selected:hover,
input.notselected:hover,
input#buttononoff:hover,
.range:hover {
    opacity: 0.7;
}

input.selected {
    border: 2px solid black;
}



.switch {
    position: relative;
    display: inline-block;
    width: 80px;
    height: 30px;
}

.switch input {
    display: none;
}

.slider {
    position: absolute;
    cursor: pointer;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    border-radius: 3px;
    background-color: #dcc;
    -webkit-transition: .3s;
    transition: .3s;
}

.slider:before {
    position: absolute;
    content: "";
    height: 30px;
    width: 30px;
    left: 0px;
    bottom: 0px;
    border-radius: 3px;
    background-color: #888;
    -webkit-transition: .3s;
    transition: .3s;
}

input:checked+.slider {
    background-color: #efe;
}

input:checked+.slider:before {
    background-color: #ccc;
    -webkit-transform: translateX(44px);
    -ms-transform: translateX(50px);
    transform: translateX(50px);
}
)=====";

const char* pageMain_js PROGMEM = R"=====(
var switch0 = document.getElementById('switch0');
var switch1 = document.getElementById('switch1');


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
        document.getElementById("buttononoff").value = this.responseText;
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
            document.getElementById("buttononoff").value = this.responseText;
        }
    };
    xhttp.open('GET', '/onoff?s=1', true);
    xhttp.send();
}
)=====";