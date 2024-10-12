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
      .container {
        display: flex;
        width: 100vw; /* Full width of the screen */
        box-sizing: border-box;
      }
      h1 {
        text-align: center;
        font-size: 34px;
      }
      .button {
        flex: 1; /* Ensures all buttons take equal width */
        margin: 5px;
        padding: 60px;
        text-align: center;
        font-size: 28px;
        background-color: #008CBA;
        color: white;
        border: none;
        cursor: pointer;
      }
      .button:hover {
        background-color: #006f9b;
      }
    </style>
  </head>
  <body>
    <h1>Robot Control</h1>
    <div class="container">
      <form action="/setState" method="get" style="width: 100%; display: flex;">
        <button class="button" name="state" value="standby">Standby</button>
        <button class="button" name="state" value="teach">Teach</button>
        <button class="button" name="state" value="operate">Operate</button>
      </form>
    </div>
  </body>
  </html>
)rawliteral";
