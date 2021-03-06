//#include <iostream>
#if SHOW_DEBUG_MESSAGES
#define DEVMODE 1 //dev mode helper
#else
#define DEVMODE 0 //dev mode helper
#endif
#include <stdio.h>
#include "const_values.h"
#include "consts.h"
#include "ArduinoJSON.h"
#include "CommunicationHandler.h"




class IoTLibrary 
{
  public:

    IoTLibrary(CommunicationHandler *ch){
        UniversalCommHandle=ch;
    }

    //publish prototypes with callbacks
    uint8_t addProperty(String dataTopic,uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void PubCallback (void),int * localVar); //simple callback function prototype
    uint8_t addProperty(String dataTopic, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void PubCallback (void),double * localVar); //simple callback function prototype
    uint8_t addProperty(String dataTopic, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void PubCallback (void),String * localVar); //simple callback function prototype
    //publish prototypes without callbacks
    uint8_t addProperty( String dataTopic,uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,int * localVar); 
    uint8_t addProperty(String dataTopic, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,double * localVar);
    uint8_t addProperty(String dataTopic, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,String * localVar);
   
    
    //subscribe prototypes
    uint8_t addProperty( String dataTopic,uint8_t PERMISSIONS, uint8_t EVENT,uint8_t METHOD, void SubCallback (String topic, String data)); //simple callback function prototype
    // uint8_t addProperty( String dataTopic,uint8_t PERMISSIONS, uint8_t EVENT,uint8_t METHOD, void SubCallback (String topic, String data)); //simple callback function prototype
    // uint8_t addProperty( String dataTopic,uint8_t PERMISSIONS, uint8_t EVENT,uint8_t METHOD, void SubCallback (String topic, String data)); //simple callback function prototype

    // uint8_t addProperty(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void callback (int), int arg); //int callback
    // uint8_t addProperty(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void callback (float), float arg); //float callback
    // //uint8_t addProperty(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void callback (String), String arg); //string object callback
    
    // uint8_t addProperty(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, int callback (int), int arg); //int callback
    // uint8_t addProperty(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, float callback (float), float arg); //float callback
    // //uint8_t addProperty(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, String callback (String), String arg); //string object callback

    void loopIoTLibrary();//for pub/sub both
    
    String ShowUniversalDoc();
    private:
        JSONHandler jh;
        String UniversalDoc[MAX_UNIVERSAL_DOC_ARRAY_LEN];
        int UniversalINTOLDPointers[MAX_UNIVERSAL_DOC_ARRAY_LEN];
        int * UniversalINTPointers[MAX_UNIVERSAL_DOC_ARRAY_LEN];
        double * UniversalDOUBLEPointers[MAX_UNIVERSAL_DOC_ARRAY_LEN];
        double UniversalDOUBLEOLDPointers[MAX_UNIVERSAL_DOC_ARRAY_LEN];
        String * UniversalSTRINGPointers[MAX_UNIVERSAL_DOC_ARRAY_LEN];
        String UniversalSTRINGOLDPointers[MAX_UNIVERSAL_DOC_ARRAY_LEN];
        
        uint8_t NextEmptyIndex=0;
        uint8_t dataPubPointer=0;

        //Subscriber Vars
        //uint8_t NumberOfSubDocs=0;
        uint8_t readPointer=0;
        uint8_t subTopicsPointer=0;
        //DynamicJsonDocument UniversalDocSub(1024);
       // String UniversalDocSub[MAX_UNIVERSAL_DOC_ARRAY_LEN];
        String subscribedTopics[MAX_UNIVERSAL_DOC_ARRAY_LEN];
        //DynamicJsonDocument doc(1024);
        ///
        
        
        

        uint8_t checkNextEmptyIndexAvailabilityAndIncrement();
        

        void PublisherLoop();
        void SubscriberLoop();
        void (*SubCallbackGlobal) (String topic, String data);

       // void constructJSONDocument(String dataValue, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD);


        //communication handler
        CommunicationHandler  * UniversalCommHandle;
};


