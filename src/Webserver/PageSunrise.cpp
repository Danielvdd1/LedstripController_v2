#include <avr/pgmspace.h>

//<link rel='icon' type='image/png' href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAZdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjAuMjHxIGmVAAACyElEQVQ4T32R60vTURjHBxb0T0SQkNTmrUzdvKZZWo3Ssla6Iu1monazIlFLX1iaRnaPmtosMcoudDMlSGPSC52KaZYVbpk6NTdzTn3z6beDCpL2hQcO5/D9PM/zPbK5lFq7mfSGWHp7+xgeHubs1ziyO+OZep5fWt1a98Mv1ZjNZgYHhxix2bDbxxgfH2dycpLcH9r5IbGXQ0isjCTlTbTUuZffv4clsx3HlNlZhaaTnOuKJ6Nl578gzfU1JFRE0tHRQU/PL4akCZyAiYmJGYCzsj/Hkdm+azagoaGB7cUhaHURtLV9wmz6iaW/H5vVhsPhmAXJaNFwplkj7qfsMtmH+npiCoLRXFtDU2MT3d3dYg2LxYLVamV0dHQGdMIQKwJ2noW5XK/n7dsaNuX4S5AgYi8H09Rk5FtXFyaTSUAGBwawSYEmv1KTUq0m7V20CLZPaiIrLSnlSVUVEemriMr0Q52rYst5JQaDAaPRKDJxwrQlESKj/Y+iOPhkA2NjY/RLawqAKtGdwAMKgpM9CU31IuyoN+HHV7LulA9RGb5sPOvP5rwAtl0KFWvG3QlnT9l68d0SoARlogK/3XL89shR7lWgTFAQIN0F7pdqn4IgJzzJg/Bj3kRKQHWukpiLQVJejcgCDnig2ueBn1bOao0cnx1uuKtdeV2ag6HqIq3VxbTXXsU33g1VglyAgw95EnbESwBFkAFSR++ty1jiu5grWcm80GVTmHWS74bbDLTeoyj7NP3GUr68v4FXjCv+2hViMmGe1opIVzKT4qirzMP8UUegQo7911PGh16iki9ncqSGCWu19HYXedTS2eZpFWclUV2WS+ubYhYtWMCfzgpGfjxkoYsLjoHn2H8+xtJ8b27ztB4UneLZrSzeV+QJUJc0do/Uta9Rx/e6m/83T0ufn0J5fhr6/FTKC9K4X5CK/kLKHGaZ7C/+EiOzvseUTgAAAABJRU5ErkJggg=="/>
//<link rel='icon' type='image/png' href="data:image/png;base64, iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO9TXL0Y4OHwAAAABJRU5ErkJggg=="/>

const char* pageSunrise_html PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>Led strip</title>
  <link rel='icon' type='image/png' href="data:image/png;base64, iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO9TXL0Y4OHwAAAABJRU5ErkJggg=="/>
  <meta name="theme-color" content="#4286f4">
  
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <link href='http://fonts.googleapis.com/css?family=Roboto:400,700' rel='stylesheet' type='text/css'>
  <!--<link href='http://fonts.googleapis.com/css?family=Roboto:400,100,100italic,300,300italic,400italic,500,500italic,700,700italic,900italic,900' rel='stylesheet' type='text/css'>-->
  <link rel="stylesheet" href="PageSunrise.css">
  <!--
	  Author: Daniël van der Drift
  -->
</head>

<body>
  <div id="bg"></div>
  <h1>Ledstrip controller</h1>
  <h2>Sunrise</h2>
  <div>
    <label for="inputSunriseEnabled">Enable sunrise:</label>
    <input type="checkbox" id="inputSunriseEnabled" name="inputSunriseEnabled">
  </div>
  <div>
    <label for="inputSunriseTime">Sunrise time (in munutes):</label>
    <input type="text" id="inputSunriseTime" name="inputSunriseTime">
    <input type="button" id="buttonSunriseTime" value="Send">
  </div>
  <div>
    <label for="inputSunriseDuration">Sunrise duration (in munutes, 1 hour max):</label>
    <input type="text" id="inputSunriseDuration" name="inputSunriseDuration">
    <input type="button" id="buttonSunriseDuration" value="Send">
  </div>
  <div>
    <h3>Example</h3>
    <p>Set the sunrise time in minutes. If you want the sunrise to end at 8 am, multiply 8 with 60. This makes 480 minutes.</p>
    <p>Set the sunrise duration in minutes. The maximum duration is 59 minutes.</p>
    <p>If input value is higher than allowed. It will be rounded down.</p>
  </div>
  <script src="PageSunrise.js"></script>
</body>
</html>
)=====";

const char* pageSunrise_css PROGMEM = R"=====(
body {
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

const char* pageSunrise_js PROGMEM = R"=====(
var switch0 = document.getElementById('inputSunriseEnabled');
var text1 = document.getElementById('inputSunriseTime');
var text2 = document.getElementById('inputSunriseDuration');
var button1 = document.getElementById('buttonSunriseTime');
var button2 = document.getElementById('buttonSunriseDuration');


// Request sunrise values
var xhttp0 = new XMLHttpRequest();
xhttp0.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
		var jsonObject = JSON.parse(this.responseText);
        // var jsonObject = '{"sunriseEnabled": 0, "sunriseTime": 0, "sunriseDuration": 0, "sunriseState": 0}';
        valE = jsonObject.sunriseEnabled;
        valT = jsonObject.sunriseTime;
        valD = jsonObject.sunriseDuration;
        //valS = jsonObject.sunriseState;

        switch0.checked = (valE === 0 ? false : true);
        text1.value = valT;
        text2.value = valD;
    }
    else if (this.readyState == 4) {
        console.log("Offline");
    }
};
xhttp0.open('GET', '/sendsunrise', true);
xhttp0.send();



// Switch event
switch0.addEventListener('click', function () {
    SendSunriseEnabled(switch0.checked);
});

// Button events
button1.addEventListener('click', function () {
    var val = parseInt(text1.value);
    SendSunriseTime(val);
});
button2.addEventListener('click', function () {
    var val = parseInt(text2.value);
    SendSunriseDuration(val);
});


function SendSunriseEnabled(valE) {
    tvalE = (valE ? '2' : '1');
    console.log('Send: /sendsunrise?e=' + tvalE);
    var xhttp = new XMLHttpRequest();
    xhttp.open('GET', '/sendsunrise?e=' + tvalE, true);
    xhttp.send();
}

function SendSunriseTime(valT) {
    tvalT = valT + 1;
    console.log('Send: /sendsunrise?t=' + tvalT);
    var xhttp = new XMLHttpRequest();
    xhttp.open('GET', '/sendsunrise?t=' + tvalT, true);
    xhttp.send();
}

function SendSunriseDuration(valD) {
    tvalD = valD + 1;
    console.log('Send: /sendsunrise?d=' + tvalD);
    var xhttp = new XMLHttpRequest();
    xhttp.open('GET', '/sendsunrise?d=' + tvalD, true);
    xhttp.send();
}
)=====";