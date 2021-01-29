#include <avr/pgmspace.h>

const char* pageColorPicker_html PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>Led strip</title>
  <link rel='icon' type='image/png' href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAZdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjAuMjHxIGmVAAACyElEQVQ4T32R60vTURjHBxb0T0SQkNTmrUzdvKZZWo3Ssla6Iu1monazIlFLX1iaRnaPmtosMcoudDMlSGPSC52KaZYVbpk6NTdzTn3z6beDCpL2hQcO5/D9PM/zPbK5lFq7mfSGWHp7+xgeHubs1ziyO+OZep5fWt1a98Mv1ZjNZgYHhxix2bDbxxgfH2dycpLcH9r5IbGXQ0isjCTlTbTUuZffv4clsx3HlNlZhaaTnOuKJ6Nl578gzfU1JFRE0tHRQU/PL4akCZyAiYmJGYCzsj/Hkdm+azagoaGB7cUhaHURtLV9wmz6iaW/H5vVhsPhmAXJaNFwplkj7qfsMtmH+npiCoLRXFtDU2MT3d3dYg2LxYLVamV0dHQGdMIQKwJ2noW5XK/n7dsaNuX4S5AgYi8H09Rk5FtXFyaTSUAGBwawSYEmv1KTUq0m7V20CLZPaiIrLSnlSVUVEemriMr0Q52rYst5JQaDAaPRKDJxwrQlESKj/Y+iOPhkA2NjY/RLawqAKtGdwAMKgpM9CU31IuyoN+HHV7LulA9RGb5sPOvP5rwAtl0KFWvG3QlnT9l68d0SoARlogK/3XL89shR7lWgTFAQIN0F7pdqn4IgJzzJg/Bj3kRKQHWukpiLQVJejcgCDnig2ueBn1bOao0cnx1uuKtdeV2ag6HqIq3VxbTXXsU33g1VglyAgw95EnbESwBFkAFSR++ty1jiu5grWcm80GVTmHWS74bbDLTeoyj7NP3GUr68v4FXjCv+2hViMmGe1opIVzKT4qirzMP8UUegQo7911PGh16iki9ncqSGCWu19HYXedTS2eZpFWclUV2WS+ubYhYtWMCfzgpGfjxkoYsLjoHn2H8+xtJ8b27ztB4UneLZrSzeV+QJUJc0do/Uta9Rx/e6m/83T0ufn0J5fhr6/FTKC9K4X5CK/kLKHGaZ7C/+EiOzvseUTgAAAABJRU5ErkJggg=="/>
  <meta name="theme-color" content="#4286f4">
  
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <link href='http://fonts.googleapis.com/css?family=Roboto:400,700' rel='stylesheet' type='text/css'>
  <!--<link href='http://fonts.googleapis.com/css?family=Roboto:400,100,100italic,300,300italic,400italic,500,500italic,700,700italic,900italic,900' rel='stylesheet' type='text/css'>-->
  <link rel="stylesheet" href="PageColorPicker.css">
  <!--
	  Author: Daniël van der Drift
  -->
</head>

