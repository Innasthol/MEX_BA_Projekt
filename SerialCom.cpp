#include "SerialCom.hpp"
#include <fcntl.h>
#include <stdio.h>

#ifdef _WIN32
#define O_NOCTTY 0
#else
#include <termios.h>
#endif

SerialCom::SerialCom(){

}

/** \brief Kontruktor der Klasse SerialCom: Initialisiert eine serielle Verbindung
 *
 * \param portName (COM-Port-Name der vom Betriebssystem dem Pololu zugewiesen wird)
 * \param baudRate (Baudrate mit der die Communikation stattfinden soll)
 *
 */
SerialCom::SerialCom(const char* portName, unsigned int baudRate){
    portName_ = portName;
    baudRate_ = baudRate;
}

/** \brief Initialisiert eine serielle Verbindung. Versetzt die serielle Verbindung in den gleichen Zustand wie der Konstruktor.
 * Diese Funktion kann genutzt werden, um eine serielle Verbindung zu aendern bzw. auch die Baudrate zu veraendern.
 *
 * \param portName (COM-Port-Name der vom Betriebssystem dem Pololu zugewiesen wird)
 * \param baudRate (Baudrate mit der die Communikation stattfinden soll)
 *
 */
void SerialCom::initSerialCom(const char* portName, unsigned int baudRate){
    close(port_);
    portName_ = portName;
    baudRate_ = baudRate;
}

/** \brief Oeffnet eine serielle Verbindung zum COM-Port der bei der Initialisierung festgelegt wurde.
 *
 * \return Liefert einen boolschen Wert, ob das Oeffnen der seriellen Verbindung funktioniert hat oder nicht (1 = Verbindung geoeffnet; 0 = Verbindung fehlgeschlagen).
 *
 */
bool SerialCom::openSerialCom(){
    closeSerialCom();
    int port = open(portName_, O_RDWR | O_NOCTTY);
    if (port == -1){
        perror(portName_);
        return 0;
    }
    port_ = port;
    return 1;
}

/** \brief Schliesst die gewuenschte serielle Verbindung.
 *
 */
bool SerialCom::closeSerialCom(){
    return close(port_);
}

/** \brief Funktion sendet ein erstelltes Kommando an den Pololu.
 *
 * \param command (Das Kommando, das durch die Pololu-Klasse erstellt wird.
 * \return Liefert bei einem Fehler 0, bei erfolgreichen Set-Befehlen eine 1, und im Falle
 * von Get-Befehlen wird entweder die Position oder der Bewegungsstatus als Rueckgabewert zurueckgegeben.
 */
unsigned short SerialCom::writeSerialCom(unsigned char command[]){
    /**< Erstelltes Kommando wird an den Pololu ueber die offene serielle Schnittstelle uebertragen. */
    if(write(port_, command, sizeof(command)) == -1){
        perror("Schreiben fehlgeschlagen.");
        return 0;
    }
    /**< Falls das Kommando getPosition(0x90) oder getMovingState(0x93) entspricht, erfordert es ein empfangen der Imformationen vom Pololu. */
    if (command[0] == 0x90){
        unsigned char response[2];
        if(read(port_,response,2) != 2){
            perror("Lesen fehlgeschlagen.");
            return 0;
        }
        /**< Liefert die aktuell eingenommene Position des abgefragten Servos. */
        return response[0] + 256*response[1];
    }else if (command[0] == 0x93){
        unsigned char response[1];
        if(read(port_,response,1) != 1){
            perror("Lesen fehlgeschlagen.");
            return 0;
        }
        /**< Liefert den Bewegungsstatus aller angeschlossenen Servos. */
        return response[0];
    }
    return 1;
}

/** \brief
 *
 * \return
 *
 */
int SerialCom::getPort(){
    return port_;
}
