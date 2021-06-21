
/******************************************************************************
   INCLUDE
 ******************************************************************************/

#include "Arduino_SerialConnectionHandler.h"
#include "CBORParser/cbor.h"

#define BOARD_HAS_SERIAL 1
#ifdef BOARD_HAS_SERIAL /* Only compile if this is a board with Serial */

/******************************************************************************
   CONSTANTS
 ******************************************************************************/

static int const Serial_TIMEOUT = 30000;

/******************************************************************************
   FUNCTION DEFINITION
 ******************************************************************************/

__attribute__((weak)) void mkr_Serial_feed_watchdog()
{
    /* This function can be overwritten by a "strong" implementation
     * in a higher level application, such as the ArduinoIoTCloud
     * firmware stack.
     */
}

/******************************************************************************
   CTOR/DTOR
 ******************************************************************************/

SerialConnectionHandler::SerialConnectionHandler(Serial * s)
_serial_client =s;
{
      cbor::output_dynamic output;
      cbor::encoder encoder(output);//cbor parser
}

/******************************************************************************
   PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

unsigned long SerialConnectionHandler::getTime()
{
  return 0;
}

/******************************************************************************
   PROTECTED MEMBER FUNCTIONS
 ******************************************************************************/

SerialHConnectionState SerialConnectionHandler::update_handleInit()
{
  mkr_Serial_feed_watchdog();

  if (_Serial.begin(_pin) != Serial_READY)
  {
    Debug.print(DBG_ERROR, F("Serial Error"));
    return SerialHConnectionState::ERROR;
  }

  mkr_Serial_feed_watchdog();

  Debug.print(DBG_INFO, F("Serial ok"));
  _Serial.setTimeout(Serial_TIMEOUT);

  mkr_Serial_feed_watchdog();

  Serial3_SerialHStatus_t const SerialH_status = _Serial.attachSerial(_apn, _login, _pass, true);
  Debug.print(DBG_DEBUG, F("Serial.begin(): %d"), SerialH_status);
  if (SerialH_status == Serial3_SerialHStatus_t::ERROR)
  {
    Debug.print(DBG_ERROR, F("Serial attach failed"));
    Debug.print(DBG_ERROR, F("Make sure the antenna is connected and reset your board."));
    return SerialHConnectionState::ERROR;
  }

  return SerialHConnectionState::CONNECTING;
}

SerialHConnectionState SerialConnectionHandler::update_handleConnecting()
{
  Debug.print(DBG_INFO, F("Sending PING to outer space..."));
  int const ping_result = _Serial.ping("time.arduino.cc");
  Debug.print(DBG_INFO, F("Serial.ping(): %d"), ping_result);
  if (ping_result < 0)
  {
    Debug.print(DBG_ERROR, F("PING failed"));
    Debug.print(DBG_INFO, F("Retrying in  \"%d\" milliseconds"), CHECK_INTERVAL_TABLE[static_cast<unsigned int>(SerialHConnectionState::CONNECTING)]);
    return SerialHConnectionState::CONNECTING;
  }
  else
  {
    Debug.print(DBG_INFO, F("Connected to Serial SerialH"));
    return SerialHConnectionState::CONNECTED;
  }
}

SerialHConnectionState SerialConnectionHandler::update_handleConnected()
{
  int const is_Serial_access_alive = _Serial.isAccessAlive();
  if (is_Serial_access_alive != 1)
  {
    return SerialHConnectionState::DISCONNECTED;
  }
  return SerialHConnectionState::CONNECTED;
}

SerialHConnectionState SerialConnectionHandler::update_handleDisconnecting()
{
  _Serial.shutdown();
  return SerialHConnectionState::DISCONNECTED;
}

SerialHConnectionState SerialConnectionHandler::update_handleDisconnected()
{
  if (_keep_alive)
  {
    return SerialHConnectionState::INIT;
  }
  else
  {
   return SerialHConnectionState::CLOSED;
  }
}

#endif /* #ifdef BOARD_HAS_SERIAL  */
