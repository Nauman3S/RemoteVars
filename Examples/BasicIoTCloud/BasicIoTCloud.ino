#include "thingProperties.h"

void setup() {
  Serial.begin(9600);
  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }
  pinMode(LED_BUILTIN, OUTPUT);
  initProperties();

  BasicCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(DBG_INFO);
  BasicCloud.printDebugInfo();
}

void loop() {
  potentiometer = analogRead(A0);
  seconds = millis() / 1000;
  dataStruct.data1 = millis()/1000;
  BasicCloud.update();
}

void onLedChange() {
  Serial.print("LED set to ");
  Serial.println(led);
  digitalWrite(LED_BUILTIN, led);
}

//this is called by RemoteVars when board2 publishes potentiometer2 on the thing1/board2/pub/potentiometer2 topic
void onRemotePotentiometerChange() {
  Serial.print("RemotePotentiometer changed to ");
  Serial.println(remote_potentiometer);
}
