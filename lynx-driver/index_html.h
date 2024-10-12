const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Lynx Robot Control</title>
  <style>
    body, html {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }
    h1 {
      text-align: center;
      font-size: 34px;
    }
    .button {
      padding: 30px;
      text-align: center;
      font-size: 28px;
      background-color: #008CBA;
      color: white;
      border: none;
      cursor: pointer;
      margin: 5px;
    }
    .button:hover {
      background-color: #006f9b;
    }
    .container {
      display: flex;
      justify-content: center;
      width: 100vw;
      box-sizing: border-box;
    }
    .controller {
      display: flex;
      flex-direction: column;
      align-items: center;
      margin-top: 20px;
    }
    .row {
      display: flex;
      justify-content: center;
      width: 100%;
    }
    .row > .button {
      flex: 1;
    }
    .middle-row {
      display: flex;
      justify-content: center;
      align-items: stretch;
      width: 100%;
    }
    .middle-row .button, .middle-row .column {
      flex: 1;
    }
    .column {
      display: flex;
      flex-direction: column;
      flex: 1;
    }
    .column .button {
      flex: 1;
    }
  </style>
</head>
<body>
  <h1>Lynx Robot Control</h1>
  <div class="container">
    <form action="/setState" method="get" style="width: 100%; display: flex; justify-content: center;">
      <button class="button" name="state" value="standby">Standby</button>
      <button class="button" name="state" value="teach">Teach</button>
      <button class="button" name="state" value="operate">Operate</button>
    </form>
  </div>

  <div class="controller">
    <form action="/control" method="get">
      <div class="row">
        <button class="button" name="action" value="up">Up</button>
      </div>
      <div class="middle-row">
        <button class="button" name="action" value="left">Left</button>
        <div class="column">
          <button class="button" name="action" value="forward">Forward</button>
          <button class="button" name="action" value="backward">Backward</button>
        </div>
        <button class="button" name="action" value="right">Right</button>
      </div>
      <div class="row">
        <button class="button" name="action" value="down">Down</button>
      </div>
    </form>
  </div>
</body>
</html>
)rawliteral";
