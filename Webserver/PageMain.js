var switch0 = document.getElementById('switch0');
var switch1 = document.getElementById('switch1');
var switch2 = document.getElementById('switch2');


// Request value rgbw
var xhttp0 = new XMLHttpRequest();
xhttp0.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var jsonArray = JSON.parse(this.responseText);
        // var jsonArray = '[{"r": 0, "g": 0, "b": 0, "w": 0}, {"r": 1, "g": 1, "b": 1, "w": 1}]';
        for (var i = 0; i < jsonArray.length; i++) {
            var jsonObject = jsonArray[i];
            var vr = jsonObject.r;
            var vg = jsonObject.g;
            var vb = jsonObject.b;
            var vw = jsonObject.w;

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
                //document.getElementById("b" + i + "5").style = 'background: rgb(' + (vr > vw ? vr : vw) + ',' + (vg > vw ? vg : vw) + ',' + (vb > vw ? vb : vw) + ')';
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
        var jsonObject = JSON.parse(this.responseText);
        // var jsonObject = '{"a": 0, "b": 0, "c": 0}';

        var va = jsonObject.a;

        console.log("A: " + va);

        switch0.checked = (va == 1 ? true : false);
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
// Request value C
var xhttp3 = new XMLHttpRequest();
xhttp3.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var value = parseInt(this.responseText);

        console.log("C: " + value);

        value = (value === 1 ? true : false)

        switch2.disabled = false;
        switch2.checked = value;
    }
    else if (this.readyState == 4) {
        console.log("C: offline");
    }
};
xhttp3.open('GET', 'http://192.168.178.62/state', true);
xhttp3.send();

// Request value anim
var xhttp4 = new XMLHttpRequest();
xhttp4.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var jsonObject = JSON.parse(this.responseText);
        // var jsonObject = {"anim": 0, "speed": 0}';
        var vanim = jsonObject.anim;
        var vspeed = jsonObject.speed;

        // Set border
        for (var j = 0; j < 6; j++) {
            if (vanim == j) {
                document.getElementById("b2" + j).className = "selected";
            }
            else {
                document.getElementById("b2" + j).className = "notselected";
            }
        }
    }
};
xhttp4.open('GET', '/sendanim', true);
xhttp4.send();

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
    SendW(1, switch0.checked);
});
switch1.addEventListener('click', function () {
    SendW(2, switch1.checked);
});
switch2.addEventListener('click', function () {
    SendW(3, switch2.checked);
});



function SendRGBW(id, valR, valG, valB, valW) {
    console.log('Send: /sendrgbw?id=' + id + '&r=' + valR + '&g=' + valG + '&b=' + valB + '&w=' + valW);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var jsonArray = JSON.parse(this.responseText);
            // var jsonArray = '[{"r": 0, "g": 0, "b": 0, "w": 0}, {"r": 1, "g": 1, "b": 1, "w": 1}]';
            for (var i = 0; i < jsonArray.length; i++) {
                var jsonObject = jsonArray[i];
                var vr = jsonObject.r;
                var vg = jsonObject.g;
                var vb = jsonObject.b;
                var vw = jsonObject.w;

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
    if (id === 1) {
        console.log('Send: /sendw?id=' + id + '&w=' + (valW ? '1' : '0'));
        var xhttp = new XMLHttpRequest();
        xhttp.open('GET', '/sendw?id=' + id + '&w=' + (valW ? '1' : '0'), true);
        xhttp.send();
    } else if (id === 2) {
        console.log('Send: http://192.168.178.61/' + (valW ? 'on' : 'off'));
        var xhttp = new XMLHttpRequest();
        xhttp.open('GET', 'http://192.168.178.61/' + (valW ? 'on' : 'off'), true);
        xhttp.send();
    }
    else if (id === 3) {
        console.log('Send: http://192.168.178.62/' + (valW ? 'on' : 'off'));
        var xhttp = new XMLHttpRequest();
        xhttp.open('GET', 'http://192.168.178.62/' + (valW ? 'on' : 'off'), true);
        xhttp.send();
    }
}

function SendAnim(anim) {
    console.log('Send: /sendanim?anim=' + anim);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var jsonObject = JSON.parse(this.responseText);
            // var jsonObject = {"anim": 0, "speed": 0}';
            var vanim = jsonObject.anim;
            var vspeed = jsonObject.speed;

            // Set border
            for (var j = 0; j < 6; j++) {
                if (vanim == j) {
                    document.getElementById("b2" + j).className = "selected";
                }
                else {
                    document.getElementById("b2" + j).className = "notselected";
                }
            }
        }
    };
    xhttp.open('GET', '/sendanim?anim=' + anim, true);
    xhttp.send();
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