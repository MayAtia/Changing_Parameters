#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

const char* ssid = "Your SSID";
//const char* password = "Your Password";
ESP8266WebServer server(80);
WiFiClient client;
void handleNotFound() {
  String message = "File Not Foundnn";
  message += "URI: ";
  message += server.uri();
  message += "nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "nArguments: ";
  message += server.args();
  message += "n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleSliderValues() {
  // Grab the values from the sliders
  lightSensorDelay = server.arg("slider1").toInt() * 1000;
  relayDelay = server.arg("slider2").toInt() * 1000;
  pwmDelay = server.arg("slider3").toInt() * 1000;
  standbyDelay = server.arg("slider4").toInt() * 1000;

  HTTPClient http;
  http.begin(client, "http://IP_Address:3000/post_values"); //Specify theIP_Address
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String httpRequestData = "lightSensorDelay=" + String(lightSensorDelay)
                           + "&relayDelay=" + String(relayDelay)
                           + "&pwmDelay=" + String(pwmDelay)
                           + "&standbyDelay=" + String(standbyDelay);
  int httpResponseCode = http.POST(httpRequestData);

  http.end();
  server.send(200, "text/plain", httpResponseCode == HTTP_CODE_OK ? "Data sent to server" : "Error sending data to server");
}

void wifi_loop() {
  server.handleClient();
  delay(10);
}
void wifi_setup() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(ssid, password);
  // WiFi.begin(ssid); //If you don't have a password, specify it
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/getSliderValues", handleSliderValues);
  server.onNotFound(handleNotFound);
  server.begin();
}

void handleRoot() {
  String html = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
.slidecontainer {
  width: 100%;
}

.slider {
  -webkit-appearance: none;
  width: 100%;
  height: 10px;
  border-radius: 5px;
  background: #d3d3d3;
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
  width: 23px;
  height: 23px;
  border: 0;
  border-radius: 50%;
  background: #4CAF50;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 23px;
  height: 23px;
  border: 0;
  border-radius: 50%;
  background: #4CAF50;
  cursor: pointer;
}
</style>
</head>
<body>

<h1>Time Set Sliders</h1>

<div class="slidecontainer">
  <h3>Light Sensor</h3>
  <input type="range" min="2" max="60" value="2" class="slider" id="myRange1">
  <p>Value: <span id="demo1"></span></p>
  
  <h3>Relay</h3>
  <input type="range" min="2" max="60" value="2" class="slider" id="myRange2">
  <p>Value: <span id="demo2"></span></p>

  <h3>PWM</h3>
  <input type="range" min="2" max="60" value="2" class="slider" id="myRange3">
  <p>Value: <span id="demo3"></span></p>

  <h3>Standby</h3>
  <input type="range" min="2" max="60" value="2" class="slider" id="myRange4">
  <p>Value: <span id="demo4"></span></p>

  <button id="settingBtn">change settings</button>
  <button id="submitBtn">Submit to database</button>

  <button id="showRecentBtn">Show Recent Settings</button>

  <button id="deleteBtn">Delete Last Entry</button>

</div>

<script>
var apiUrl = "http://192.168.68.107:3000/";
var slider1 = document.getElementById("myRange1");
var output1 = document.getElementById("demo1");
output1.innerHTML = slider1.value;

slider1.oninput = function() {
  output1.innerHTML = this.value;
}

var slider2 = document.getElementById("myRange2");
var output2 = document.getElementById("demo2");
output2.innerHTML = slider2.value;

slider2.oninput = function() {
  output2.innerHTML = this.value;
}

var slider3 = document.getElementById("myRange3");
var output3 = document.getElementById("demo3");
output3.innerHTML = slider3.value;

slider3.oninput = function() {
  output3.innerHTML = this.value;
}

var slider4 = document.getElementById("myRange4");
var output4 = document.getElementById("demo4");
output4.innerHTML = slider4.value;

slider4.oninput = function() {
  output4.innerHTML = this.value;
}
document.getElementById("showRecentBtn").onclick = function() {
    var url =apiUrl+ "get_recent_json";

    fetch(url)
    .then((response) => {
        console.log('Response received:', response);
        return response.json();  // Read the response body
    })
    .then((data) => {
        console.log('Data received:', data);

        // Update the sliders with the fetched data
        slider1.value = data.lightSensorDelay / 1000;
        output1.innerHTML = slider1.value;
        
        slider2.value = data.relayDelay / 1000;
        output2.innerHTML = slider2.value;
        
        slider3.value = data.pwmDelay / 1000;
        output3.innerHTML = slider3.value;
        
        slider4.value = data.standbyDelay / 1000;
        output4.innerHTML = slider4.value;

        window.open(apiUrl+'get_recent', '_blank');
    })
    .catch((error) => {
        console.log('Error fetching data:', error);
    });
}
document.getElementById("submitBtn").onclick = function() {
  var url = apiUrl+"post_values ";
  
  var data = {
    slider1: slider1.value,
    slider2: slider2.value,
    slider3: slider3.value,
    slider4: slider4.value
  };

  fetch(url, {
    method: "POST",
    headers: {
      'Content-Type': 'application/x-www-form-urlencoded'
    },
    body: new URLSearchParams(data)
  })
  .then((response) => {
    console.log('Response received:', response);
    return response.text();  // Read the response body
  })
  .then((data) => {
    console.log('Data received:', data);
  })
  .catch((error) => {
    console.log('Error sending data to server:', error);
  });
}
document.getElementById("settingBtn").onclick = function() {
  var url = `/getSliderValues?slider1=${slider1.value}&slider2=${slider2.value}&slider3=${slider3.value}&slider4=${slider4.value}`;

  console.log("Submit button clicked. Sending GET request with slider values.");
  console.log(`Slider values: ${url.substring(17)}`);

  fetch(url)
  .then((response) => {
    console.log('Response received:', response);
    return response.text();  // Read the response body
  })
  .then((data) => {
    console.log('Data received:', data);
  })
  .catch((error) => {
    console.log('Error sending data to server:', error);
  });
}

    document.getElementById("deleteBtn").onclick = function() {
      var url =apiUrl+"delete_last";

      fetch(url, {
        method: "DELETE"
      })
      .then((response) => {
        console.log('Response received:', response);
        return response.json();
      })
      .then((data) => {
        console.log('Data received:', data);
        alert('Last entry deleted successfully!');
      })
      .catch((error) => {
        console.log('Error deleting data:', error);
      });
    };

</script>

</body>
</html>

)=====";
  server.send(200, "text/html", html);
}
