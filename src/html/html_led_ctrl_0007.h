#ifndef _HTML_LED_CTRL_0007_H_
#define _HTML_LED_CTRL_0007_H_



const char html_page_led_ctrl_0007[] = R"=====(



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
        var cnt_200ms = 0;
        setInterval(function () {
            // invoke this function every -n milliseconds
            slider_update_control(0);

            cnt_200ms = cnt_200ms + 1;

            if( cnt_200ms > 10 ){
                document.getElementById("msg_id").style.display = '';
            }
        }, 200); // interval duration in milliseconds


        var gRrate = -1,
            gGrate = -1,
            gBrate = -1,
            gMsg = "msg"
            g_cnt = 0;


        function slider_update_control(save_me) {


            Rrate = document.getElementById("Rrate").value;
            Grate = document.getElementById("Grate").value;
            Brate = document.getElementById("Brate").value;
            Msg  = document.getElementById("msg_id").value;
            //replace all spaces with _
            Msg = Msg.replace(" ", "_");
            Msg = Msg.replace(" ", "_");
            Msg = Msg.replace(" ", "_");
            Msg = Msg.replace(" ", "_");
            Msg = Msg.replace(" ", "_");
            Msg = Msg.replace(" ", "_");
            Msg = Msg.replace(" ", "_");
            Msg = Msg + " "; //add space at the end so we can get it with c functions


            if (gRrate != Rrate || gGrate != Grate || gBrate != Brate || save_me) {

                var send_str =
                    '{"type":"led_ctrl","id":"7","save":"no","cnt":"0000","rrate":"0000","grate":"0000","brate":"0000","msg":"0000"}';

                if (save_me) {
                    send_str = send_str.replace('"save":"no"', '"save":"yes"');
                }
                send_str = send_str.replace('"cnt":"0000"', '"cnt":"' + g_cnt + '"');

                send_str = send_str.replace('"rrate":"0000"', '"rrate":"' + Rrate + '"');
                send_str = send_str.replace('"grate":"0000"', '"grate":"' + Grate + '"');
                send_str = send_str.replace('"brate":"0000"', '"brate":"' + Brate + '"');

                send_str = send_str.replace('"msg":"0000"', '"msg":"' + Msg + '"');


                document.getElementById("sl_status").innerHTML = send_str;

                g_cnt = g_cnt + 1;

                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) {
                        document.getElementById("response_text").innerHTML =
                            this.responseText;
                    }
                };
                xhttp.open("GET", "led_ctrl_0007.html?json_msg=" + send_str, true);
                xhttp.send();

            }

 
            gRrate = Rrate;
            gGrate = Grate;
            gBrate = Brate;
            gMsg = Msg;

        }
    </script>
</head>

<body>

    <h1>There is message for you, guess what is it :)</h1>

    <div style="width:100%;">



        <table style="width: 100%;">


            <tr>
                <td>
                    Red:
                </td>
                <td colspan="2">

                    <input type="range" min="0" max="255" value="5555" id="Rrate" class="slider"
                        style="background: #fa4e4e;" onchange="slider_update_control(0)">
                </td>
            </tr>

            <tr>
                <td colspan="3">
                    <h3></h3>
                </td>
            </tr>

            <tr>
                <td>
                    Green:
                </td>
                <td colspan="2">
                    <input type="range" min="0" max="255" value="5555" id="Grate" class="slider"
                        style="background: #54fa4e;" onchange="slider_update_control(0)">
                </td>
            </tr>

            <tr>
                <td colspan="3">
                    <h3></h3>
                </td>
            </tr>

            <tr>
                <td>
                    Blue:
                </td>
                <td colspan="2">
                    <input type="range" min="0" max="255" value="5555" id="Brate" class="slider"
                        style="background: #764efa;" onchange="slider_update_control(0)">
                </td>
            </tr>

            <tr>
                <td colspan="3">
                    <h3></h3>
                </td>
            </tr>

            <tr>
                <td>
                    Message:
                </td>
                <td colspan="2">
                    <input type="text" value="message" id="msg_id" style="width: 100%; height: 30px; display:none;"  onchange="slider_update_control(0)">
                </td>
            </tr>



        </table>




        <div style="width:100%; word-wrap: break-word;">
            <p></p>
            <input type="button" id="save_btn"
                style="margin-left:0%; width:20%;height:50px; background-color: #4CAF50;border-color:burlywood;"
                value="Save" onclick="slider_update_control(1)">
            <p></p>
            <button
                style="margin-left:0%; width:20%;height:50px; background-color: rgb(40, 78, 41);border-color:burlywood;"
                onclick="window.location.href = '/';"> Main Page </button>
            <p></p>
            <h5 id="response_text" style="word-wrap: break-word;"> Response </h5>
            <h5 id="sl_status" style="word-wrap: break-word;"> None </h5>
        </div>


    </div>

</body>

</html>


)=====";











#endif
