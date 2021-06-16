#include <iostream>
#include <stdio.h>
#include "const_values.h"
#include "consts.h"
#include "ArduinoJSON.h"
#include "CommunicationHandler.h"

#define DEVMODE 1 //dev mode helper
#define STRING_EMU 0 //arduino string object emulator

#if STRING_EMU==1
class String{
    public:
    char * val;
     String() {
         val = "";
         
      }
      String(char * f) {
         val = f;
      }
     String operator= (const String &S){
        val=S.val;
        printf("\n%s",val);
    }
};
#endif
class IoTLibrary 
{
  public:

    IoTLibrary(CommunicationHandler *ch){
        UniversalCommHandle=ch;
    }

    //publish prototypes
    uint8_t addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void PubCallback (void)); //simple callback function prototype
    uint8_t addProp(float localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void PubCallback (void)); //simple callback function prototype
    uint8_t addProp(char* localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void PubCallback (void)); //simple callback function prototype
   // uint8_t addProp(String localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void callback (void)); //simple callback function prototype
    
    //subscribe prototypes
    uint8_t addProp( uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void SubCallback (int), int arg); //simple callback function prototype

    uint8_t addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void callback (int), int arg); //int callback
    uint8_t addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void callback (float), float arg); //float callback
    uint8_t addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void callback (String), String arg); //string object callback
    
    uint8_t addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, int callback (int), int arg); //int callback
    uint8_t addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, float callback (float), float arg); //float callback
    uint8_t addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, String callback (String), String arg); //string object callback

    void loopIoTLibrary(char * dataStreamFromConnectionHandler);//for pub/sub both
    void loopIoTLibrary();//for pub only
    char * ShowUniversalDoc();
    private:
        StaticJsonDocument<500> UniversalDoc[MAX_UNIVERSAL_DOC_ARRAY_LEN];
        uint8_t NextEmptyIndex=0;
        uint8_t dataPubPointer=0;

        //Subscriber Vars
        uint8_t NumberOfSubDocs=0;
        uint8_t readPointer=0;
        //DynamicJsonDocument UniversalDocSub(1024);
        StaticJsonDocument<500> UniversalDocSub[MAX_UNIVERSAL_DOC_ARRAY_LEN];
        //DynamicJsonDocument doc(1024);
        ///
        
        
        

        uint8_t checkNextEmptyIndexAvailabilityAndIncrement();
        void constructJSONDocument(uint8_t DATA_TYPE,int dataValue, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD);
        void constructJSONDocument(uint8_t DATA_TYPE,float dataValue, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD);
        void constructJSONDocument(uint8_t DATA_TYPE,char* dataValue, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD);

        void PublisherLoop();
        void SubscriberLoop();
       // void constructJSONDocument(uint8_t DATA_TYPE,String dataValue, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD);


        //communication handler
        CommunicationHandler  * UniversalCommHandle;
};

void IoTLibrary::PublisherLoop(){
    //serialize json for sending to communication handler
    char a[600];
    for(int i=0;i<NextEmptyIndex;i++){
    serializeJson(UniversalDoc[i],a);
    printf("SENDING: \n%s\n",a);
    UniversalCommHandle->sendJSONString(a);
    this->dataPubPointer++;
    }
    this->dataPubPointer=0;
    /////
}


void IoTLibrary::loopIoTLibrary(){
    PublisherLoop();   
}
void IoTLibrary::SubscriberLoop(){
    char  a[600];
    //a=UniversalCommHandle->readJSONString();

    //get data string here in char a[600]
    
    UniversalCommHandle->readJSONString(a);
    #if DEVMODE
    serializeJson(UniversalDoc[NextEmptyIndex-1],a);//not required; just for testing
    #endif
    //deserialize the incomming data string
    DynamicJsonDocument temp(500);
    deserializeJson(temp, a);
    UniversalDocSub[0]=temp;
    //extract number of docs
    const int NumberOfSubDocs = UniversalDocSub[0]["NumberOfDocs"];
    printf("  %d",NumberOfSubDocs);
    readPointer++;//first doc already read
    if(NumberOfSubDocs>0){
        if(readPointer>NumberOfSubDocs){
            readPointer=0;
        }
        deserializeJson(temp, a);
        UniversalDocSub[readPointer]=temp;

        
        readPointer++;
    }
    const int NS = UniversalDocSub[1]["NumberOfDocs"];
    printf("  %d",NS);
    //const char* dt = doc["DATA_TYPE"];
    // long time          = doc["time"];
    // double latitude    = doc["data"][0];
    // double longitude   = doc["data"][1];
   
}
void IoTLibrary::loopIoTLibrary(char * dataStreamFromConnectionHandler){
    PublisherLoop();
    
    
    SubscriberLoop();
    
    
}

