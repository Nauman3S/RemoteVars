#include <stdio.h>
#include <string.h>
#define DEVM 0
class CommunicationHandler{

    public:
    CommunicationHandler();
    ~CommunicationHandler();
    

        String readJSONString();//read a JSON string as a char array starts with '{' and ends with '}'
        void sendJSONString(String str);//sends a JSON string as a char array


};

CommunicationHandler::CommunicationHandler()
{
    printf("Initializing Communication Handler");
}
CommunicationHandler::~CommunicationHandler()
{
    printf("Clearing up Communication Handler");
}
void CommunicationHandler::sendJSONString(String str){
    char startW='{';
    char endW='}';
    //check if the JSON is valid
//    if(str.c_str()[0]==startW && str[strlen(str.c_str())-1]){
    #if SHOW_DEBUG_MESSAGES==0
        // SERIAL_PORT_MONITOR.print("COMMHANDLE HERE SENDING ");
        // SERIAL_PORT_MONITOR.println(str);
    #else
        Serial.print("Publishing Data: ");
        Serial.println(str);
    #endif

        //send to serial,wifi,rf etc
  //  }
}

String CommunicationHandler::readJSONString(){
    //printf("COMMHANDLE AS A READER");
    char startW='{';
    char endW='}';
    String incommingJSON="";
    //read from serial, wifi, rf etc from '{'  to '}' and store it in the variable above
    return incommingJSON;
    
}