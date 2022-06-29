const String HTML = R"=====(
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>

    <style>

        @font-face {
            font-family: 'Brush';
                src: url("data:application/octet-stream;base64,AAEAAAAOAIAAAwBgT1MvMnG4ijIAAADsAAAAVmNtYXBA5XCVAAABRAAAAyhjdnQgZ79p5wAAB");
        }

        body {
            background-image:linear-gradient(45deg, #e05b33, #24343d);
            font-size: 40px;
        }

        div {
            background-position: center;
            text-align: center;
            font-family: 'Brush';
        }

        button {
            height:15%;
            width:90%;
            border-radius: 50px;
            font-family: 'Brush';
            color: black;
            margin-top: 20px;
        }

        .button0 {
            margin-bottom: 50px;
            background: linear-gradient(to right, blue, pink);
            border-radius: 5px;
            width: 90%;
            height: 25%;
            font-size: 20px;
            border-width: thick;
            border-color: linear-gradient(to right, rgb(255, 0, 0), rgb(0, 0, 0));;
        }

        .button1 {
            background-image: linear-gradient(90deg, green, #9af371);
            width:45%;
        }

        .button2 {
            background-image: linear-gradient(90deg, blue, #57aee0);
            width:45%;
        }

        .button3 {
            background-image: linear-gradient(90deg, red, #d67628);

        }

        .button4 {
            background-image: linear-gradient(90deg, purple, #d67628);
        }

    </style>

</head>
<body>
    <div>
        <a href=\><button class="button0"><h1>e-GATE <br> CONTROLLER</h1></button></a>
        <a href=\CloseAllGates><button class="button1"><h1>CLOSE</h1></button></a>
        <a href=\OpenAllGates><button class="button2"><h1>OPEN</h1></button></a>
        <a href=\StopAllGates><button class="button3"><h1>STOP</h1></button></a>
        <a href=\OpenCloseSingleGate><button class="button4"><h1>OPEN / CLOSE Single Gate</h1></button></a>
    </div>
</body>
</html>
)=====";