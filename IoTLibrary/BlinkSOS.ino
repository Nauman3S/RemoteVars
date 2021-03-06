/*
 * This is a Test file for IoTLibrary
 *    ...
 */

#include <Arduino.h>
#include "IoTLibrary.h"

#define LED LED_BUILTIN


void PublishCallback(){
    printf("\n\nData Published\n\n");
}

int LEDValue=0;
void SubscribeCallBack(String topic, String data){
    printf("\n\nData Received\n\n");
    SERIAL_PORT_MONITOR.println(topic);
    SERIAL_PORT_MONITOR.println(data);
    if(topic.indexOf("SubTopicLED")>=0){
      LEDValue=data.toInt();
    }
}

CommunicationHandler myCommHandle;//intermediate communication handler

IoTLibrary al(&myCommHandle);

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
  



    int firstValue=12;
    al.addProp(firstValue,String("Topic1"), PERMISSIONS::WRITE, EVENTS::ON_CHANGE,METHODS::PUBLISH, PublishCallback);
    //char * strData="String Data";
    String strData=String("String Data");
    al.addProp(strData,String("Topic2"), PERMISSIONS::WRITE, EVENTS::ON_CHANGE,METHODS::PUBLISH, PublishCallback);

    
    
    al.ShowUniversalDoc();

    

}
void SubscriptionsLoop(){
  al.addProp(METHODS::SUBSCRIBE,String("SubTopicLED"),SubscribeCallBack);
}

void LEDHandler() {
  digitalWrite(LED, LEDValue);
  delay(200);
  
}

void loop() {
  SERIAL_PORT_MONITOR.println(F("SOS"));
  int dataValue=12;
  String returnV=al.publishData(String("Topic1"),dataValue);
  SERIAL_PORT_MONITOR.println(returnV.c_str());

  String strDataValue=String("this is a string data");
  String returnV2=al.publishData(String("Topic2"),strDataValue);
  SERIAL_PORT_MONITOR.println(returnV2.c_str());

  LEDHandler();

  
  al.loopIoTLibrary();//required
  SubscriptionsLoop();
  delay(10);
}
