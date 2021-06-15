#include <iostream>
#include <stdio.h>
#include "const_values.h"
#include "consts.h"
#include "ArduinoJSON.h"

#define STRING_EMU 1 //arduino string object emulator
#if STRING_EMU
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

    int temp;
    uint8_t addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void callback (void)); //simple callback function prototype
    uint8_t addProp(float localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void callback (void)); //simple callback function prototype
    uint8_t addProp(char* localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void callback (void)); //simple callback function prototype
   // uint8_t addProp(String localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void callback (void)); //simple callback function prototype
    

    uint8_t addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void callback (int), int arg); //int callback
    uint8_t addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void callback (float), float arg); //float callback
    uint8_t addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void callback (String), String arg); //string object callback
    
    uint8_t addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, int callback (int), int arg); //int callback
    uint8_t addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, float callback (float), float arg); //float callback
    uint8_t addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, String callback (String), String arg); //string object callback

    void loopIoTLibrary();
    char * ShowUniversalDoc();
    private:
        StaticJsonDocument<500> UniversalDoc[MAX_UNIVERSAL_DOC_ARRAY_LEN];
        uint8_t NextEmptyIndex=0;

        
        StaticJsonDocument<500*MAX_UNIVERSAL_DOC_ARRAY_LEN> tempDoc;
        JsonArray tempArray = tempDoc.createNestedArray("UniversalArray");
        

        uint8_t checkNextEmptyIndexAvailabilityAndIncrement();
        void constructJSONDocument(uint8_t DATA_TYPE,int dataValue, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD);
        void constructJSONDocument(uint8_t DATA_TYPE,float dataValue, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD);
        void constructJSONDocument(uint8_t DATA_TYPE,char* dataValue, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD);
       // void constructJSONDocument(uint8_t DATA_TYPE,String dataValue, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD);

};

void IoTLibrary::loopIoTLibrary(){
    char temp[500];
    //generate universal array for sending
    for (int i=0;i<NextEmptyIndex-1;i++){
        serializeJson(UniversalDoc[i], temp);
        tempArray.add(temp);
    }
    /////
    
    
    char a[500];
    serializeJson(tempDoc, a);
    printf("\n%s\n",a);
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
     

    UniversalDoc[NextEmptyIndex-1]=doc;

}
void IoTLibrary::constructJSONDocument(uint8_t DATA_TYPE,float dataValue, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD){
    StaticJsonDocument<500> doc;
    
     doc["DATA_TYPE"] = DATATYPES::getStingDataType(DATA_TYPE);
     doc["DataValue"] = dataValue;
     doc["Permissions"] = PERMISSIONS::getStringPermission(PERMISSIONS);
     doc["Event"] = EVENTS::getStringEvent(EVENT);
     doc["Method"] = METHODS::getStringMethod(METHOD);
     

    UniversalDoc[NextEmptyIndex-1]=doc;

}

void IoTLibrary::constructJSONDocument(uint8_t DATA_TYPE,char * dataValue, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD){
    StaticJsonDocument<500> doc;
    
     doc["DATA_TYPE"] = DATATYPES::getStingDataType(DATA_TYPE);
     doc["DataValue"] = dataValue;
     doc["Permissions"] = PERMISSIONS::getStringPermission(PERMISSIONS);
     doc["Event"] = EVENTS::getStringEvent(EVENT);
     doc["Method"] = METHODS::getStringMethod(METHOD);
     

    UniversalDoc[NextEmptyIndex-1]=doc;

}

uint8_t IoTLibrary::addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,void callback (void)){
    
    if(checkNextEmptyIndexAvailabilityAndIncrement()){
        printf("addProp  %d %d %d nextInd %d",localVar,PERMISSIONS,EVENT,this->NextEmptyIndex);
        constructJSONDocument(DATATYPES::INT,localVar,PERMISSIONS,EVENT,METHOD);
        callback();
    }

    return 1;
}
uint8_t IoTLibrary::addProp(float localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,void callback (void)){
    
    if(checkNextEmptyIndexAvailabilityAndIncrement()){
        printf("addProp  %d %d %d nextInd %d",localVar,PERMISSIONS,EVENT,this->NextEmptyIndex);
        constructJSONDocument(DATATYPES::FLOAT,localVar,PERMISSIONS,EVENT,METHOD);
        callback();
    }

    return 1;
}

uint8_t IoTLibrary::addProp(char * localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,void callback (void)){
    
    if(checkNextEmptyIndexAvailabilityAndIncrement()){
        printf("addProp  %d %d %d nextInd %d",localVar,PERMISSIONS,EVENT,this->NextEmptyIndex);
        constructJSONDocument(DATATYPES::CHAR_ARRAY,localVar,PERMISSIONS,EVENT,METHOD);
        callback();
    }

    return 1;
}

uint8_t IoTLibrary::addProp(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,void callback (int), int arg){
    
    printf("addProp  %d %d %d",localVar,PERMISSIONS,EVENT);
    callback(arg);
    return 1;
}
