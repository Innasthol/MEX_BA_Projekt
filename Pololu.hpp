#ifndef POLOLU_HPP_INCLUDED
#define POLOLU_HPP_INCLUDED

#include "SerialCom.hpp"

/** \brief Interface
 *
 */
class IPololu {
public:
    virtual bool setPosition(unsigned short servo, unsigned short goToPosition) = 0;
    virtual bool setSpeed(unsigned short servo, unsigned short goToSpeed) = 0;
    virtual bool setAcceleration(unsigned short servo, unsigned short goToAcceleration) = 0;
    virtual unsigned short getPosition(unsigned short servo) = 0;
    virtual bool getMovingState() = 0;
};

/** \brief Klasse fuer ein Pololu-Objekt dass eine serielle Verbindung enthaelt und grundlegende
 * Funktionen fuer die Programmierung des Controllers bereitstellt.
 *
 */
class Pololu : public IPololu {
private:
    /**< Objekt einer SerialCom-Klasse */
    SerialCom serialCom;
public:
    /**< Kontruktor fuehrt serialCom.initSerialCom aus */
    Pololu(const char* portName, unsigned short baudRate);
    /**< Dient zum aendern der Verbindungdaten. Versetzt die serielle Verbindung in den selben Zustand wie der Konstrutor, jedoch mit neuem Portnamen und Baudrate */
    void initConnection(const char* portName, unsigned short baudRate);
    /**< Oeffnen und Schliessen. Funktionen rufen nur die Funktionen openSerialCom und closeSerialCom auf */
    bool openConnection();
    bool closeConnection();
    /**< Servo faehrt eine neue Position an */
    bool setPosition(unsigned short servo, unsigned short goToPosition);
    /**< Geschwindkeit fuer eine Servo wird gesetzt */
    bool setSpeed(unsigned short servo, unsigned short goToSpeed);
    /**< Beschleunigung fuer eine Servo wird gesetzt */
    bool setAcceleration(unsigned short servo, unsigned short goToAcceleration);
    /**< Liefert die aktuelle Position eines Servos */
    unsigned short getPosition(unsigned short servo);
    /**< Liefert den Bewegungsstatus aller Servos */
    bool getMovingState();
};

#endif // POLOLU_HPP_INCLUDED
