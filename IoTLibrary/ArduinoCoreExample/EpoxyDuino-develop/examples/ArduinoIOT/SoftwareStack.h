//Software Stack

class SoftwareStack{
    public:
    #define DebugPrint(x) Serial.println(x)
    
    
    String StringSeparator(String data, char separator, int index);
    char * StrToCharArray(String data);
    uint8_t isValidJSON(String jsonS);

    
    private:
      //  String configs="";
      char buf[100];
      
};




char * SoftwareStack::StrToCharArray(String data){
      data.toCharArray(this->buf, data.length()+1);
      return this->buf;
}

uint8_t SoftwareStack::isValidJSON(String jsonS){
  if(jsonS.indexOf("{")>=0 && (jsonS.indexOf("}")>=0)){
    return 1;
  }
  else{
    return 0;
  }
}

String SoftwareStack::StringSeparator(String data, char separator, int index)
{
  
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
  
}
