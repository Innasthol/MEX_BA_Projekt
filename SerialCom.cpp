#include "SerialCom.hpp"
#include <stdlib.h>
#include <iostream>

/** \brief Um zur Laufzeit den Port zu aendern wird initSerialCom verwendet. Beim aufrufen der Funktion wird zuerst der aktuelle Port geschlossen, und im Anschluss mit neuen Portnamen und Baudrate initialisiert.
 * Im Anschluss kann der neu initialisierte Port über openSerialCom geöffnet werden.
 *
 * \param portName_
 * \param baudRate_
 * \return Konstruktor hat keinen Rueckgabewert
 *
 */
bool SerialCom::initSerialCom (const char* portName, unsigned int baudRate){
    closeSerialCom();
    portName_ = portName;
    baudRate_ = baudRate;
}

/** \brief Funktion dient dem oeffnen eines seriellen Ports mit den ueber den Konstruktor bereitgestellten Portnamen und der Baudrate.
 *         Die Funktion ist so aufgebaut, dass sie abhaengig vom Betriebssystem entweder ueber Windows bzw. ueber LINUX eine Verbindung aufbaut.
 *         LINUX ist noch nicht implementiert.
 *         Folge Arbeitsschritte werden beim Oeffnen einer seriellen Verbindung ausgefuehrt:
 *         1. COM-Port wird geoeffnet
 *         2.
 *
 * \param Es werden keine Uebergabeparameter benötigt.
 * \return Liefert einen Wahrheitswert, ob der Verbindungsaufbau funktioniert hat.
 *
 */
bool SerialCom::openSerialCom () {
    #ifdef LINUX
        std::cout << "Es läuft Linux" << std::endl;
    #elif defined(WIN32) //Windows
        DCB commState;
        BOOL success;
        COMMTIMEOUTS timeouts;

        /**Oeffnen eines COM-Ports.*/
        port_ = CreateFile (portName_, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (port_ == INVALID_HANDLE_VALUE) {
            switch (GetLastError()) {
                case ERROR_ACCESS_DENIED:
                    fprintf(stderr, "openSerialCom: Zugriff verweigert. Port wird bereits verwendet.\n");
                    break;
                case ERROR_FILE_NOT_FOUND:
                    fprintf(stderr, "openSerialCom: Port konnte nicht gefunden werden.  "
                    "Ueberpruefen, ob \"%s\" der korrekte Portname ist.  ", portName_);
                    break;
                default:
                    fprintf(stderr, "openSerialCom: Unable to open serial port.");
                    break;
            }
            return INVALID_HANDLE_VALUE;
        }

        /* Set the timeouts. */
        success = GetCommTimeouts(port_, &timeouts);
        if (!success)
        {
            fprintf(stderr, "Error: Unable to get comm timeouts.\n");
            CloseHandle(port_);
            return INVALID_HANDLE_VALUE;
        }
        timeouts.ReadIntervalTimeout = 1000;
        timeouts.ReadTotalTimeoutConstant = 1000;
        timeouts.ReadTotalTimeoutMultiplier = 0;
        timeouts.WriteTotalTimeoutConstant = 1000;
        timeouts.WriteTotalTimeoutMultiplier = 0;
        success = SetCommTimeouts(port_, &timeouts);
        if (!success)
        {
            fprintf(stderr, "Error: Unable to set comm timeouts\n");
            CloseHandle(port_);
            return INVALID_HANDLE_VALUE;
        }

        /**Auslesen der Statusinformationen vom Controller*/
        success = GetCommState(port_, &commState);
        if (!success)
        {
            fprintf(stderr, "openSerialCom: Laden der Statusinformationen fehlgeschlagen.");
            CloseHandle(port_);
            return INVALID_HANDLE_VALUE;
        }

        /**Setzen der Baudrate und schreiben auf den Controller*/
        commState.BaudRate = baudRate_;
        success = SetCommState(port_, &commState);
        if (!success)
        {
            fprintf(stderr, "openSerialCom: Setzen der Statusinformationen fehlgeschlagen.");
            CloseHandle(port_);
            return INVALID_HANDLE_VALUE;
        }

        /**Leeren des Buffers.*/
        success = FlushFileBuffers(port_);
        if (!success)
        {
            fprintf(stderr, "openSerialCom: Löschen des Buffers fehlgeschlagen.");
            CloseHandle(port_);
            return INVALID_HANDLE_VALUE;
        }
        return true;
	#endif
}

/** \brief Funktion dient dem Schliessen der seriellen Verbindung. Abhaengig vom Betriebssystem wird der jeweilige
 * Handler geschlossen.
 *
 * \param
 * \param
 * \return Bei erfolgreichem schliessen des Ports liefert CloseHandle eine Wert ungleich 0, bei einem Fehler liefert sie 1.
 *
 */
bool SerialCom::closeSerialCom () {
    #ifdef LINUX

    #elif defined(WIN32)
        return CloseHandle(port_);
    #endif // LINUX
}

/** \brief Funktion dient dem Schreiben auf der seriellen Verbindung, die mit openSerialPort geoeffnet wurde.
 *
 * \param Als Parameter wird der Funktion eine Anweisung uebergeben, die das vom Controller auszufuehrende Kommando enthält.
 * \return Rueckgabewert der Funktion ist ein Wahrheitswert, der angibt, ob das Schreiben ueber die serielle Verbindung erfolgreich war oder nicht.
 *
 */
bool SerialCom::writeSerialCom (unsigned char* command) {
    /**bytesTransfered liefert von WriteFile die Anzahl der Uebertragenen Bytes. */
    DWORD bytesTransfered;

    return WriteFile(port_, command, sizeof(command), &bytesTransfered, NULL);
};

/*unsigned char* SerialCom::readSerialCom (unsigned char* command) {
    DWORD bytesTransfered;
    unsigned char response[2];
    bool success;
    bool read;

    success = writeSerialCom(command);
    read = ReadFile(port_, response, sizeof(response), &bytesTransfered, NULL);

    return response;
};*/

/** \brief Funktion dient dem anzeigen, welcher Portname fuer die serielle Verbindung gewaehlt wurde.
 *
 * \return Liefert als Rueckgabewert den gesetzten Portname als "const char".
 *
 */
const char* SerialCom::getPortName (){
    return portName_;
}

/** \brief Funktion dient dem anzeigen, welche Baudrate fuer die serielle Verbindung gewaehlt wurde.
 *
 * \return Liefert als Rueckgabewert die gesetzte Baudrate als "unsigned int".
 *
 */
unsigned int SerialCom::getBaudRate (){
    return baudRate_;
}

