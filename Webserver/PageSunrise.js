var switch0 = document.getElementById('switch0');


// Request value e
var xhttp0 = new XMLHttpRequest();
xhttp0.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var value = parseInt(this.responseText);

        console.log("E: " + value);

        value = (value === 1 ? true : false)

        switch0.checked = value;
    }
    else if (this.readyState == 4) {
        console.log("E: offline");
    }
};
xhttp0.open('GET', '/sendsunrise', true);
xhttp0.send();



// Switch events
switch0.addEventListener('click', function () {
    SendSunrise(switch0.checked);
});


function SendSunrise(valE) {
    console.log('Send: /sendsunrise?e=' + (valE ? '2' : '1'));
    var xhttp = new XMLHttpRequest();
    xhttp.open('GET', '/sendsunrise?e=' + (valE ? '2' : '1'), true);
    xhttp.send();
}