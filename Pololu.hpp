#ifndef POLOLU_HPP_INCLUDED
#define POLOLU_HPP_INCLUDED

#include "SerialCom.hpp"

class IPololu {
public:
    virtual bool openConnection();
    virtual bool closeConnection();
    virtual bool setPosition(unsigned int servo, unsigned short goToPosition) = 0;
    virtual bool setSpeed(unsigned int servo, unsigned short goToSpeed) = 0;
    virtual bool setAcceleration(unsigned int servo, unsigned short goToAcceleration) = 0;
    virtual unsigned short getPosition(unsigned int servo) = 0;
    virtual bool getMovingState() = 0;
};

class Pololu : public IPololu {
private:
    SerialCom serialCom;
public:
    Pololu(const char* portName, unsigned short baudRate);
    bool openConnection();
    bool closeConnection();
    bool setPosition(unsigned int servo, unsigned short goToPosition);
    bool setSpeed(unsigned int servo, unsigned short goToSpeed);
    bool setAcceleration(unsigned int servo, unsigned short goToAcceleration);
    unsigned short getPosition(unsigned int servo);
    bool getMovingState();
};

#endif // POLOLU_HPP_INCLUDED