void IoTLibrary::PublisherLoop(){
    //serialize json for sending to communication handler
    
    for(int i=0;i<NextEmptyIndex;i++){
      
        jh.extractDataID(UniversalDoc[i]);
    
    //jh.updateJSON(UniversalDoc[i],UniversalINTPointers[i]);
    //SERIAL_PORT_MONITOR.print( "EXTRACTED TYPE:");
    //SERIAL_PORT_MONITOR.println( );
    if(jh.extractDataType(UniversalDoc[i]).indexOf("i")>=0){
      
        //SERIAL_PORT_MONITOR.print( "UPDATED INT:");
        // SERIAL_PORT_MONITOR.println(*UniversalINTPointers[i] );
        if(UniversalINTOLDPointers[i]!=*UniversalINTPointers[i]){
        String UpdatedValue=jh.updateJSON(UniversalDoc[i],*UniversalINTPointers[i]);
        // SERIAL_PORT_MONITOR.print( "UPDATED VAL:");
        // SERIAL_PORT_MONITOR.println(UpdatedValue );
        UniversalINTOLDPointers[i]=*UniversalINTPointers[i];
        UniversalDoc[i]=UpdatedValue;
        UniversalCommHandle->sendJSONString(UniversalDoc[i]);
        }
        
    }
    else if(jh.extractDataType(UniversalDoc[i]).indexOf("d")>=0){
        if(UniversalDOUBLEOLDPointers[i]!=*UniversalDOUBLEPointers[i]){
        String UpdatedValue=jh.updateJSON(UniversalDoc[i],*UniversalDOUBLEPointers[i]);
        UniversalDOUBLEOLDPointers[i]=*UniversalDOUBLEPointers[i];
        UniversalDoc[i]=UpdatedValue;
        UniversalCommHandle->sendJSONString(UniversalDoc[i]);
        }
    }
    else if(jh.extractDataType(UniversalDoc[i]).indexOf("s")>=0){
        if(UniversalSTRINGOLDPointers[i]!=*UniversalSTRINGPointers[i]){
        String UpdatedValue=jh.updateJSON(UniversalDoc[i],*UniversalSTRINGPointers[i]);
        UniversalSTRINGOLDPointers[i]=*UniversalSTRINGPointers[i];
        UniversalDoc[i]=UpdatedValue;
        UniversalCommHandle->sendJSONString(UniversalDoc[i]);
        }
    }
    
    this->dataPubPointer++;
    }
    this->dataPubPointer=0;
    /////
}


// void IoTLibrary::loopIoTLibrary(){
//     PublisherLoop();   
// }

