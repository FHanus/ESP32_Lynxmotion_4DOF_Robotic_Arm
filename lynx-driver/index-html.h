#ifndef INDEX_HTML_H
#define INDEX_HTML_H

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
      user-select: none;
    }
    .button:hover {
      background-color: #006f9b;
    }
    .container, .gripper {
      width: 100%;
      box-sizing: border-box;
    }
    .state-buttons {
      display: flex;
      width: 100%;
    }
    .state-buttons .button {
      flex: 1;
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
    .joint-angles {
      text-align: center;
      font-size: 20px;
      margin: 10px;
    }
  </style>
    <script>
      var intervalId;

      function sendAction(action) {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "/control?action=" + action, true);
        xhttp.send();
      }

      function startSending(action) {
        sendAction(action);
        intervalId = setInterval(function() {
          sendAction(action);
        }, 100); // Send action every 100ms while button is held
      }

      function stopSending() {
        clearInterval(intervalId);
        sendAction("stop"); // Notify server to stop the action
      }

      // Function to fetch and display joint angles
      function fetchJointAngles() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            var angles = JSON.parse(this.responseText);
            document.getElementById("theta1").innerText = angles.theta1;
            document.getElementById("theta2").innerText = angles.theta2;
            document.getElementById("theta3").innerText = angles.theta3;
            document.getElementById("theta4").innerText = angles.theta4;
            document.getElementById("thetaEndEffector").innerText = angles.thetaEndEffector;
          }
        };
        xhttp.open("GET", "/getJointAngles", true);
        xhttp.send();
      }

      // Fetch joint angles every second
      setInterval(fetchJointAngles, 1000);
    </script>
</head>
<body>
  <h1>Lynx Robot Control</h1>

  <div class="joint-angles">
    <p>Theta1: <span id="theta1">0</span>°</p>
    <p>Theta2: <span id="theta2">0</span>°</p>
    <p>Theta3: <span id="theta3">0</span>°</p>
    <p>Theta4: <span id="theta4">0</span>°</p>
    <p>Theta End Effector: <span id="thetaEndEffector">0</span>°</p>
  </div>

  <div class="container">
    <form action="/setState" method="get" class="state-buttons">
      <button class="button" name="state" value="standby">Standby</button>
      <button class="button" name="state" value="teach">Teach</button>
      <button class="button" name="state" value="operate">Operate</button>
    </form>
  </div>

  <div class="gripper">
    <form action="/setGripperState" method="get" class="state-buttons">
      <button class="button" name="gripper" value="open">Open Gripper</button>
      <button class="button" name="gripper" value="close">Close Gripper</button>
    </form>
  </div>

  <div class="controller">
    <!-- Updated buttons with event handlers -->
    <div class="row">
      <button class="button" onmousedown="startSending('up')" onmouseup="stopSending()" ontouchstart="startSending('up')" ontouchend="stopSending()">Up</button>
    </div>
    <div class="middle-row">
      <button class="button" onmousedown="startSending('left')" onmouseup="stopSending()" ontouchstart="startSending('left')" ontouchend="stopSending()">Left</button>
      <div class="column">
        <button class="button" onmousedown="startSending('forward')" onmouseup="stopSending()" ontouchstart="startSending('forward')" ontouchend="stopSending()">Forward</button>
        <button class="button" onmousedown="startSending('backward')" onmouseup="stopSending()" ontouchstart="startSending('backward')" ontouchend="stopSending()">Backward</button>
      </div>
      <button class="button" onmousedown="startSending('right')" onmouseup="stopSending()" ontouchstart="startSending('right')" ontouchend="stopSending()">Right</button>
    </div>
    <div class="row">
      <button class="button" onmousedown="startSending('down')" onmouseup="stopSending()" ontouchstart="startSending('down')" ontouchend="stopSending()">Down</button>
    </div>
  </div>
</body>
</html>
)rawliteral";

#endif // INDEX_HTML_H
