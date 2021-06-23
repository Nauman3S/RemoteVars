#include <stdio.h>
#include <string.h>
//#include "ArduinoJSON.h"
#define DEVM 0
class CommunicationHandler{

    public:
    CommunicationHandler();
    ~CommunicationHandler();
    
    void PublishToMQTT(String topic, String data);

    String readJSONString();//read a JSON string as a char array starts with '{' and ends with '}'
    void sendJSONString(String str);//sends a JSON string as a char array
    private:
    JSONHandler jsonH;

};

CommunicationHandler::CommunicationHandler()
{
    //Serial.println("Initializing Communication Handler");
}
CommunicationHandler::~CommunicationHandler()
{
  //  Serial.println("Clearing up Communication Handler");
}

void CommunicationHandler::PublishToMQTT(String topic, String data){
    #if DEVMODE==1
    Serial.print("Publishing Topic=");
    
    #endif
    //publishing logic here
}
void CommunicationHandler::sendJSONString(String str){
    
    //check if the JSON is valid
//    if(str.c_str()[0]==startW && str[strlen(str.c_str())-1]){
    #if DEVMODE==0
        // SERIAL_PORT_MONITOR.print("COMMHANDLE HERE SENDING ");
        // SERIAL_PORT_MONITOR.println(str);
    #else
        Serial.print("Publishing Data: ");
        Serial.println(str);//encoding handled by Serial
    #endif
    if(str.indexOf("}")>=0 && str.indexOf("{")>=0){//check integrity before sending
    Serial.println(str);
    PublishToMQTT(jsonH.extractTopic(str),jsonH.extractData(str));
    }
        //send to serial,wifi,rf etc
  //  }
}

String CommunicationHandler::readJSONString(){
    //printf("COMMHANDLE AS A READER");
    char startW='{';
    char endW='}';
    String incommingJSON="";
    incommingJSON=String(Serial.readString());
    if(incommingJSON.indexOf("}")>=0 && incommingJSON.indexOf("{")>=0){
        return incommingJSON;//no need of managing topics, it will be handled by iotlibrary
    }

    //read from serial, wifi, rf etc from '{'  to '}' and store it in the variable above
    
    
}