void IoTLibrary::SubscriberLoop(){
    
    String incommingDataJSON=UniversalCommHandle->readJSONString();
   //Sample Data
   //incommingDataJSON=String("{DataType:i,Value:12,DataTopic:remote_potentiometer,ID:1,Permission:2,Event:3,Method:1}");//sample
    if(jh.isValidJSON(incommingDataJSON)==1){
        //printf("validjson");    
        String topicR=jh.extractTopic(incommingDataJSON);
        String valR;
        for(int i=0;i<subTopicsPointer;i++){
            
            if(topicR.indexOf(subscribedTopics[i])>=0){
                //if topic is subscribed
                valR=jh.extractData(incommingDataJSON);
                SubCallbackGlobal(topicR,String(valR));
                if(jh.extractDataType(incommingDataJSON).indexOf("i")>=0){
                    //integer data received
                    //used callback above to send the data
                    

                }
            }
        }
    }
    else{
        //Incomming JSON is invalid
    }

    
    //printf("SubscriberLOOPEND");
    
   
}
void IoTLibrary::loopIoTLibrary(){
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
String IoTLibrary::ShowUniversalDoc(){
    String v;
    for(int i=0;i<NextEmptyIndex;i++){
        v=v+UniversalDoc[i]+String(',');
        
    }
    return v;
}
uint8_t IoTLibrary::addProperty(String dataTopic, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,void PubCallback (void),int * localVar){
    if(METHOD==METHODS::PUBLISH && EVENT==EVENTS::ON_LOCAL_CHANGE && PERMISSIONS==PERMISSIONS::WRITE_TO_LOCAL){
    if(checkNextEmptyIndexAvailabilityAndIncrement()){
        #if DEVMODE
        printf("addProperty  %d %d %d nextInd %d",*localVar,PERMISSIONS,EVENT,this->NextEmptyIndex);
        #endif
        UniversalINTPointers[NextEmptyIndex]=localVar;
        UniversalDoc[NextEmptyIndex]=jh.constructJSON(*UniversalINTPointers[NextEmptyIndex],dataTopic,NextEmptyIndex,PERMISSIONS,EVENT,METHOD);
        UniversalINTOLDPointers[NextEmptyIndex]=*UniversalINTPointers[NextEmptyIndex];
        NextEmptyIndex++;
        //constructJSONDocument(DATATYPES::INT,localVar,dataTopic,PERMISSIONS,EVENT,METHOD);
        PubCallback();
    }
    }

    return 1;
}
uint8_t IoTLibrary::addProperty(String dataTopic, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,void PubCallback (void),double * localVar){
    if(METHOD==METHODS::PUBLISH && EVENT==EVENTS::ON_LOCAL_CHANGE && PERMISSIONS==PERMISSIONS::WRITE_TO_LOCAL){
    if(checkNextEmptyIndexAvailabilityAndIncrement()){
        #if DEVMODE
        printf("addProperty  %d %d %d nextInd %d",*localVar,PERMISSIONS,EVENT,this->NextEmptyIndex);
        #endif
        //constructJSONDocument(DATATYPES::FLOAT,localVar,dataTopic,PERMISSIONS,EVENT,METHOD);
        UniversalDOUBLEPointers[NextEmptyIndex]=localVar;
        UniversalDoc[NextEmptyIndex]=jh.constructJSON(*UniversalDOUBLEPointers[NextEmptyIndex],dataTopic,NextEmptyIndex,PERMISSIONS,EVENT,METHOD);
        UniversalDOUBLEOLDPointers[NextEmptyIndex]=*UniversalDOUBLEPointers[NextEmptyIndex];
        NextEmptyIndex++;
        PubCallback();
    }
    }
    return 1;
}

uint8_t IoTLibrary::addProperty(String dataTopic, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,void PubCallback (void),String * localVar){
    if(METHOD==METHODS::PUBLISH && EVENT==EVENTS::ON_LOCAL_CHANGE && PERMISSIONS==PERMISSIONS::WRITE_TO_LOCAL){
    if(checkNextEmptyIndexAvailabilityAndIncrement()){
        #if DEVMODE
        //  /printf("addProperty  %s %d %d nextInd %d",localVar.c_str(),PERMISSIONS,EVENT,this->NextEmptyIndex);
        #endif
        //constructJSONDocument(DATATYPES::CHAR_ARRAY,localVar,dataTopic,PERMISSIONS,EVENT,METHOD);
        UniversalSTRINGPointers[NextEmptyIndex]=localVar;
        UniversalDoc[NextEmptyIndex]=jh.constructJSON(*UniversalSTRINGPointers[NextEmptyIndex],dataTopic,NextEmptyIndex,PERMISSIONS,EVENT,METHOD);
        UniversalSTRINGOLDPointers[NextEmptyIndex]=*UniversalSTRINGPointers[NextEmptyIndex];
        NextEmptyIndex++;
        PubCallback();
    }
    }
    return 1;
}





uint8_t IoTLibrary::addProperty(String dataTopic, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,int * localVar){
    if(METHOD==METHODS::PUBLISH && EVENT==EVENTS::ON_LOCAL_CHANGE && PERMISSIONS==PERMISSIONS::WRITE_TO_LOCAL){
    if(checkNextEmptyIndexAvailabilityAndIncrement()){
        #if DEVMODE
        printf("addProperty  %d %d %d nextInd %d",*localVar,PERMISSIONS,EVENT,this->NextEmptyIndex);
        #endif
        UniversalINTPointers[NextEmptyIndex]=localVar;
        UniversalDoc[NextEmptyIndex]=jh.constructJSON(*UniversalINTPointers[NextEmptyIndex],dataTopic,NextEmptyIndex,PERMISSIONS,EVENT,METHOD);
        UniversalINTOLDPointers[NextEmptyIndex]=*UniversalINTPointers[NextEmptyIndex];
        NextEmptyIndex++;
        //constructJSONDocument(DATATYPES::INT,localVar,dataTopic,PERMISSIONS,EVENT,METHOD);
        
    }
    }

    return 1;
}
uint8_t IoTLibrary::addProperty(String dataTopic, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,double * localVar){
    if(METHOD==METHODS::PUBLISH && EVENT==EVENTS::ON_LOCAL_CHANGE && PERMISSIONS==PERMISSIONS::WRITE_TO_LOCAL){
    if(checkNextEmptyIndexAvailabilityAndIncrement()){
        #if DEVMODE
        printf("addProperty  %d %d %d nextInd %d",*localVar,PERMISSIONS,EVENT,this->NextEmptyIndex);
        #endif
        //constructJSONDocument(DATATYPES::FLOAT,localVar,dataTopic,PERMISSIONS,EVENT,METHOD);
        UniversalDOUBLEPointers[NextEmptyIndex]=localVar;
        UniversalDoc[NextEmptyIndex]=jh.constructJSON(*UniversalDOUBLEPointers[NextEmptyIndex],dataTopic,NextEmptyIndex,PERMISSIONS,EVENT,METHOD);
        UniversalDOUBLEOLDPointers[NextEmptyIndex]=*UniversalDOUBLEPointers[NextEmptyIndex];
        NextEmptyIndex++;
        
    }
    }
    return 1;
}

uint8_t IoTLibrary::addProperty(String dataTopic, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,String * localVar){
    if(METHOD==METHODS::PUBLISH && EVENT==EVENTS::ON_LOCAL_CHANGE && PERMISSIONS==PERMISSIONS::WRITE_TO_LOCAL){
    if(checkNextEmptyIndexAvailabilityAndIncrement()){
        #if DEVMODE
      //  printf("addProperty  %s %d %d nextInd %d",localVar.c_str(),PERMISSIONS,EVENT,this->NextEmptyIndex);
        #endif
        //constructJSONDocument(DATATYPES::CHAR_ARRAY,localVar,dataTopic,PERMISSIONS,EVENT,METHOD);
        UniversalSTRINGPointers[NextEmptyIndex]=localVar;
        UniversalDoc[NextEmptyIndex]=jh.constructJSON(*UniversalSTRINGPointers[NextEmptyIndex],dataTopic,NextEmptyIndex,PERMISSIONS,EVENT,METHOD);
        UniversalSTRINGOLDPointers[NextEmptyIndex]=*UniversalSTRINGPointers[NextEmptyIndex];
        NextEmptyIndex++;
        
    }
    }
    return 1;
}

////////Subscribe methods
// uint8_t IoTLibrary::addProperty( uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD, void SubCallback (int), int arg){
//     if(METHOD==METHODS::SUBSCRIBE){
        
//         SubCallback(arg);
//     }

// }

uint8_t IoTLibrary::addProperty( String dataTopic,uint8_t PERMISSIONS, uint8_t EVENT,uint8_t METHOD, void SubCallback (String topic, String data)){
    //  printf("Trying to subscribe to %s",dataTopic.c_str());
     if(METHOD==METHODS::SUBSCRIBE && PERMISSIONS==PERMISSIONS::READ_FROM_CLOUD && EVENT==EVENTS::ON_CLOUD_CHANGE){
         printf("TRYING to subscribe");
         SERIAL_PORT_MONITOR.println(dataTopic);
         subscribedTopics[subTopicsPointer]=dataTopic;
         subTopicsPointer++;
         SubCallbackGlobal=SubCallback;
    //      strcpy(subscribedTopics[subTopicsPointer],dataTopic.c_str());
     
     }
}

// uint8_t IoTLibrary::addProperty( String dataTopic,uint8_t PERMISSIONS, uint8_t EVENT,uint8_t METHOD, void SubCallback (String topic, String data)){
//     //  printf("Trying to subscribe to %s",dataTopic.c_str());
//      if(METHOD==METHODS::SUBSCRIBE && PERMISSIONS==PERMISSIONS::READ_FROM_CLOUD && EVENT==EVENTS::ON_CLOUD_CHANGE){
//          printf("TRYING to subscribe");
//          SERIAL_PORT_MONITOR.println(dataTopic);
//          subscribedTopics[subTopicsPointer]=dataTopic;
//          subTopicsPointer++;
//          SubCallbackGlobal=SubCallback;
//     //      strcpy(subscribedTopics[subTopicsPointer],dataTopic.c_str());
     
//      }
// }

// uint8_t IoTLibrary::addProperty( String dataTopic,uint8_t PERMISSIONS, uint8_t EVENT,uint8_t METHOD, void SubCallback (String topic, String data)){
//     //  printf("Trying to subscribe to %s",dataTopic.c_str());
//      if(METHOD==METHODS::SUBSCRIBE && PERMISSIONS==PERMISSIONS::READ_FROM_CLOUD && EVENT==EVENTS::ON_CLOUD_CHANGE){
//          printf("TRYING to subscribe");
//          SERIAL_PORT_MONITOR.println(dataTopic);
//          subscribedTopics[subTopicsPointer]=dataTopic;
//          subTopicsPointer++;
//          SubCallbackGlobal=SubCallback;
//     //      strcpy(subscribedTopics[subTopicsPointer],dataTopic.c_str());
     
//      }
// }
// uint8_t IoTLibrary::addProperty(int localVar, uint8_t PERMISSIONS, uint8_t EVENT, uint8_t METHOD,void callback (int), int arg){
//     #if DEVMODE
//     printf("addProperty  %d %d %d",localVar,PERMISSIONS,EVENT);
//     #endif
//     callback(arg);
//     return 1;
// }
