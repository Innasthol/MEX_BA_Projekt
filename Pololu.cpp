#include "Pololu.hpp"
#include "SerialCom.hpp"
#include <windows.h>

/** \brief Kontruktor der Pololu-Klasse. Initialiesiert die serielle Schnittstelle durch Aufrufen
 * der initSerialCom der SerialCom-Klasse.
 *
 * \param portName
 * \param baudRate
 *
 */
Pololu::Pololu(const char* portName, unsigned short baudRate){
    serialCom.initSerialCom(portName, baudRate);
}

/** \brief Oeffnet durch Aufrufen der openSerialCom() eine serielle Verbindung.
 *
 * \return
 *
 */
bool Pololu::openConnection(){
    return serialCom.openSerialCom();
}

/** \brief Schliesst durch Aufrufen der closeSerialCom() eine serielle Verbindung.
 *
 * \return
 *
 */
bool Pololu::closeConnection(){
    return serialCom.closeSerialCom();
}

/** \brief Dient dem Anfahren einer Position. Ruft dazu die Funktion writeSerialCom() der SerialCom-Klasse auf.
 *
 * \param servo = anzusprechender Servo
 * \param goToPosition = Position, die der gewaehlte Servo anfahren soll
 * \return Rueckgabewert, ob das Schreiben funktioniert hat
 *
 */
bool Pololu::setPosition(unsigned int servo, unsigned short goToPosition){
    /* Generiert den Befehl fuer den Pololu
     * 0x84 = Kommando fuer das setzen der Position
     * servo = Anzusprechender Servo 0 - 11
     * goToPositiion = aufgeteilt auf 2 Byte, zuerst die low bits, dann die high bits
     */
    unsigned char command[] = {0x84, servo, goToPosition & 0x7F, goToPosition >> 7 & 0x7F};
    serialCom.writeSerialCom(command);
}

void Pololu::initConnection(const char* portName, unsigned short baudRate){
    serialCom.initSerialCom(portName, baudRate);
}

/** \brief Dient dem Setzen der Geschwindikeit mit der sich der Servo drehen soll.
 * Ruft dazu die Funktion writeSerialCom() der SerialCom-Klasse auf.
 *
 * \param servo = anzusprechender Servo
 * \param goToSpeed = Geschwindigkeit, die der gewaehlte Servo annehmen soll
 * \return Rueckgabewert, ob das Schreiben funktioniert hat
 *
 */
bool Pololu::setSpeed(unsigned int servo, unsigned short goToSpeed){
    /* Generiert den Befehl fuer den Pololu
     * 0x87 = Kommando fuer das setzen der Geschwindigkeit
     * servo = Anzusprechender Servo 0 - 11
     * goToSpeed = aufgeteilt auf 2 Byte, zuerst die low bits, dann die high bits
     */
    unsigned char command[] = {0x87, servo, goToSpeed & 0x7F, goToSpeed >> 7 & 0x7F};
    serialCom.writeSerialCom(command);
}

/** \brief Dient dem Setzen der Beschleunigung mit der der Servo die gesetzte Geschwindigkeit erreicht.
 * Ruft dazu die Funktion writeSerialCom() der SerialCom-Klasse auf.
 *
 * \param servo = anzusprechender Servo
 * \param goToAcceleration = Beschleunigung, die der gewaehlte Servo annehmen soll
 * \return Rueckgabewert, ob das Schreiben funktioniert hat
 *
 */
bool Pololu::setAcceleration(unsigned int servo, unsigned short goToAcceleration){
    /* Generiert den Befehl fuer den Pololu
     * 0x89 = Kommando fuer das setzen der Beschleunigung
     * servo = Anzusprechender Servo 0 - 11
     * goToAcceleration = aufgeteilt auf 2 Byte, zuerst die low bits, dann die high bits
     */
    unsigned char command[] = {0x89, servo, goToAcceleration & 0x7F, goToAcceleration >> 7 & 0x7F};
    serialCom.writeSerialCom(command);
}

/** \brief Dient dem Ermitteln der aktuellen Position eines Servos.
 * Ruft dazu die Funktion writeSerialCom() der SerialCom-Klasse auf.
 *
 * \param servo = anzusprechender Servo
 * \return Aktuelle Position des Servos als Unsigned Short.
 *
 */
unsigned short Pololu::getPosition(unsigned int servo){
    /* Generiert den Befehl fuer den Pololu
     * 0x90 = Kommando fuer das auslesen der Position
     * servo = Anzusprechender Servo 0 - 11
     */
    unsigned char command[] = {0x90, servo};
    return serialCom.writeSerialCom(command);
}

/** \brief Dient dem Ermitteln sich eines der angeschlossenen Servos noch in Bewegung ist.
 * Ruft dazu die Funktion writeSerialCom() der SerialCom-Klasse auf.
 *
 * \param servo = anzusprechender Servo
 * \return Aktuelle Position des Servos als Unsigned Short.
 *
 */
bool Pololu::getMovingState(){
    /* Generiert den Befehl fuer den Pololu
     * 0x93 = Kommando fuer das auslesen der Position
     */
    unsigned char command[] = {0x93};
    return serialCom.writeSerialCom(command);
}
