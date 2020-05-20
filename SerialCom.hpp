#ifndef SERIALCOM_HPP_INCLUDED
#define SERIALCOM_HPP_INCLUDED

/**Abhaengig vom laufenden Betriebssystem werden die benoetigten Bibliotheken geladen.*/
#ifdef LINUX

#elif defined(WIN32)
    #include <windows.h>
#endif


/** \brief Interface fuer die serielle Kommunikation. Beinhaltet fuenf rein virtuelle Funktionen zum Oeffnen und Schliessen
 * einer seriellen Verbindung. Sowie das Schreiben und Lesen von Daten ueber die serielle Verbindung.
 *
 * \param
 * \param
 * \return
 *
 */
class ISerialCom {
public:
    virtual bool initSerialCom (const char* portName, unsigned int baudRate) = 0;
    virtual bool openSerialCom () = 0;
    virtual bool closeSerialCom () = 0;
    virtual bool writeSerialCom (unsigned char* command) = 0;
    //virtual unsigned char readSerialCom (unsigned char* command) = 0;
};

/** \brief Klasse erbt vom Interface ISerialCom und definiert die Funktionen.
 *
 * \param Portname (portName_)
 * \param Baudrate (baudRate_)
 * \param je nach Betriebssystem ein Handler für seriellen Port unter Windows bzw. ein File-Handler unter Linux
 *
 */
class SerialCom : public ISerialCom {
private:
    const char* portName_;
    unsigned int baudRate_;
    #ifdef LINUX

    #elif defined(WIN32)
        HANDLE port_;
    #endif
public:
    bool initSerialCom (const char* portName, unsigned int baudRate);
    bool openSerialCom ();
    bool closeSerialCom ();
    bool writeSerialCom (unsigned char* command);
    //unsigned char readSerialCom (unsigned char* command);
    const char* getPortName ();
    unsigned int getBaudRate ();
};

#endif // SERIALCOM_HPP_INCLUDED