<body>
  <div id="bg"></div>
  <h1>Ledstrip controller</h1>
  <p>Drag the sliders to change the color.</p>
  <div class='wrapper'>
    <div class='first'>
      <input id='rangeR' class='range' type='range' min='0' max='255' value='255'>
    </div>
    <div class='second'>
      <p>Red: <span id='textR'></span></p>
    </div>
  </div>
  <div class='wrapper'>
    <div class='first'>
      <input id='rangeG' class='range' type='range' min='0' max='255' value='255'>
    </div>
    <div class='second'>
      <p>Green: <span id='textG'></span></p>
    </div>
  </div>
  <div class='wrapper'>
    <div class='first'>
      <input id='rangeB' class='range' type='range' min='0' max='255' value='255'>
    </div>
    <div class='second'>
      <p>Blue: <span id='textB'></span></p>
    </div>
  </div>
  <div class='wrapper'>
    <div class='first'>
      <input id='rangeW' class='range' type='range' min='0' max='255' value='255'>
    </div>
    <div class='second'>
      <p>White: <span id='textW'></span></p>
    </div>
  </div>
  <br>
  <br>
  <br>
  <br>
  <br>
  <center>
    <input class='button' type='button' onclick='SendRGBW()' style='width:140px; height:60px' value="Send">
    <p id='textRGBW'>0 0 0 0</p>
  </center>
  <br>
  <br>
  <br>
  <div id="item">
    <p>Animations</p>
    <table id="t3" class="table1">
      <tr>
        <th>
          <input id="b20" class='notselected' type='button' onclick='SendAnimType(0)' style='font-size: 12px;' value="Nothing">
        </th>
        <th>
          <input id="b21" class='notselected' type='button' onclick='SendAnimType(1)' style='font-size: 12px;' value="Rainbow">
        </th>
        <th>
          <input id="b22" class='notselected' type='button' onclick='SendAnimType(2)' style='font-size: 12px;' value="Random">
        </th>
      </tr>
      <tr>
        <td>
          <input id="b23" class='notselected' type='button' onclick='SendAnimType(3)' style='font-size: 12px;' value="Random smooth">
        </td>
        <td>
          <input id="b24" class='notselected' type='button' onclick='SendAnimType(4)' style='font-size: 12px;' value="Random blink">
        </td>
        <td>
          <input id="b25" class='notselected' type='button' onclick='SendAnimSpeed(30)' style='font-size: 12px;' value="Speed selection" disabled>
        </td>
      </tr>
    </table>
  </div>
  <script src="PageColorPicker.js"></script>
</body>
</html>
)=====";

const char* pageColorPicker_css PROGMEM = R"=====(
p,
h1 {
    font-family: 'Roboto', Times, serif;
}

#bg {
    position: fixed;
    left: 0px;
    top: 0px;
    height: 100%;
    width: 100%;
    z-index: -1;
    background: linear-gradient(to top, rgb(0, 0, 0) 0%, rgb(255, 255, 255) 40%);
}

.wrapper {
    width: 100%;
    clear: both;
}

.first {
    width: 70%;
    float: left;
    margin-top: 10px;
}

.second {
    width: auto;
    float: left;
    margin-left: 10px;
}

input.button {
    border: none;
    border-radius: 8px;
    color: black;
    text-align: center;
    font-size: 16px;
    opacity: 1;
    transition-duration: 0.4s;
}

input.button:hover {
    opacity: 0.7;
}

.range {
    -webkit-appearance: none;
    width: 100%;
    height: 30px;
    border-radius: 3px;
    background: #d3d3d3;
    outline: none;
    opacity: 1;
    -webkit-transition: .2s;
    transition: opacity .2s;
}

.range:hover {
    opacity: 0.7;
}

.range::-webkit-slider-thumb {
    -webkit-appearance: none;
    appearance: none;
    width: 30px;
    height: 30px;
    border-radius: 3px;
    background: #4CAF50;
    cursor: pointer;
}

.range::-moz-range-thumb {
    width: 30px;
    height: 30px;
    border-radius: 3px;
    background: #4CAF50;
    cursor: pointer;
}

#rangeR.range::-webkit-slider-thumb,
-moz-range-thumb {
    background: #F00;
}

#rangeG.range::-webkit-slider-thumb,
-moz-range-thumb {
    background: #0F0;
}

#rangeB.range::-webkit-slider-thumb,
-moz-range-thumb {
    background: #00F;
}

#rangeW.range::-webkit-slider-thumb,
-moz-range-thumb {
    background: #FFF;
    border: 1px solid black;
}
)=====";

const char* pageColorPicker_js PROGMEM = R"=====(
var url_string = window.location.href;
var url = new URL(url_string);
var id = parseInt(url.searchParams.get("id")) - 1;
console.log("Id: " + id);

var sliderR = document.getElementById('rangeR');
var outputR = document.getElementById('textR');
var valR = 0;

var sliderG = document.getElementById('rangeG');
var outputG = document.getElementById('textG');
var valG = 0;

var sliderB = document.getElementById('rangeB');
var outputB = document.getElementById('textB');
var valB = 0;

