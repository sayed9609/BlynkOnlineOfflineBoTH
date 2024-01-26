//Replace with your own blynk credentials
#define BLYNK_TEMPLATE_ID "TMPL6oqHrRr3n"
#define BLYNK_TEMPLATE_NAME "Smart Switch"
#define BLYNK_AUTH_TOKEN "cQjOEoklmlTrdZt3DuIyOeOMQRfHVcZq"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//server status led
#define online 5

//buttons and led pins
#define button 4

#define led1 23

int switchState = 0;     // actual read value from pin4
int oldSwitchState = 0;  // last read value from pin4

int lightsOn = 0;

//server state | flag for online/Offline Mode
bool onlineMode = false;

//Wifi config
char ssid[] = "Smart Switch";
char pass[] = "12345678";

BlynkTimer timer;

//
void isServerConnected() {
  bool isConnected = Blynk.connected();
  if (isConnected == true) {
    onlineMode = true;
    digitalWrite(online, HIGH);
    Serial.println("Connected");
  }
  if (isConnected == false) {
    if (onlineMode == true) {
      onlineMode = false;
    }
    Serial.println("Not Connected");
    digitalWrite(online, LOW);
  }
}

void setup() {
  Serial.begin(115200);

  timer.setInterval(2000L, isServerConnected);

  pinMode(online, OUTPUT);

  pinMode(led1, OUTPUT);

  pinMode(button, INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  //Blynk.run();
  timer.run();
  switchState = digitalRead(button);

  if (onlineMode) {
    Blynk.run();
    whenOnline();  //handles online functionalities
  } else {
    whenOffline();  //handles offline functionalities
  }
}

void whenOnline() {
  if (switchState != oldSwitchState)  // catch change
  {
    oldSwitchState = switchState;
    if (switchState == HIGH) {
      // toggle
      lightsOn = !lightsOn;
      updateLEDs();
      updateBlynkServer();
      delay(50);
    }
  }
}

void whenOffline() {
  if (switchState != oldSwitchState)  // catch change
  {
    oldSwitchState = switchState;
    if (switchState == HIGH) {
      // toggle
      lightsOn = !lightsOn;
      updateLEDs();
      delay(50);
    }
  }
}

void updateLEDs() {
  digitalWrite(led1, lightsOn);
}

void updateBlynkServer() {
  Blynk.virtualWrite(V0, lightsOn);
}

BLYNK_WRITE(V0) {
  lightsOn = param.asInt();
  Serial.println(lightsOn);
  updateLEDs();
}