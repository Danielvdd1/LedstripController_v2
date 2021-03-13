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