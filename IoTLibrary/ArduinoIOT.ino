/*
 * This is a Example program for IoTLibrary
 *    ...
 */

#define SHOW_DEBUG_MESSAGES 0

#include <Arduino.h>
#include "IoTLibrary.h"

#define LED LED_BUILTIN

#define SERIAL_PORT_MONITOR Serial


void PublishCallback(){
    //will be called on each data pulbish
    //can be used to construct some custom logic
}

int remote_potentiometer=0;

void SubscribeCallBack(String topic, String data){
    //data from the subscribed topics will be recieved here
    SERIAL_PORT_MONITOR.print("Received Data on Callback: Topic=");
    SERIAL_PORT_MONITOR.print(topic);
    SERIAL_PORT_MONITOR.print(" Data=");
    SERIAL_PORT_MONITOR.println(data);
    if(topic.indexOf("remote_potentiometer")>=0){
      remote_potentiometer=data.toInt();
    }
}
//Communication handler library
CommunicationHandler myCommHandle;//intermediate communication handler
IoTLibrary al(&myCommHandle);//attach it with IoTLibrary to pass the data 
///

//Local variables. Values of these are published on the topics defined in the setup function
int firstValue=12;
double secondValue=32.132;
String thirdValue="HERE is a string "+String(firstValue);
String fourthValue="data from cloud";
int variableT=99;
//

void setup() {
  
  SERIAL_PORT_MONITOR.begin(115200);
  while (!SERIAL_PORT_MONITOR); // For Leonardo/Micro

  SERIAL_PORT_MONITOR.println( "asd");
  
  al.addProperty(String("Topic1"), PERMISSIONS::WRITE_TO_LOCAL, EVENTS::ON_LOCAL_CHANGE,METHODS::PUBLISH, PublishCallback,&firstValue);
  al.addProperty(String("Topic2"), PERMISSIONS::WRITE_TO_LOCAL, EVENTS::ON_LOCAL_CHANGE,METHODS::PUBLISH, PublishCallback,&secondValue);
  al.addProperty(String("Topic3"), PERMISSIONS::WRITE_TO_LOCAL, EVENTS::ON_LOCAL_CHANGE,METHODS::PUBLISH, PublishCallback,&thirdValue);
  al.addProperty(String("Topic4"), PERMISSIONS::WRITE_TO_LOCAL, EVENTS::ON_LOCAL_CHANGE,METHODS::PUBLISH,&variableT);


  //subscribe to a topic
  al.addProperty(String("remote_potentiometer"),PERMISSIONS::READ_FROM_CLOUD,EVENTS::ON_CLOUD_CHANGE,METHODS::SUBSCRIBE,SubscribeCallBack);

  
  
  SERIAL_PORT_MONITOR.println( al.ShowUniversalDoc());//shows the list of topics in use

    

}




void loop() {

  
  al.loopIoTLibrary();//required
   
  delay(1500);//should be as minimum as possible
  
  secondValue=secondValue+0.2;
  thirdValue.replace(String(firstValue), String(firstValue+1));
  firstValue++;
}
