#include "IoTConnectionHandler.h"

#define SERIAL_BUFFER_SIZE 1024

class SerialConnectionHandler : public IoTConnectionHandler
{
  public:

    SerialConnectionHandler (
      ReceivedMessageCallback fReceivedMessageCallback,
      RX_PIN,
      TX_PIN,
      BAUD
    ) :
      fReceivedMessageCallback_ (fReceivedMessageCallback),
      data_ (NULL),
      bufferSize_ (SERIAL_BUFFER_SIZE),
      rxPin_ (RX_PIN),
      txPin_ (TX_PIN),
      baud_ (BAUD)
    {}

    ~SerialConnectionHandler () {
      stop();
    }

    void subscribeToChannel (MQTTChannel *channel);

    void begin ();

    void stop ();

    bool isConnected() ;

    //the IoTLibrary will call this function to send messages
    void sendMessage (const MQTTChannel * channel, const byte * data, const int length);

    //call this periodically
    bool update ();

  private:

    void reset ();

    //the IoTLibrary should register a function with this form to get messages received from this serial port
    typedef int  (*ReceivedMessageCallback)  (const MQTTChannel * channel, const byte * data, const int length);

    // callback function
    ReceivedMessageCallback ReceivedMessageCallback_;
    byte * inputbuffer_;
    const int bufferSize_;
    byte inputPos_;

    int rxPin_, txPin_, baud_;
    SoftwareSerial *serialPort_;

    MQTTChannel *myChannel_;  //keep track of the channel we were told to subscribe to
};


///////////////////////SerialIoTConnectionHandler.cpp////////////////////////

void SerialConnectionHandler::begin ()
{
  serialPort_ = new SoftwareSerial(rxPin_, txPin_, baud_);
  inputbuffer_ = (byte *) malloc (bufferSize_);
  reset ();
}

void SerialConnectionHandler::stop ()
{
  free (inputbuffer_);
  data_ = NULL;
}

bool SerialConnectionHandler::isConnected() {
  if ( inputbuffer_ == NULL)
    return false;

  //claim we are connected to the MQTT broker
  return true;
}

void SerialConnectionHandler::subscribeToChannel (MQTTChannel *channel)
{
  //we can't actually subscribe to a channel, but we will claim that incoming data comes from this one channel
  myChannel_ = channel;
}

bool SerialConnectionHandler::sendMessage (const MQTTChannel * channel, const byte * data, const int length);
{
  //we ignore the requested channel
  //the data will go out on the serial port
  for ( int i = 0; i < length; i++)
  {
    serialPort.write(data[i]);
  }
  return true;
};

void SerialConnectionHandler::update ()
{
  if (!isConnected())
    return ;

  if (serialPort.available() > 0) {
    incomingByte = serialPort.read();
    switch (incomingByte)
    {
      case '/0':        // end of packet
      case '/n':
      case '/r':
        if (inputPos_ > 0) //any data received?
        {
          if (inputPos_ < bufferSize_)
          {
            inputbuffer_[inputPos_++] = 0; //append NULL to the end

            //tell the IoTLibrary that a message was received
            fReceivedMessageCallback_(myChannel_, inputbuffer_, inputPos_);
          }
        }
        break;

      default:          //add byte to buffer
        if (inputPos_ < bufferSize_)
          inputbuffer_[inputPos_++] = incomingByte;
        else  //overflow
          reset ();
        break;
    }
  }
}

void SerialConnectionHandler::reset ()
{
  inputPos_ = 0;
}
