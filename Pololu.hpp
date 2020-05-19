#ifndef POLOLU_HPP_INCLUDED
#define POLOLU_HPP_INCLUDED
#include "SerialCom.hpp"

#ifdef LINUX

#elif defined(WIN32)
    //#include <windows.h>
#endif

class IPololu {
public:
    virtual bool setPosition (unsigned char servo, unsigned short position) = 0;
    virtual bool setSpeed (unsigned char servo, unsigned short speed) = 0;
    virtual bool setAcceleration (unsigned char servo, unsigned short acceleration) = 0;
    virtual unsigned short getPosition (unsigned char servo) = 0;
    virtual unsigned char getMovingState () = 0;
};

class Pololu : public IPololu {
private:
    SerialCom serialCom_;
public:
    Pololu (const char* portName, unsigned int baudRate);
    bool setPosition (unsigned char servo, unsigned short position);
    bool setSpeed (unsigned char servo, unsigned short speed);
    bool setAcceleration (unsigned char servo, unsigned short acceleration);
    unsigned short getPosition (unsigned char servo);
    unsigned char getMovingState ();
};

#endif // POLOLU_HPP_INCLUDED
