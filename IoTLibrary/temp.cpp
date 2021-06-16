#include <iostream>
#include <stdio.h>
#include "IoTLibrary.h"
//#include "CommunicationHandler.h"

CommunicationHandler myCommHandle;
void printIT(){
    printf("\n\nPRINTER\n\n");
}
void printINT(int a){
    printf("\n\nPRINTER_INT\n\n %d",a);
}
int main() {
    

    IoTLibrary al(&myCommHandle);

    char * strAr="new dt char array";
    al.addProp(12, PERMISSIONS::WRITE, EVENTS::ON_CHANGE,METHODS::PUBLISH, printIT);
    al.addProp(99, PERMISSIONS::WRITE, EVENTS::ON_CHANGE,METHODS::PUBLISH, printIT);
    al.addProp((float)12.1, PERMISSIONS::WRITE, EVENTS::ON_CHANGE,METHODS::PUBLISH, printIT);


    al.addProp(strAr, PERMISSIONS::WRITE, EVENTS::ON_CHANGE,METHODS::PUBLISH, printIT);
    al.addProp(12, PERMISSIONS::WRITE, EVENTS::ON_CHANGE,METHODS::PUBLISH, printINT,11);
    
    std::cout << "Hello World!";
      printf("adsds");
    
    String s="NEW STRING OBJECT IS HERE";
    printf("%s",s.val);


    //   StaticJsonDocument<200> doc;
    // doc["sensor"] = "gps";
    //     doc["time"] = 1351824120;
    //     char a[200];
    //     serializeJson(doc,a);
    //     printf("json%s",a);

    al.ShowUniversalDoc();

    al.loopIoTLibrary("tmep");

    char * a="{adss new}";
    printf("%s  first {%c  last }%c",a,a[0],a[strlen(a)-1]);
    return 0;
}
