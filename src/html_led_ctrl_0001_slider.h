

const char MAIN_page[] = R"=====(

<!DOCTYPE html>
<html>

<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <style>
        .slider {
            -webkit-appearance: none;
            width: 100%;
            height: 25px;
            background: #d3d3e3;
            outline: none;
            opacity: 0.7;
            -webkit-transition: .2s;
            transition: opacity .2s;
        }

        .slider:hover {
            opacity: 1;
        }

        .slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 25px;
            height: 25px;
            background: #000000;
            cursor: pointer;
        }

        .slider::-moz-range-thumb {
            width: 25px;
            height: 25px;
            background: #4CAF50;
            cursor: pointer;
        }
    </style>

    <script>
        function fun_range_onChange() {

            slider_update_control();

        }


        setInterval(function () {
            // invoke this function every -n milliseconds
            slider_update_control();
        }, 100); // interval duration in milliseconds


        var gR = -1,
            gG = -1,
            gB = -1,
            gBr = -1;

        function slider_update_control() {


            valueR = document.getElementById("range_R").value;
            valueG = document.getElementById("range_G").value;
            valueB = document.getElementById("range_B").value;
            valueBrightness = document.getElementById("range_Brightness").value;

            if (valueR != gR || valueG != gG || valueB != gB || valueBrightness != gBr) {
                
                state_str = "R=" + valueR;
                state_str += "&G=" + valueG;
                state_str += "&B=" + valueB;
                state_str += "&Brightness=" + valueBrightness;

                document.getElementById("sl_status").innerHTML = state_str;

                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) {
                        document.getElementById("response_text").innerHTML =
                            this.responseText;
                    }
                };
                xhttp.open("GET", "led_ctrl_0001?" + state_str, true);
                xhttp.send();
            }

            gR = valueR;
            gG = valueG;
            gB = valueB;
            gBr = valueBrightness;

        }

        function getData() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("ADCValue").innerHTML =
                        this.responseText;
                }
            };
            xhttp.open("GET", "readADC", true);
            xhttp.send();
        }
    </script>
</head>

<body>

    <h1>Custom Range Slider</h1>

    <div style="width:100%;">
        <p>Red:</p>
        <input type="range" min="0" max="100" value="50" id="range_R" class="slider" style="background: #cc0000;"
            onchange="fun_range_onChange()">

        <p>Green:</p>
        <input type="range" min="0" max="100" value="50" id="range_G" class="slider" style="background: #00cc00;"
            onchange="fun_range_onChange()">

        <p>Blue:</p>
        <input type="range" min="0" max="100" value="50" id="range_B" class="slider" style="background: #0000cc;"
            onchange="fun_range_onChange()">

        <p>Brightness:</p>
        <input type="range" min="0" max="100" value="50" id="range_Brightness" class="slider"
            style="background: #cccccc;" onchange="fun_range_onChange()">

        <p></p>
        <h5 id="response_text"> Response </h5>
        <h5 id="sl_status"> None </h5>
    </div>

</body>

</html>

)=====";