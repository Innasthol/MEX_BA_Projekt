#ifndef SERIALCOM_HPP_INCLUDED
#define SERIALCOM_HPP_INCLUDED

class ISerialCom {
public:
    virtual void initSerialCom(const char* portName, unsigned int baudRate) = 0;
    virtual bool openSerialCom() = 0;
    virtual bool closeSerialCom() = 0;
    virtual unsigned short writeSerialCom(unsigned char command[]) = 0;
};

class SerialCom : public ISerialCom {
private:
    const char* portName_;
    unsigned int baudRate_;
    int port_;
public:
    SerialCom();
    SerialCom(const char* portName, unsigned int baudRate);
    void initSerialCom(const char* portName, unsigned int baudRate);
    bool openSerialCom();
    bool closeSerialCom();
    unsigned short writeSerialCom(unsigned char command[]);
    int getPort();
};

#endif // SERIALCOM_HPP_INCLUDED
