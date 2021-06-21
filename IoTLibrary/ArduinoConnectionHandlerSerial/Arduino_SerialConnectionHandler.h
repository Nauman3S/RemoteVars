
#ifndef SERIAL_CONNECTION_MANAGER_H_
#define SERIAL_CONNECTION_MANAGER_H_

/******************************************************************************
   INCLUDE
 ******************************************************************************/

#include "Arduino_ConnectionHandler.h"

#define BOARD_HAS_SERIAL 1
#ifdef BOARD_HAS_SERIAL /* Only compile if this is a board with Serial */

/******************************************************************************
   CLASS DECLARATION
 ******************************************************************************/

class SerialConnectionHandler : public ConnectionHandler
{
  public:

    //SerialConnectionHandler(const char * pin, const char * apn, const char * login, const char * pass, bool const keep_alive = true);
    SerialConnectionHandler(Serial *s);


    virtual unsigned long getTime() override;
    virtual Client & getClient() override { return _serial_client; };
    virtual UDP & getUDP() override { return _Serial_udp; };


  protected:

    virtual SerialHConnectionState update_handleInit         () override;
    virtual SerialHConnectionState update_handleConnecting   () override;
    virtual SerialHConnectionState update_handleConnected    () override;
    virtual SerialHConnectionState update_handleDisconnecting() override;
    virtual SerialHConnectionState update_handleDisconnected () override;


  private:

    const char * _pin;
    const char * _apn;
    const char * _login;
    const char * _pass;

    Serial _Serial;
    GPRS _gprs;
    SerialUDP _Serial_udp;
    Serial * _serial_client;
};

#endif /* #ifdef BOARD_HAS_SERIAL  */

#endif /* #ifndef Serial_CONNECTION_MANAGER_H_ */
