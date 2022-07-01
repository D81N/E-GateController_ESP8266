#include <Arduino.h>

const String HTML = R"=====(
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>E-GateController</title>

    <link rel="icon" type="image/png" sizes="16x16" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAYAAACqaXHeAAAABmJLR0QA/wD/AP+gvaeTAAAETElEQVR4nO2Z3WsVRxTAf6Z+XK2JV1GojUax2BjxP6hJRIU+lVp8t9fSqNA++WIFv6gPTani/yFYhfahWiNqKlrFiE9SJbVqUGyhoqaiPrg+nLPsuO73zu69t9kfDHf3zJyZc87OnpnZCxUVFRXlMR/YDvwE3Ab+A14CfwIngB3AvKZZVyCzgG+BJ4ATU54A+1Xnf8Fy4Dqeg6eBIaAXeBeYDXwAfA6cBF5pu+tAT/nm2mUFcA9x6BawLoHOWuAP1ZmgjYNQA8YQR0aBBSl0u4BzeDOhLV+HfXhPPo3zLl14M2G/RbtKoQ5MIsav88kbwDHgBvBUyw2VNbSNy1rt4zFttjrsQAwf0fsasBt4RPwq8Aj4RnUATql8e0m2W+E4YvQQ0A1cxnPwV5WvAuZq6QO2ISuE2+531W3o/fEyHcjLHcToXmCnXt8EBhLoDiJ5w1HdXr0eL8TSgniGGD1X7xvILhBgEXAQWSEmtYypbJG2WaA6IPsER/tsG9wd32yffDPRu8HH2sbEDcBkEYZ2FNEp8EB/u33yNUAnchZYj5cD1gM/I0vfGp/OEv39uxBLC+IY8tS2BNT1R+gF1X2hfZ2wYFdpNBCjT4bUvwMMA/e1DKssiBG8FaVwZgCHgX+0HFJZWurIev4K2cwEsQvv3Q/b6fXj5Yb5IW2iSO3PId5OTD9kGBhkM+Mg29mukDZLtQRRJ/9WOLU/D7XRR0hiclSWhRqymXGQg01YEIKo4x2GrgEzM9oQ649/FXD0t2ZcO2TjOfAZchweAK4Q/jqY9COBGwDuAp8iX42ykNqfYd6eMt9lHNzlfeCS0d8pYCvwITAHOequRLL9iNHuKrAs59ip/ZkOfG80HlZZXmpI0vs3wCB/eQrswTsM5SHWn2khik5MfVbqyJT+BFiNJMAOZJMzBvwCHEWyvk0S+fMb4U9l1LJBZZDIHzMiccnO9mwomtT+uNFJKs9Ll9F3ZwH9J/KnqMNQErpDrkslKAD+96UoBkOubZPYn9GAxjaToN+AM4bsTES7rGTyxz+4zVlg9rUhwKiPCx4zUd+2AhCk58oW4n3zM8tNrYvSzWtHSwTA3RabX4ov++paNgBJjYuSOcBfwHvG/WK8L8k2xkhjf+4OomR9yCHnrCG7iByQ/LrdvDkDzqpuX4Zx09ifqAObxf2jc9CQuf8XzCpozKYFYBz4EfjSkHUAXwMvDNkL4Cutc2VDyL9B460QgKgO0souRBh6IUY3razpAegBNgIHeNPRB8Am436Tysw2B5D9Qk+GcdPYn6gDm2Uv3uHHHK9T64oYs2kBmADOA0cy6B5R3YkiA+A/Ezs+edx9FllS8oyR2H4b3/vSEvdhJW2gctHM7wEtQTNmQKlPOI5qBjRhTNvJMhdTfgYkXQbbndBlcMrPgKQ5wOZGqExZ7Aye8jOgygHl29JapM0B7UaVA+Jo2g6sBZgG1QyoqKiY4rwGQ+W7NwGij+UAAAAASUVORK5CYII=" />

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