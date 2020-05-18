#ifndef SERIALCOM_HPP_INCLUDED
#define SERIALCOM_HPP_INCLUDED

#ifdef LINUX

#elif defined(WIN32)
    #include <windows.h>
#endif

class ISerialCom {
public:
    virtual bool openSerialCom () = 0;
    virtual bool closeSerialCom () = 0;
    virtual bool writeSerialCom (unsigned char* command) = 0;
    virtual bool readSerialCom (unsigned char* response) = 0;
};

class SerialCom : public ISerialCom {
private:
    const char* portName_;
    unsigned int baudRate_;
    #ifdef LINUX

    #elif defined(WIN32)
        HANDLE port_;
    #endif
public:
    SerialCom (const char* portName, unsigned int baudRate);
    bool openSerialCom ();
    bool closeSerialCom ();
    bool writeSerialCom (unsigned char* command);
    bool readSerialCom (unsigned char* response);
};

#endif // SERIALCOM_HPP_INCLUDED
