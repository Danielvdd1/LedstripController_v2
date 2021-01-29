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