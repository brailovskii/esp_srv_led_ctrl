#ifndef _HTML_MAIN_PAGE_H_
#define _HTML_MAIN_PAGE_H_



const char html_page_main[] = R"=====(




<!DOCTYPE html>
<html>

<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <style>
        .button {
          background-color: #4CAF50; /* Green */
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
            background-color:gold;
            color: chocolate;
            }
        .button002 {
            border-radius: 4px;
            background-color: cadetblue;
            color: crimson;
            }
        .button003 {border-radius: 8px;}
        .button004 {border-radius: 12px;}
        .button100 {border-radius: 10%;}
        </style>
    <script>
    </script>
</head>

<body>


    <div style="width: 100%;">

        <h1 style="text-align: center;">Main page</h1>


            <button class="button button001" onclick="window.location.href = 'colour_ctrl.html';"> Colour control </button>
            <p></p>
            <button class="button button002" onclick="window.location.href = 'settings.html';"> Settings </button>
            
    </div>

</body>

</html>






)=====";




#endif 


