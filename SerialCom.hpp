#ifndef SERIALCOM_HPP_INCLUDED
#define SERIALCOM_HPP_INCLUDED

#ifdef _WIN32
    #include <windows.h>
#else

#endif

/** \brief Interface
 *
 */
class ISerialCom {
public:
    virtual void initSerialCom(const char* portName, unsigned short baudRate) = 0;
    virtual bool openSerialCom() = 0;
    virtual bool closeSerialCom() = 0;
    virtual short writeSerialCom(unsigned char command[]) = 0;
};

/** \brief Klasse fuer eine serielle Verbingung ueber einen COM-PORT
 *
 */
class SerialCom : public ISerialCom {
private:
    /**< COM-Port-Bezeichnung */
    const char* portName_;
    /**< Baudrate fuer die Kommunikation mit dem Controller */
    unsigned short baudRate_;
    /**< Je nach Betriebssystem wird eine Veriable fuer die serielle Verbingund benoetigt */
    #ifdef _WIN32
        HANDLE port_;
    #else
        int port_;
    #endif
public:
    /**< Kontruktoren */
    SerialCom();
    SerialCom(const char* portName, unsigned short baudRate);
    /**< Legt Portname und Baudrate fuer die serielle Verbindung fest */
    void initSerialCom(const char* portName, unsigned short baudRate);
    /**< Oeffnen und Schliessen der seriellen Verbindung */
    bool openSerialCom();
    bool closeSerialCom();
    /**< Uebertragen von Befehlen an den Controller */
    short writeSerialCom(unsigned char command[]);
    /**< Je nach Betriebssystem liefert die Funktion den aktiven Port */
    #ifdef _WIN32
        HANDLE getPort();
    #else
        int getPort();
    #endif
};

#endif // SERIALCOM_HPP_INCLUDED
