#include "Pololu.hpp"
#include <stdlib.h>
#include <iostream>

Pololu::Pololu (const char* portName, unsigned int baudRate){
    serialCom_.initSerialCom(portName, baudRate);
}

/** \brief Dienst dem Oeffnen eines seriellen Ports. Dazu wird die Funktion openSerialCom
 * der Klasse SerialCom aufgerufen. Zuvor wird eine evtl. Verbindung ueber closeSerialCom geschlossen.
 *
 * \return Rueckgabewert ist der Rueckgabewert aus der Funktion closeSerialCom
 *
 */
bool Pololu::openConnection(){
    serialCom_.closeSerialCom();
    return serialCom_.openSerialCom();
}

/** \brief Dienst dem Schliessen des offenen seriellen Ports. Dazu wird die Funktion closeSerialCom
 * der Klasse SerialCom aufgerufen.
 *
 * \return Rueckgabewert ist der Rueckgabewert aus der Funktion closeSerialCom
 *
 */
bool Pololu::closeConnection(){
    return serialCom_.closeSerialCom();
}

/** \brief Funktion dient dem setzen der Position, die der angesprochener Servo anfahren soll.
 *
 * \param servo = 0-11 ansprechen eines einzelnen Servos
 * \param position = Position, die vom Servo angesteuert werden soll
 * \return Rueckgabewert 1 bei erfolgreichem Uebertragen des Positionskommandos an den Controller, ansonsten 0
 *
 */
bool Pololu::setPosition (unsigned char servo, unsigned short position){
    unsigned char command[4];
	BOOL success;

	// Compose the command.
	command[0] = 0x84;
	command[1] = servo;
	command[2] = position & 0x7F;
	command[3] = (position >> 7) & 0x7F;

	success = serialCom_.writeSerialCom(command);
	if (!success)
	{
		printf("WriteError: Schreiben auf seriellen Port fehlgeschlagen.");
		return 0;
	}
	return 1;
}

/** \brief Funktion dient dem setzen der Geschwindigkeit, mit der der angesprochener Servo sich bewegen soll.
 *
 * \param servo = 0-11 ansprechen eines einzelnen Servos
 * \param speed = Geschwindigkeit, die vom Servo zum bewegen nutzen soll (Eingabewert 1-10000, 0=höchste Geschwindigkeit)
 * \return Rueckgabewert 1 bei erfolgreichem Uebertragen des Geschwindigkeitskommandos an den Controller, ansonsten 0
 *
 */
bool Pololu::setSpeed (unsigned char servo, unsigned short speed){
    unsigned char command[4];
	BOOL success;

	// Compose the command.
	command[0] = 0x87;
	command[1] = servo;
	command[2] = speed & 0x7F;
	command[3] = (speed >> 7) & 0x7F;

	success = serialCom_.writeSerialCom(command);
	if (!success)
	{
		printf("WriteError: Schreiben auf seriellen Port fehlgeschlagen.");
		return 0;
	}
	return 1;
}

/** \brief Funktion dient dem setzen der Beschleunigung, die der angesprochener Servo zum Anfahren nutzen soll.
 *
 * \param servo = 0-11 ansprechen eines einzelnen Servos
 * \param acceleration = Beschleunigung, die vom Servo zum Anfahren genutzt werden soll (Eingabewert 1-255, 0=höchste Beschleunigung)
 * \return Rueckgabewert 1 bei erfolgreichem Uebertragen des Beschleunigungskommandos an den Controller, ansonsten 0
 *
 */
bool Pololu::setAcceleration (unsigned char servo, unsigned short acceleration){
    unsigned char command[4];
	BOOL success;

	// Compose the command.
	command[0] = 0x89;
	command[1] = servo;
	command[2] = acceleration & 0x7F;
	command[3] = (acceleration >> 7) & 0x7F;

	success = serialCom_.writeSerialCom(command);
	if (!success)
	{
		printf("WriteError: Schreiben auf seriellen Port fehlgeschlagen.");
		return 0;
	}
	return 1;
}

unsigned int Pololu::getPosition (unsigned char servo){

}

bool Pololu::getMovingState (){

}
