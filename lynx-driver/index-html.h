#ifndef INDEX_HTML_H
#define INDEX_HTML_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Lynx Robot Control (F.H.)</title>
  <style>
    body, html {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
      font-family: Arial, sans-serif;
      background-color: #f0f0f0;
    }
    h1 {
      text-align: center;
      font-size: 34px;
      margin-top: 20px;
    }
    .container, .gripper {
      width: 90%;
      margin: 0 auto;
      box-sizing: border-box;
      margin-bottom: 20px;
    }
    .state-buttons {
      display: flex;
      width: 100%;
      margin-bottom: 20px;
      /* Consider changing this to center to avoid spacing around a single button */
      justify-content: center;
    }
    .button {
      padding: 15px 25px;
      text-align: center;
      font-size: 18px;
      background-color: #008CBA;
      color: white;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      margin: 5px;
      user-select: none;
      flex: 1; /* This allows the button to expand to fill the width if it's the only element */
    }
    .button:hover {
      background-color: #006f9b;
    }
    .button.selected {
      background-color: #005f7f; 
    }
    .servo-control {
      margin: 20px auto;
      display: flex;
      align-items: center;
      justify-content: center;
      width: 90%;
    }
    .servo-control label {
      width: 120px;
      font-size: 18px;
      margin-right: 10px;
    }
    .servo-buttons {
      display: flex;
    }
    .servo-button {
      padding: 10px 20px;
      margin: 0 5px;
      background-color: #008CBA;
      color: white;
      border: none;
      border-radius: 5px;
      cursor: pointer;
    }
    .servo-button:hover {
      background-color: #006f9b;
    }
    .servo-button.active {
      background-color: #005f7f; 
    }
  </style>
  <script>
    var currentState = "%CURRENT_STATE%";
    var currentGripperState = "%CURRENT_GRIPPER_STATE%";

    function highlightCurrentState() {
      var stateButtons = {
        'standby': document.getElementById('standbyButton'),
        'teach': document.getElementById('teachButton'),
        'operate': document.getElementById('operateButton')
      };
      if (currentState in stateButtons) {
        stateButtons[currentState].classList.add('selected');
      }

      var gripperButtons = {
        'open': document.getElementById('openGripperButton'),
        'close': document.getElementById('closeGripperButton')
      };
      if (currentGripperState in gripperButtons) {
        gripperButtons[currentGripperState].classList.add('selected');
      }
    }

    var servoIntervals = {};

    function sendServoAction(servoId, action) {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "/servoControl?servo=" + servoId + "&action=" + action, true);
      xhttp.send();
    }

    function startServoAction(servoId, action, button) {
      sendServoAction(servoId, action);
      button.classList.add('active');
      servoIntervals[servoId + action] = setInterval(function() {
        sendServoAction(servoId, action);
      }, 100);
    }

    function stopServoAction(servoId, action, button) {
      clearInterval(servoIntervals[servoId + action]);
      button.classList.remove('active');
      sendServoAction(servoId, 'stop');
    }

    window.onload = function() {
      highlightCurrentState();
    };
  </script>
</head>
<body>
  <h1>Lynx Robot Control</h1>

  <div class="container">
    <form action="/setState" method="get" class="state-buttons">
      <button id="standbyButton" class="button" name="state" value="standby">Standby</button>
      <button id="teachButton" class="button" name="state" value="teach">Teach</button>
      <button id="operateButton" class="button" name="state" value="operate">Operate</button>
    </form>
  </div>

  <div class="gripper">
    <form action="/setGripperState" method="get" class="state-buttons">
      <button id="openGripperButton" class="button" name="gripper" value="open">Open Gripper</button>
      <button id="closeGripperButton" class="button" name="gripper" value="close">Close Gripper</button>
    </form>
  </div>

  <div class="container">
    <!-- Make this also a state-buttons container to match styling -->
    <form action="/recordPosition" method="get" class="state-buttons">
      <button class="button" type="submit">Save Waypoint</button>
    </form>
  </div>

  <div class="controller">
    <div class="servo-control">
      <label for="servo1">Servo 1:</label>
      <div class="servo-buttons">
        <button id="servo1-decrease" class="servo-button"
          onmousedown="startServoAction(1, 'decrease', this)" onmouseup="stopServoAction(1, 'decrease', this)" 
          ontouchstart="startServoAction(1, 'decrease', this)" ontouchend="stopServoAction(1, 'decrease', this)">
          -
        </button>
        <button id="servo1-increase" class="servo-button"
          onmousedown="startServoAction(1, 'increase', this)" onmouseup="stopServoAction(1, 'increase', this)" 
          ontouchstart="startServoAction(1, 'increase', this)" ontouchend="stopServoAction(1, 'increase', this)">
          +
        </button>
      </div>
    </div>
    <div class="servo-control">
      <label for="servo2">Servo 2:</label>
      <div class="servo-buttons">
        <button id="servo2-decrease" class="servo-button"
          onmousedown="startServoAction(2, 'decrease', this)" onmouseup="stopServoAction(2, 'decrease', this)" 
          ontouchstart="startServoAction(2, 'decrease', this)" ontouchend="stopServoAction(2, 'decrease', this)">
          -
        </button>
        <button id="servo2-increase" class="servo-button"
          onmousedown="startServoAction(2, 'increase', this)" onmouseup="stopServoAction(2, 'increase', this)" 
          ontouchstart="startServoAction(2, 'increase', this)" ontouchend="stopServoAction(2, 'increase', this)">
          +
        </button>
      </div>
    </div>
    <div class="servo-control">
      <label for="servo3">Servo 3:</label>
      <div class="servo-buttons">
        <button id="servo3-decrease" class="servo-button"
          onmousedown="startServoAction(3, 'decrease', this)" onmouseup="stopServoAction(3, 'decrease', this)" 
          ontouchstart="startServoAction(3, 'decrease', this)" ontouchend="stopServoAction(3, 'decrease', this)">
          -
        </button>
        <button id="servo3-increase" class="servo-button"
          onmousedown="startServoAction(3, 'increase', this)" onmouseup="stopServoAction(3, 'increase', this)" 
          ontouchstart="startServoAction(3, 'increase', this)" ontouchend="stopServoAction(3, 'increase', this)">
          +
        </button>
      </div>
    </div>
    <div class="servo-control">
      <label for="servo4">Servo 4:</label>
      <div class="servo-buttons">
        <button id="servo4-decrease" class="servo-button"
          onmousedown="startServoAction(4, 'decrease', this)" onmouseup="stopServoAction(4, 'decrease', this)" 
          ontouchstart="startServoAction(4, 'decrease', this)" ontouchend="stopServoAction(4, 'decrease', this)">
          -
        </button>
        <button id="servo4-increase" class="servo-button"
          onmousedown="startServoAction(4, 'increase', this)" onmouseup="stopServoAction(4, 'increase', this)" 
          ontouchstart="startServoAction(4, 'increase', this)" ontouchend="stopServoAction(4, 'increase', this)">
          +
        </button>
      </div>
    </div>
  </div>
</body>
</html>
)rawliteral";

#endif // INDEX_HTML_H
