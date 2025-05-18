#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

#define EEPROM_SIZE 512

const char* ssid = "Home Network";
const char* password = "Idontkn0w";

ESP8266WebServer server(80);

const int pinCount = 8;
const int pinNumbers[pinCount] = {16, 5, 4, 0, 2, 14, 12, 13};

String jsonData = "";
String hostname = "esp8266";
String title = "ESP8266 Control Panel";
String pinLabels[pinCount];
bool pinEnabled[pinCount];

void saveJSONToEEPROM(const String& data) {
  for (int i = 0; i < data.length() && i < EEPROM_SIZE - 1; ++i)
    EEPROM.write(i, data[i]);
  EEPROM.write(data.length(), '\0');
  EEPROM.commit();
}

void loadJSONFromEEPROM() {
  jsonData = "";
  for (int i = 0; i < EEPROM_SIZE; i++) {
    char c = EEPROM.read(i);
    if (c == '\0') break;
    jsonData += c;
  }
}

void parseJSONConfig() {
  StaticJsonDocument<512> doc;
  DeserializationError err = deserializeJson(doc, jsonData);
  if (err) {
    Serial.println("Failed to parse JSON config.");
    return;
  }

  hostname = doc["hostname"] | "esp8266";
  title = doc["title"] | "ESP8266 Control Panel";
  JsonObject pins = doc["pinconfig"].as<JsonObject>();

  for (int i = 0; i < pinCount; i++) {
    String key = "D" + String(i);
    if (pins.containsKey(key) && pins[key].as<String>() != "") {
      pinLabels[i] = pins[key].as<String>();
      pinEnabled[i] = true;
    } else {
      pinLabels[i] = "";
      pinEnabled[i] = false;
    }
  }
}

int readPins() {
  byte status = 0;
  for (int i = 0; i < pinCount; i++) {
    if (pinEnabled[i]) {
      status |= (digitalRead(pinNumbers[i]) << i);
    }
  }
  return status;
}

void draw_table() {
  int status = readPins();

  String html = R"rawliteral(
<html><head><title>)rawliteral" + title + R"rawliteral(</title><style>
.powerbutton{width:100px;height:100px;border-radius:50px;color:white;}
.button_ON{background-color:green;}
.button_OFF{background-color:red;}
</style></head><body><center><h1>)rawliteral" + title + R"rawliteral(</h1>
<form id='myform' action='' method='post'><table id='my_table'></table></form>
</center><script>
var buttons_status=)rawliteral" + String(status) + R"rawliteral(;var button_labels={)rawliteral";

  for (int i = 0; i < pinCount; i++) {
    if (pinEnabled[i]) {
      html += String(i) + ":'" + pinLabels[i] + "',";
    }
  }

  html += R"rawliteral(};
function update_target(id,val){document.getElementById('myform').action='/set?'+id+'='+val;document.getElementById('myform').submit();}
function set_button_value(btn,id,val){
  btn.name='D'+id;btn.id='D'+id;
  var x=document.createElement('span');x.innerText=id+' - '+val;
  btn.appendChild(x);
  if((val & (1<<id))>0){
    btn.onclick=function(){update_target(this.id,0)};
    btn.value='ON';btn.className='button_ON powerbutton';
  } else {
    btn.onclick=function(){update_target(this.id,1)};
    btn.value='OFF';btn.className='button_OFF powerbutton';
  }
}
function draw_table(){
  var table=document.getElementById('my_table');
  for(var id in button_labels){
    if(!button_labels[id]) continue;
    var row=table.insertRow();
    var col1=row.insertCell();col1.innerText=button_labels[id];
    var col2=row.insertCell();
    var btn=document.createElement('input');btn.type='button';
    set_button_value(btn,id,buttons_status);
    col2.appendChild(btn);
  }
}
draw_table();
</script></body></html>)rawliteral";

  server.send(200, "text/html", html);
}

void setPins() {
  for (int i = 0; i < pinCount; i++) {
    String key = "D" + String(i);
    if (server.hasArg(key) && pinEnabled[i]) {
      int val = server.arg(key).toInt();
      digitalWrite(pinNumbers[i], val ? HIGH : LOW);
      break;
    }
  }
  draw_table();
}

void handleUploadPage() {
  String page = R"rawliteral(
<html><body><h2>Upload JSON Config</h2><form action='/upload' method='post'>
<textarea name='json' rows='20' cols='60'>)rawliteral";
  page += jsonData;
  page += R"rawliteral(</textarea><br>
<input type='submit' value='Upload'></form>
<p><a href='/'>Back</a></p></body></html>)rawliteral";
  server.send(200, "text/html", page);
}

void handleUploadJSON() {
  if (server.hasArg("json")) {
    jsonData = server.arg("json");
    saveJSONToEEPROM(jsonData);
    parseJSONConfig();
    server.send(200, "text/html", R"rawliteral(<h3>JSON saved!</h3><p><a href='/'>Back</a></p>)rawliteral");
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Missing JSON data");
  }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  loadJSONFromEEPROM();
  parseJSONConfig();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi connected: " + WiFi.localIP().toString());

  if (MDNS.begin(hostname.c_str())) {
    Serial.println("MDNS started: http://" + hostname + ".local");
  }

  for (int i = 0; i < pinCount; i++) {
    pinMode(pinNumbers[i], OUTPUT);
    digitalWrite(pinNumbers[i], LOW);
  }

  server.on("/", draw_table);
  server.on("/set", setPins);
  server.on("/upload", HTTP_GET, handleUploadPage);
  server.on("/upload", HTTP_POST, handleUploadJSON);
  server.begin();
}

void loop() {
  MDNS.update();
  server.handleClient();
}