uint8_t IoTLibrary::checkNextEmptyIndexAvailabilityAndIncrement(){
    if(this->NextEmptyIndex<MAX_UNIVERSAL_DOC_ARRAY_LEN){
        this->NextEmptyIndex++;
        return 1;
    }
    else{
        return 0;
    }
}
char * IoTLibrary::ShowUniversalDoc(){
    char a[600];
    for(int i=0;i<NextEmptyIndex;i++){
    serializeJson(UniversalDoc[i],a);
    printf("\n%s\n",a);
    }
}
void IoTLibrary::constructJSONDocument(uint8_t DATA_TYPE,int dataValue, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD){
    StaticJsonDocument<500> doc;
    
     doc["DATA_TYPE"] = DATATYPES::getStingDataType(DATA_TYPE);
     doc["DataValue"] = dataValue;
     doc["Permissions"] = PERMISSIONS::getStringPermission(PERMISSIONS);
     doc["Event"] = EVENTS::getStringEvent(EVENT);
     doc["Method"] = METHODS::getStringMethod(METHOD);
     doc["NumberOfDocs"] = NextEmptyIndex-1;
     
     

    UniversalDoc[NextEmptyIndex-1]=doc;

}
void IoTLibrary::constructJSONDocument(uint8_t DATA_TYPE,float dataValue, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD){
    StaticJsonDocument<500> doc;
    
     doc["DATA_TYPE"] = DATATYPES::getStingDataType(DATA_TYPE);
     doc["DataValue"] = dataValue;
     doc["Permissions"] = PERMISSIONS::getStringPermission(PERMISSIONS);
     doc["Event"] = EVENTS::getStringEvent(EVENT);
     doc["Method"] = METHODS::getStringMethod(METHOD);
     doc["NumberOfDocs"] = NextEmptyIndex-1;
     

    UniversalDoc[NextEmptyIndex-1]=doc;

}

void IoTLibrary::constructJSONDocument(uint8_t DATA_TYPE,char * dataValue, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD){
    StaticJsonDocument<500> doc;
    
     doc["DATA_TYPE"] = DATATYPES::getStingDataType(DATA_TYPE);
     doc["DataValue"] = dataValue;
     doc["Permissions"] = PERMISSIONS::getStringPermission(PERMISSIONS);
     doc["Event"] = EVENTS::getStringEvent(EVENT);
     doc["Method"] = METHODS::getStringMethod(METHOD);
     doc["NumberOfDocs"] = NextEmptyIndex-1;
     

    UniversalDoc[NextEmptyIndex-1]=doc;

}


uint8_t IoTLibrary::addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,void PubCallback (void)){
    if(METHOD==METHODS::PUBLISH){
    if(checkNextEmptyIndexAvailabilityAndIncrement()){
        printf("addProp  %d %d %d nextInd %d",localVar,PERMISSIONS,EVENT,this->NextEmptyIndex);
        constructJSONDocument(DATATYPES::INT,localVar,PERMISSIONS,EVENT,METHOD);
        PubCallback();
    }
    }

    return 1;
}
uint8_t IoTLibrary::addProp(float localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,void PubCallback (void)){
    if(METHOD==METHODS::PUBLISH){
    if(checkNextEmptyIndexAvailabilityAndIncrement()){
        printf("addProp  %d %d %d nextInd %d",localVar,PERMISSIONS,EVENT,this->NextEmptyIndex);
        constructJSONDocument(DATATYPES::FLOAT,localVar,PERMISSIONS,EVENT,METHOD);
        PubCallback();
    }
    }
    return 1;
}

uint8_t IoTLibrary::addProp(char * localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,void PubCallback (void)){
    if(METHOD==METHODS::PUBLISH){
    if(checkNextEmptyIndexAvailabilityAndIncrement()){
        printf("addProp  %d %d %d nextInd %d",localVar,PERMISSIONS,EVENT,this->NextEmptyIndex);
        constructJSONDocument(DATATYPES::CHAR_ARRAY,localVar,PERMISSIONS,EVENT,METHOD);
        PubCallback();
    }
    }
    return 1;
}

////////Subscribe methods
uint8_t IoTLibrary::addProp( uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void SubCallback (int), int arg){
    if(METHOD==METHODS::SUBSCRIBE){
        
        SubCallback(arg);
    }

}

uint8_t IoTLibrary::addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,void callback (int), int arg){
    
    printf("addProp  %d %d %d",localVar,PERMISSIONS,EVENT);
    callback(arg);
    return 1;
}
