#ifndef _HTML_LED_CTRL_0002_H_
#define _HTML_LED_CTRL_0002_H_



const char html_page_led_ctrl_0002[] = R"=====(


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
        setInterval(function () {
            // invoke this function every -n milliseconds
            slider_update_control(0);
        }, 200); // interval duration in milliseconds


        var gRmin = -1,
            gRmax = -1,
            gGmin = -1,
            gGmax = -1,
            gBmin = -1,
            gBmax = -1,
            gRrate = -1,
            gGrate = -1,
            gBrate = -1,
            gColUpdRate = -1,
            g_cnt = 0;


        function slider_update_control(save_me) {


            Rmin = document.getElementById("Rmin").value;
            Rmax = document.getElementById("Rmax").value;
            Gmin = document.getElementById("Gmin").value;
            Gmax = document.getElementById("Gmax").value;
            Bmin = document.getElementById("Bmin").value;
            Bmax = document.getElementById("Bmax").value;

            Rrate = document.getElementById("Rrate").value;
            Brate = document.getElementById("Grate").value;
            Grate = document.getElementById("Brate").value;

            ColUpdRate = document.getElementById("col_upd_rate").value;


            if (gRmin != Rmin || gRmax != Rmax || gGmin != Gmin || gGmax != Gmax || gBmin != Bmin || gBmax != Bmax  || gRrate != Rrate || gGrate != Grate || gBrate != Brate  || gColUpdRate != ColUpdRate  || save_me) {

                var send_str =
                    '{"type":"led_ctrl","id":"2","save":"no","cnt":"0000","rmin":"0000","rmax":"0000","gmin":"0000","gmax":"0000","bmin":"0000","bmax":"0000","rrate":"0000","grate":"0000","brate":"0000","col_upd_rate":"0000"}';

                if (save_me) {
                    send_str = send_str.replace('"save":"no"', '"save":"yes"');
                }
                send_str = send_str.replace('"cnt":"0000"', '"cnt":"' + g_cnt + '"');

                send_str = send_str.replace('"rmin":"0000"', '"rmin":"' + Rmin + '"');
                send_str = send_str.replace('"rmax":"0000"', '"rmax":"' + Rmax + '"');
                send_str = send_str.replace('"gmin":"0000"', '"gmin":"' + Gmin + '"');
                send_str = send_str.replace('"gmax":"0000"', '"gmax":"' + Gmax + '"');
                send_str = send_str.replace('"bmin":"0000"', '"bmin":"' + Bmin + '"');
                send_str = send_str.replace('"bmax":"0000"', '"bmax":"' + Bmax + '"');

                send_str = send_str.replace('"rrate":"0000"', '"rrate":"' + Rrate + '"');
                send_str = send_str.replace('"grate":"0000"', '"grate":"' + Grate + '"');
                send_str = send_str.replace('"brate":"0000"', '"brate":"' + Brate + '"');

                send_str = send_str.replace('"col_upd_rate":"0000"', '"col_upd_rate":"' + ColUpdRate + '"');
                

                document.getElementById("sl_status").innerHTML = send_str;

                g_cnt = g_cnt + 1;

                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) {
                        document.getElementById("response_text").innerHTML =
                            this.responseText;
                    }
                };
                xhttp.open("GET", "led_ctrl_0002?json_msg=" + send_str, true);
                xhttp.send();

            }

            gRmin = Rmin;
            gRmax = Rmax;
            gGmin = Gmin;
            gGmax = Gmax;
            gBmin = Bmin;
            gBmax = Bmax;
            gRrate = Rrate;
            gGrate = Grate;
            gBrate = Brate;
            gColUpdRate = ColUpdRate;

        }
    </script>
</head>

<body>

    <h1>Colour Randomizer! :)</h1>

    <div style="width:100%;">
        <p>Colour change Rate</p>
        <input type="range" min="0" max="255" value="5555" id="col_upd_rate" class="slider" style="background: #5b255d;"
            onchange="slider_update_control(0)">

        <p>Red max:</p>
        <input type="range" min="0" max="255" value="5555" id="Rmax" class="slider" style="background: #fa0303;"
            onchange="slider_update_control(0)">

        <p>Red min:</p>
        <input type="range" min="0" max="255" value="5555" id="Rmin" class="slider" style="background: #fa0303;"
            onchange="slider_update_control(0)">

        <p>Green max:</p>
        <input type="range" min="0" max="255" value="5555" id="Gmax" class="slider" style="background: #03f810;"
            onchange="slider_update_control(0)">

        <p>Green min:</p>
        <input type="range" min="0" max="255" value="5555" id="Gmin" class="slider" style="background: #03f810;"
            onchange="slider_update_control(0)">

        <p>Blue max:</p>
        <input type="range" min="0" max="255" value="5555" id="Bmax" class="slider" style="background: #0023ec;"
            onchange="slider_update_control(0)">

        <p>Blue min:</p>
        <input type="range" min="0" max="255" value="5555" id="Bmin" class="slider" style="background: #0023ec;"
            onchange="slider_update_control(0)">


        <p></p>
        <p></p>

        <p>Red update rate:</p>
        <input type="range" min="0" max="255" value="5555" id="Rrate" class="slider" style="background: #fa4e4e;"
            onchange="slider_update_control(0)">
        <p>Green update rate:</p>
        <input type="range" min="0" max="255" value="5555" id="Grate" class="slider" style="background: #54fa4e;"
            onchange="slider_update_control(0)">
        <p>Blue update rate:</p>
        <input type="range" min="0" max="255" value="5555" id="Brate" class="slider" style="background: #764efa;"
            onchange="slider_update_control(0)">

        <input type="button" id="save_btn"
            style="margin-left:25%; width:50%;height:50px; background-color: #4CAF50;border-color:burlywood;"
            value="Save" onclick="slider_update_control(1)">

        <p></p>
        <h5 id="response_text"> Response </h5>
        <h5 id="sl_status"> None </h5>
    </div>

</body>

</html>


)=====";











#endif
