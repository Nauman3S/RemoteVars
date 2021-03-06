#include "SoftwareStack.h"


class JSONHandler{

  public:
    String constructJSON(String Value, String DataTopic, uint8_t ID ,uint8_t Permission, uint8_t Event, uint8_t Method);
    String constructJSON(int Value, String DataTopic,  uint8_t ID ,uint8_t Permission, uint8_t Event, uint8_t Method);
    String constructJSON(double Value, String DataTopic,  uint8_t ID ,uint8_t Permission, uint8_t Event, uint8_t Method);
    
    String extractDataType(String jsonV);
    String extractDataID(String jsonV);
    String extractTopic(String jsonV);
    String extractData(String jsonV);
    
    String updateJSON(String JSONV, String Value);
    String updateJSON(String JSONV, int Value);
    String updateJSON(String JSONV, double Value);
    uint8_t isValidJSON(String jsonS){return ss.isValidJSON(jsonS);};

  private:
    SoftwareStack ss;
    String types(String a) { return String("s"); }
    String types(int a) {  return String("i"); }
    String types(char *a) {  return String("c"); }
    String types(float a) {  return String("f"); }
    String types(double a) {  return String("d"); }

};
String JSONHandler::extractDataType(String jsonV){
  int a1=jsonV.indexOf("DataType:");
  int a2=jsonV.indexOf(",Value");

  String s1=jsonV.substring(a1,a2);
  //SERIAL_PORT_MONITOR.println(s1);
  return s1;
}

String JSONHandler::extractTopic(String jsonV){
  int a1=jsonV.indexOf("DataTopic:");
  int a2=jsonV.indexOf(",ID:");

  String s1=jsonV.substring(a1+10,a2);
  //SERIAL_PORT_MONITOR.println(s1);
  return s1;
}
String JSONHandler::extractData(String jsonV){
  int a1=jsonV.indexOf("Value:");
  int a2=jsonV.indexOf(",DataTopic:");

  String s1=jsonV.substring(a1+6,a2);
  //SERIAL_PORT_MONITOR.println(s1);
  return s1;
}
String JSONHandler::extractDataID(String jsonV){
  //printf("DataID Func::");
  
  int a1=jsonV.indexOf("ID:");
  int a2=jsonV.indexOf(",Permission");
  String s1=jsonV.substring(a1,a2);
  //SERIAL_PORT_MONITOR.println(s1);
  //printf("END DataID Func::");
  return s1;
}

String JSONHandler::updateJSON(String JSONV, String Value){
  String NewString=JSONV;
  int a1=NewString.indexOf("Value:");
  int a2=NewString.indexOf(",DataTopic");
  String oldValue=NewString.substring(a1+6,a2);
  NewString.replace(String(oldValue),String(Value));
  


  return NewString;
}
String JSONHandler::updateJSON(String JSONV, int Value){
  String NewString=JSONV;
  int a1=NewString.indexOf("Value:");
  int a2=NewString.indexOf(",DataTopic");
  String oldValue=NewString.substring(a1+6,a2);
  NewString.replace(String(oldValue),String(Value));
  
  return NewString;
}
String JSONHandler::updateJSON(String JSONV, double Value){
  String NewString=JSONV;
  int a1=NewString.indexOf("Value:");
  int a2=NewString.indexOf(",DataTopic");
  String oldValue=NewString.substring(a1+6,a2);
  NewString.replace(String(oldValue),String(Value));

  return NewString;
}
String JSONHandler::constructJSON( String Value, String DataTopic, uint8_t ID , uint8_t Permission, uint8_t Event, uint8_t Method){
  String s=String("{")
  +String("DataType:")+String(types(Value))+String(",")
  +String("Value:")+Value+String(",")
  +String("DataTopic:")+DataTopic+String(",")
  +String("ID:")+ID+String(",")
  +String("Permission:")+String(Permission)+String(",")
  +String("Event:")+String(Event)+String(",")
  +String("Method:")+String(Method)
  +String("}");

  return s;
}

String JSONHandler::constructJSON( int Value, String DataTopic, uint8_t ID , uint8_t Permission, uint8_t Event, uint8_t Method){
  String s=String("{")
  +String("DataType:")+String(types(Value))+String(",")
  +String("Value:")+String(Value)+String(",")
  +String("DataTopic:")+DataTopic+String(",")
  +String("ID:")+ID+String(",")
  +String("Permission:")+String(Permission)+String(",")
  +String("Event:")+String(Event)+String(",")
  +String("Method:")+String(Method)
  +String("}");

  return s;
}
String JSONHandler::constructJSON( double Value, String DataTopic, uint8_t ID ,uint8_t Permission, uint8_t Event, uint8_t Method){
  String s=String("{")
  +String("DataType:")+String(types(Value))+String(",")
  +String("Value:")+String(Value)+String(",")
  +String("DataTopic:")+DataTopic+String(",")
  +String("ID:")+ID+String(",")
  +String("Permission:")+String(Permission)+String(",")
  +String("Event:")+String(Event)+String(",")
  +String("Method:")+String(Method)
  +String("}");

  return s;
}