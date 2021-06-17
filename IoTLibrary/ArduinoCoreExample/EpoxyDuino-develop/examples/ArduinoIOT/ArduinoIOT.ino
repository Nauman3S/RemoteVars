/*
 * This is a Test file for IoTLibrary
 *    ...
 */

#include <Arduino.h>
//#include "ArduinoJSON.h"
#include "IoTLibrary.h"

#define LED LED_BUILTIN


void PublishCallback(){
    //printf("\n\nData Published\n\n");
}

int LEDValue=0;
void SubscribeCallBack(String topic, String data){
    //printf("\n\nData Received\n\n");
    SERIAL_PORT_MONITOR.println(topic);
    SERIAL_PORT_MONITOR.println(data);
    if(topic.indexOf("SubTopicLED")>=0){
      LEDValue=data.toInt();
    }
}

CommunicationHandler myCommHandle;//intermediate communication handler

IoTLibrary al(&myCommHandle);
int firstValue=12;
double secondValue=32.132;
String thirdValue="HERE is a string";
String fourthValue="data from cloud";
int variableT=0;

void setup() {
  #if ! defined(EPOXY_DUINO)
  delay(1000); // some boards reboot twice
#endif
  SERIAL_PORT_MONITOR.begin(115200);
  while (!SERIAL_PORT_MONITOR); // For Leonardo/Micro

  SERIAL_PORT_MONITOR.print(F("LED_BUILTIN: "));
  SERIAL_PORT_MONITOR.println(LED_BUILTIN);
  SERIAL_PORT_MONITOR.print(F("LED: "));
  SERIAL_PORT_MONITOR.println(LED);

  pinMode(LED, OUTPUT);
  
  
    al.addProperty(&firstValue,String("Topic1"), PERMISSIONS::WRITE_TO_LOCAL, EVENTS::ON_LOCAL_CHANGE,METHODS::PUBLISH, PublishCallback);
    al.addProperty(&secondValue,String("Topic2"), PERMISSIONS::WRITE_TO_LOCAL, EVENTS::ON_LOCAL_CHANGE,METHODS::PUBLISH, PublishCallback);
    al.addProperty(&thirdValue,String("Topic3"), PERMISSIONS::WRITE_TO_LOCAL, EVENTS::ON_LOCAL_CHANGE,METHODS::PUBLISH, PublishCallback);

    al.addProperty(&variableT,String("Topic4"), PERMISSIONS::WRITE_TO_LOCAL, EVENTS::ON_LOCAL_CHANGE,METHODS::PUBLISH);

    al.addProperty(String("TOPICSub"),&fourthValue,PERMISSIONS::READ_FROM_CLOUD,EVENTS::ON_CLOUD_CHANGE,METHODS::SUBSCRIBE,SubscribeCallBack);
  
    
    
    SERIAL_PORT_MONITOR.println( al.ShowUniversalDoc());

    

}
// void SubscriptionsLoop(){
//   al.addProperty(METHODS::SUBSCRIBE,String("SubTopicLED"),SubscribeCallBack);
// }

void LEDHandler() {
  LEDValue=fourthValue.toInt();
  digitalWrite(LED, LEDValue);
  delay(200);
  
}

void loop() {

  
   al.loopIoTLibrary();//required
   
  // SubscriptionsLoop();
  delay(1500);
  firstValue++;
  secondValue=secondValue+0.2;
  thirdValue=thirdValue+String("-");
}
