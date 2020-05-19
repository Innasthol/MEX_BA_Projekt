#include "Pololu.hpp"
#include <stdlib.h>
#include <iostream>

Pololu::Pololu (const char* portName, unsigned int baudRate){
    serialCom_.initSerialCom(portName, baudRate);
}

bool Pololu::setPosition (unsigned char servo, unsigned short position){
    unsigned char command[4];
	BOOL success;

	// Compose the command.
	command[0] = 0x84;
	command[1] = servo;
	command[2] = position & 0x7F;
	command[3] = (position >> 7) & 0x7F;

    serialCom_.openSerialCom();
	success = serialCom_.writeSerialCom(command);
	if (!success)
	{
		fprintf(stderr, "WriteError: Schreiben auf seriellen Port fehlgeschlagen.");
		return 0;
	}else{
        printf("Write ausgefuehrt\n");
	}
	serialCom_.closeSerialCom();
	return 1;
}

bool Pololu::setSpeed (unsigned char servo, unsigned short speed){

}

bool Pololu::setAcceleration (unsigned char servo, unsigned short acceleration){

}

unsigned short Pololu::getPosition (unsigned char servo){

}

unsigned char Pololu::getMovingState (){

}
