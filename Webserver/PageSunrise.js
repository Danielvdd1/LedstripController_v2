var switch0 = document.getElementById('switch0');


// Request value e
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
    }
    else if (this.readyState == 4) {
        console.log("E: offline");
    }
};
xhttp0.open('GET', '/sendsunrise', true);
xhttp0.send();



// Switch events
switch0.addEventListener('click', function () {
    SendSunriseEnabled(switch0.checked);
});


function SendSunriseEnabled(valE) {
    console.log('Send: /sendsunrise?e=' + (valE ? '2' : '1'));
    var xhttp = new XMLHttpRequest();
    xhttp.open('GET', '/sendsunrise?e=' + (valE ? '2' : '1'), true);
    xhttp.send();
}

function SendSunriseTime(valT) {
    console.log('Send: /sendsunrise?t=' + valT);
    var xhttp = new XMLHttpRequest();
    xhttp.open('GET', '/sendsunrise?t=' + valT, true);
    xhttp.send();
}

function SendSunriseDuration(valD) {
    console.log('Send: /sendsunrise?d=' + valD);
    var xhttp = new XMLHttpRequest();
    xhttp.open('GET', '/sendsunrise?d=' + valD, true);
    xhttp.send();
}