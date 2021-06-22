//************"thingProperties.h"***************************//
#include <BasicIoTCloud.h>
#include <Basic_ConnectionHandler.h>

#if defined(BOARD_HAS_WIFI)
 #define THING_ID "xxxxxxxx"
 #define BOARD_ID "xxxxxxxx"
 #define SECRET_SSID "YOUR_WIFI_NETWORK_NAME"
 #define SECRET_PASS "YOUR_WIFI_PASSWORD"
 #define BROKER_IP "xxx.xxx.xxx.xxx"
 #define BROKER_PORT "xxxx"
 //this device will subscribe and publish on MQTT channels THING_ID/BOARD_ID/sub and THING_ID/BOARD_ID/pub respectively
#endif
#elif defined(BOARD_HAS_SERIAL)
 #define RX_PIN 2
 #define TX_PIN 3 
 #define BAUD 115200
 //this device will send and receive messages as if it were communicationg to an MQTT broker
#endif

#define JSON_Encoding MESSAGE_PACK

void onSwitchButtonChange();
void onLedChange();
void onRemotePotentiometerChange();

struct DataStruct
{
  int data1;
  float data2;
};

struct DataStruct dataStruct;
bool led;
int potentiometer;
int remote_potentiometer;
int seconds;

void initProperties() {
#if defined(BOARD_HAS_WIFI)
  BasicCloud.addProperty(led, Permission::Write).onUpdate(onLedChange);
  BasicCloud.addProperty(potentiometer, Permission::Read).publishOnChange(10);
  BasicCloud.addProperty(seconds, Permission::Read).publishOnChange(1);
  BasicCloud.addProperty(dataStruct, Permission::Read).publishOnChange(1);
  BasicCloud.addProperty(remote_potentiometer, "RemoteBoard_ID", "potentiometer2", Permission::write).onUpdate(onRemotePotentiometerChange);
  //this device will subscribe to THING_ID/RemoteBoard_ID/pub/potentiometer2 (or perhaps THING_ID/RemoteBoard_ID/pub and listen to changes to potentiometer2)
#elif defined(BOARD_HAS_SERIAL)
  BasicCloud.addProperty(led, 1, READWRITE, ON_CHANGE, onLedChange);
  BasicCloud.addProperty(potentiometer, 2, READ, ON_CHANGE);
  BasicCloud.addProperty(seconds, 3, READ, 5 * MINUTES);
  BasicCloud.addProperty(dataStruct, 4, READ, ON_CHANGE);
  //remote_potentiometer can not be used with serial
#endif
}

#if defined(BOARD_HAS_WIFI)
  WiFiConnectionHandler BasicIoTPreferredConnection(SECRET_SSID, SECRET_PASS);
#elif defined(BOARD_HAS_SERIAL)
  SerialConnectionHandler BasicIoTPreferredConnection(RX_PIN, TX_PIN, BAUD);
#endif

//****************************************************//

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

void onRemotePotentiometerChange() {
  Serial.print("RemotePotentiometer changed to ");
  Serial.println(remote_potentiometer);
}
