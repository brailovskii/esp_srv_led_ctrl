#ifndef _HTML_LED_CTRL_0001_H_
#define _HTML_LED_CTRL_0001_H_




const char html_page_led_ctrl_0001[] = R"=====(








    
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


        var gR = -1,
            gG = -1,
            gB = -1,
            gBr = -1,
            g_cnt = 0;


        function slider_update_control(save_me) {


            valueR = document.getElementById("range_R").value;
            valueG = document.getElementById("range_G").value;
            valueB = document.getElementById("range_B").value;
            valueBrightness = document.getElementById("range_Brightness").value;

            if(save_me == 1){

            }


            if (valueR != gR || valueG != gG || valueB != gB || valueBrightness != gBr || save_me) {
                
                var send_str = '{"type":"led_ctrl","id":"1","save":"no","cnt":"0000","r":"0000","g":"0000","b":"0000","br":"0000"}';

                if(save_me){
                    send_str = send_str.replace('"save":"no"', '"save":"yes"');
                }
                send_str = send_str.replace('"cnt":"0000"', '"cnt":"' + g_cnt + '"');
                send_str = send_str.replace('"r":"0000"', '"r":"' +  valueR + '"');
                send_str = send_str.replace('"g":"0000"', '"g":"' +  valueG + '"');
                send_str = send_str.replace('"b":"0000"', '"b":"' +  valueB + '"');
                send_str = send_str.replace('"br":"0000"', '"br":"' +  valueBrightness + '"');

                document.getElementById("sl_status").innerHTML = send_str;

                g_cnt = g_cnt + 1;
                
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) {
                        document.getElementById("response_text").innerHTML =
                            this.responseText;
                    }
                };
                xhttp.open("GET", "led_ctrl_0001?json_msg=" + send_str, true);
                xhttp.send();
                
            }

            gR = valueR;
            gG = valueG;
            gB = valueB;
            gBr = valueBrightness;

        }

    </script>
</head>

<body>

    <h1>Play with my colours! :)</h1>

    <div style="width:100%;">
        <p>Red:</p>
        <input type="range" min="0" max="255" value="5555" id="range_R" class="slider" style="background: #cc0000;"
            onchange="slider_update_control(0)">

        <p>Green:</p>
        <input type="range" min="0" max="255" value="5555" id="range_G" class="slider" style="background: #00cc00;"
            onchange="slider_update_control(0)">

        <p>Blue:</p>
        <input type="range" min="0" max="255" value="5555" id="range_B" class="slider" style="background: #0000cc;"
            onchange="slider_update_control(0)">

        <p>Brightness:</p>
        <input type="range" min="0" max="255" value="5555" id="range_Brightness" class="slider"
            style="background: #cccccc;" onchange="slider_update_control(0)">

        <p></p><p></p>

        <input type="button" id="save_btn" style="margin-left:25%; width:50%;height:50px; background-color: #4CAF50;border-color:burlywood;" value="Save" onclick="slider_update_control(1)">

        <p></p>
        <h5 id="response_text"> Response </h5>
        <h5 id="sl_status"> None </h5>
    </div>

</body>

</html>









)=====";













#endif
