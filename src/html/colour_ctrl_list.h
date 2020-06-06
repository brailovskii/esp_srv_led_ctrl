#ifndef _COLOUR_CTRL_LIST_H_
#define _COLOUR_CTRL_LIST_H_




const char html_page_colour_ctrl_list[] = R"=====(






<!DOCTYPE html>
<html>

<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <style>
        .button {
            background-color: #4CAF50;
            /* Green */
            border: none;
            color: white;
            padding: 20px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
            width: 100%;
            font-size: 25px;
        }

        .button001 {
            border-radius: 2px;
            background-color: aqua;
            color: chocolate;
        }

        .button002 {
            border-radius: 4px;
            background-color: cadetblue;
            color: crimson;
        }

        .button003 {
            border-radius: 8px;
            background-color: chocolate;
            color: darkmagenta;
        }

        .button004 {
            border-radius: 12px;
        }

        .button100 {
            border-radius: 10%;
        }
    </style>
    <script>
    </script>
</head>

<body>

    <div style="width: 100%;">

        <h1 style="text-align: center;">Press on one of the buttons to select mode</h1>

        <button class="button button001" onclick="window.location.href = 'html_led_ctrl_0001.html';"> RGB colour
            select</button>
        <p></p>
        <button class="button button002" onclick="window.location.href = 'html_led_ctrl_0002.html';"> Random colour
        </button>
        <p></p>
        <button class="button button003" onclick="window.location.href = 'html_led_ctrl_0003.html';"> Animated leds
        </button>
        <p></p>
        <button class="button button004" onclick="window.location.href = 'html_led_ctrl_0007.html';"> Morse Message
        </button>
        <p>
            <h6> </h6>
        </p>
        <p>
            <h6> </h6>
        </p>
        <p>
            <h6> </h6>
        </p>
        <p>
            <h6> </h6>
        </p>
        <p>
            <h6> </h6>
        </p>
        <button class="button button100" onclick="window.location.href = '/';"> Main Page </button>
    </div>

</body>

</html>








)=====";




#endif

