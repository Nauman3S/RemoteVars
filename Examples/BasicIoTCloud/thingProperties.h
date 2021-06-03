#include <RemoteVarsIoT.h>
#include <RemoteVarsConnectionHandler.h>
#if defined(BOARD_HAS_WIFI)
  #include "WiFi_RemoteVarsConnectionHandler.h"
#elif defined(BOARD_HAS_SERIAL)
  #include "Serial_RemoteVarsConnectionHandler.h"
#endif


#if defined(BOARD_HAS_WIFI)
 #define THING_ID "thing1"
 #define BOARD_ID "board1"
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
  BasicCloud.addProperty(led, Permission::Write).onUpdate(onLedChange);         //onLedChange is called when thing1/board1/pub/led is written to
  BasicCloud.addProperty(potentiometer, Permission::Read).publishOnChange(10);  //publish thing1/board1/pub/potentiometer at least every 10 seconds even if not changes
  BasicCloud.addProperty(seconds, Permission::Read).publishOnChange(1);
  BasicCloud.addProperty(dataStruct, Permission::Read).publishOnChange(1);
  BasicCloud.addProperty(remote_potentiometer, "board2", "potentiometer2", Permission::write).onUpdate(onRemotePotentiometerChange);
  //subscribe to thing1/board2/pub/potentiometer2 and call onRemotePotentiometerChange when a message is published
#elif defined(BOARD_HAS_SERIAL)
  BasicCloud.addProperty(led, 1, READWRITE, ON_CHANGE, onLedChange);  //use 1 instead of the variable name "led" to save bandwidth
  BasicCloud.addProperty(potentiometer, 2, READ, ON_CHANGE);          //same as https://github.com/arduino-libraries/ArduinoCloudThing
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
