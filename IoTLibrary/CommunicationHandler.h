#include <stdio.h>
#include <string.h>
class CommunicationHandler{

    public:
    CommunicationHandler();
    ~CommunicationHandler();
    

        char * readJSONString();//read a JSON string as a char array starts with '{' and ends with '}'
        void sendJSONString(char * str);//sends a JSON string as a char array


};

CommunicationHandler::CommunicationHandler()
{
    printf("Initializing Communication Handler");
}
CommunicationHandler::~CommunicationHandler()
{
    printf("Clearing up Communication Handler");
}
void CommunicationHandler::sendJSONString(char * str){
    char startW='{';
    char endW='}';
    //check if the JSON is valid
    if(str[0]==startW && str[strlen(str)-1]){
        printf("COMMHANDLE HERE SENDING %s",str);
        //send to serial,wifi,rf etc
    }
}

char *  CommunicationHandler::readJSONString(){
    printf("COMMHANDLE AS A READER");
    char startW='{';
    char endW='}';

    //read from serial, wifi, rf etc from '{'  to '}' and store it in the variable below
    char * JSONRead;
    //check if the JSON is valid
    if(JSONRead[0]==startW && JSONRead[strlen(JSONRead)-1]){
        return JSONRead;
    }
}