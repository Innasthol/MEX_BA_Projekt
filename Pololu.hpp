#ifndef POLOLU_HPP_INCLUDED
#define POLOLU_HPP_INCLUDED
#include "SerialCom.hpp"

#ifdef LINUX

#elif defined(WIN32)
    //#include <windows.h>
#endif

class IPololu {
public:
    virtual bool openConnection() = 0;
    virtual bool closeConnection() = 0;
    virtual bool setPosition (unsigned char servo, unsigned short position) = 0;
    virtual bool setSpeed (unsigned char servo, unsigned short speed) = 0;
    virtual bool setAcceleration (unsigned char servo, unsigned short acceleration) = 0;
    virtual unsigned int getPosition (unsigned char servo) = 0;
    virtual bool getMovingState () = 0;
};

class Pololu : public IPololu {
private:
    SerialCom serialCom_;
public:
    Pololu (const char* portName, unsigned int baudRate);
    bool openConnection();
    bool closeConnection();
    bool setPosition (unsigned char servo, unsigned short position);
    bool setSpeed (unsigned char servo, unsigned short speed);
    bool setAcceleration (unsigned char servo, unsigned short acceleration);
    unsigned int getPosition (unsigned char servo);
    bool getMovingState ();
};

#endif // POLOLU_HPP_INCLUDED
