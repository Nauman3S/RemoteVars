class PERMISSIONS{
    public:
        static const int READ= 1;
        static const int WRITE= 2;
        static const int READ_WRITE= 3;

        static const char * getStringPermission(const int p){
            if(p==1){
                return "READ";
            }
            else if(p==2){
                return "WRITE";
            }
            else if(p==3){
                return "READ_WRITE";
            }
        }

};

class EVENTS{
    public:
        static const int ON_CHANGE= 1;
        static const int ON_CLOUD_CHANGE= 2;
        static const int ON_LOCAL_CHANGE= 3;
         static const char * getStringEvent(const int e){
            if(e==1){
                return "ON_CHANGE";
            }
            else if(e==2){
                return "ON_CLOUD_CHANGE";
            }
            else if(e==3){
                return "ON_LOCAL_CHANGE";
            }
        }
        

};

class METHODS{
    public:
        static const int PUBLISH= 1;
        static const int SUBSCRIBE= 2;
         static const char * getStringMethod(const int m){
            if(m==1){
                return "PUBLISH";
            }
            else if(m==2){
                return "SUBSCRIBE";
            }
           
        }
        
};

class DATATYPES{
    public:
        static const int INT= 1;
        static const int FLOAT= 2;
        static const int STRING= 3;
        static const int CHAR_ARRAY= 4;

         static const char * getStringDataType(const int d){
            if(d==1){
                return "INT";
            }
            else if(d==2){
                return "FLOAT";
            }
            else if(d==3){
                return "STRING";
            }
            else if(d==4){
                return "CHAR_ARRAY";
            }
         }
        
        
};