var sliderW = document.getElementById('rangeW');
var outputW = document.getElementById('textW');
var valW = 0;

var endValue = document.getElementById('textRGBW');
var valRGBW = "";



var xhttp0 = new XMLHttpRequest();
xhttp0.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var jsonArray = JSON.parse(this.responseText);
        // var jsonArray = '[{"name": "", "r": 0, "g": 0, "b": 0, "w": 0, "isTransitioning": 0},...]';
        var jsonObject = jsonArray[id];
		var valname = jsonObject.name;
		valR = jsonObject.r;
        valG = jsonObject.g;
        valB = jsonObject.b;
        valW = jsonObject.w;
		//var valIsTransitioning = jsonObject.isTransitioning;
		
        outputR.innerHTML = valR;
        outputG.innerHTML = valG;
        outputB.innerHTML = valB;
        outputW.innerHTML = valW;

        sliderR.value = valR;
        sliderG.value = valG;
        sliderB.value = valB;
        sliderW.value = valW;

        Update();
    }
};
xhttp0.open('GET', '/sendrgbw', true);
xhttp0.send();



sliderR.oninput = function () {
    valR = +this.value;
    outputR.innerHTML = valR;
    Update();
}
sliderG.oninput = function () {
    valG = +this.value;
    outputG.innerHTML = valG;
    Update();
}
sliderB.oninput = function () {
    valB = +this.value;
    outputB.innerHTML = valB;
    Update();
}
sliderW.oninput = function () {
    valW = +this.value;
    outputW.innerHTML = valW;
    Update();
}


function Update() {
    valRGBW = '' + valR + ' ' + valG + ' ' + valB + ' ' + valW;
    endValue.innerHTML = valRGBW;

    document.getElementById("bg").style.background = 'linear-gradient(to top, rgb(' + ((valR >= valW) ? valR : valW) + ',' + ((valG >= valW) ? valG : valW) + ',' + ((valB >= valW) ? valB : valW) + ') 0%, rgb(255, 255, 255) 40%)';
}

function SendRGBW() {
	var tid = id + 1;
	var tvalR = valR + 1;
	var tvalG = valG + 1;
	var tvalB = valB + 1;
	var tvalW = valW + 1;
    console.log('Send: /sendrgbw?id=' + tid + '&r=' + tvalR + '&g=' + tvalG + '&b=' + tvalB + '&w=' + tvalW);
    var xhttp = new XMLHttpRequest();
    xhttp.open('GET', '/sendrgbw?id=' + tid + '&r=' + tvalR + '&g=' + tvalG + '&b=' + tvalB + '&w=' + tvalW, true);
    xhttp.send();
}

function SendAnimType(animType) {
	var tid = id + 1;
	var tanimType = animType + 1;
    console.log('Send: /sendrgbw?id=' + tid + '&at=' + tanimType);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var jsonObject = JSON.parse(this.responseText);
            // var jsonObject = {"animType": 0, "animSpeed": 0}';
            var vat = jsonObject.animType;
            var vas = jsonObject.animSpeed;

            // Set border
            for (var j = 0; j < 6; j++) {
                if (vat == j) {
                    document.getElementById("b2" + j).className = "selected";
                }
                else {
                    document.getElementById("b2" + j).className = "notselected";
                }
            }
        }
    };
    xhttp.open('GET', '/sendrgbw?id=' + tid + '&at=' + tanimType, true);
    xhttp.send();
}

function SendAnimSpeed(animSpeed) {
	var tid = id + 1;
	var tanimSpeed = animSpeed + 1;
    console.log('Send: /sendrgbw?id=' + tid + '&as=' + tanimSpeed);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var jsonObject = JSON.parse(this.responseText);
            // var jsonObject = {"animType": 0, "animSpeed": 0}';
            var vat = jsonObject.animType;
            var vas = jsonObject.animSpeed;

            //
        }
    };
    xhttp.open('GET', '/sendrgbw?id=' + tid + '&as=' + tanimSpeed, true);
    xhttp.send();
}
)=====";