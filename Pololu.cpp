#include "Pololu.hpp"
#include "SerialCom.hpp"
#include <string>
#include <iostream>

/** \brief Pololu class constructor. Initializes the serial interface by calling initSerialCom of the SerialCom class.
 *
 * \param portName (COM-Portname: e.g. COM4)
 * \param baudRate
 *
 */
Pololu::Pololu(const char* portName, unsigned short baudRate){
    serialCom.initSerialCom(portName, baudRate);
}

/** \brief Opens a serial connection by calling the openSerialCom () function of the SerialCom class.
 *
 * \return
 *
 */
bool Pololu::openConnection(){
    try{
        serialCom.openSerialCom();
    }catch(std::string errorMessage){
        std::cout << errorMessage;
        return 0;
    }
    return 1;
}

/** \brief Closes a serial connection by calling closeSerialCom ().
 *
 * \return
 *
 */
bool Pololu::closeConnection(){
    try{
        serialCom.closeSerialCom();
    }catch(std::string errorMessage){
        std::cout << errorMessage;
        return 0;
    }
    return 1;
}

void Pololu::initConnection(const char* portName, unsigned short baudRate){
    serialCom.initSerialCom(portName, baudRate);
}

/** \brief Moves servo to a assumed position.
 *
 * \param servo = servo to address
 * \param goToPosition = Position that the selected servo should assume
 * \return The return value is 1 if writing was successfully, 0 if the was an error while writing
 *
 */
bool Pololu::setPosition(unsigned short servo, unsigned short goToPosition){
    /* Generates the command for the controller.
     * 0x84 = Command for setting the position
     * servo = servo to address as a transfer parameter
     * goToPositiion = divided into 2 bytes, first the low bits, then the high bits
     */
    unsigned short sizeCommand = 4;
    unsigned char command[] = {0x84, (unsigned char)servo, (unsigned char)(goToPosition & 0x7F), (unsigned char)((goToPosition >> 7) & 0x7F)};
    try{
        serialCom.writeSerialCom(command, sizeCommand, NULL, 0);
    }catch(std::string errorMessage){
        throw std::string("Pololu::setPosition: " + errorMessage);
        return FALSE;
    }catch(...){
        throw std::string("Pololu::setPosition: Unknown error, while writing to port.");
        return FALSE;
    }
    return TRUE;
}

/** \brief Sets the speed at which the servo should turn.
 *
 * \param servo = servo to address
 * \param goToSpeed = Speed that the selected servo should assume
 * \return The return value is 1 if writing was successfully, 0 if the was an error while writing
 *
 */
bool Pololu::setSpeed(unsigned short servo, unsigned short goToSpeed){
    /* Generates the command for the controller.
     * 0x87 = Command for setting the speed
     * servo = servo to address as a transfer parameter
     * goToSpeed = divided into 2 bytes, first the low bits, then the high bits
     */
    unsigned short sizeCommand = 4;
    unsigned char command[] = {0x87, (unsigned char)servo, (unsigned char)(goToSpeed & 0x7F), (unsigned char)((goToSpeed >> 7) & 0x7F)};
    try{
        serialCom.writeSerialCom(command, sizeCommand, NULL, 0);
    }catch(std::string errorMessage){
        throw std::string("Pololu::setSpeed: " + errorMessage);
        return FALSE;
    }catch(...){
        throw std::string("Pololu::setSpeed: Unknown error, while writing to port.");
        return FALSE;
    }
    return TRUE;
}

/** \brief Sets the acceleration with which the servo reaches the set speed.
 *
 * \param servo = servo to address
 * \param goToAcceleration = Acceleration that the selected servo should assume
 * \return The return value is 1 if writing was successfully, 0 if the was an error while writing
 *
 */
bool Pololu::setAcceleration(unsigned short servo, unsigned short goToAcceleration){
    /* Generates the command for the controller.
     * 0x89 = Command for setting the acceleration
     * servo = servo to address as a transfer parameter
     * goToAcceleration = divided into 2 bytes, first the low bits, then the high bits
     */
    unsigned short sizeCommand = 4;

    unsigned char command[] = {0x89, (unsigned char)servo, (unsigned char)(goToAcceleration & 0x7F), (unsigned char)((goToAcceleration >> 7) & 0x7F)};
    try{
        serialCom.writeSerialCom(command, sizeCommand, NULL, 0);
    }catch(std::string errorMessage){
        throw std::string("Pololu::setAcceleration: " + errorMessage);
        return FALSE;
    }catch(...){
        throw std::string("Pololu::setAcceleration: Unknown error, while writing to port.");
        return FALSE;
    }
    return TRUE;
}

/** \brief Determines the current position of a servo.
 *
 * \param servo = servo to address
 * \return The return value is the current position of the selected servo
 *
 */
unsigned short Pololu::getPosition(unsigned short servo){
    /* Generates the command for the controller.
     * 0x90 = Command for reading out the position
     * servo = servo to address as a transfer parameter
     */
    unsigned short sizeCommand = 2;
    unsigned short sizeResponse = 2;
    unsigned char response[sizeResponse];

    unsigned char command[] = {0x90, (unsigned char)servo};
    try{
        serialCom.writeSerialCom(command, sizeCommand, response, sizeResponse);
    }catch(std::string errorMessage){
        throw std::string("Pololu::getPosition: " + errorMessage);
    }catch(...){
        throw std::string("Pololu::getPosition: Unknown error, while writing/reading to port.");
    }

    return response[0] + 256 * response[1];
}

/** \brief Determines whether one of the connected servos is still in motion.
 *
 * \param servo = Suitable servo
 * \return The return value is 1 if a servo is still moving, 0 if there is no moving
 *
 */
bool Pololu::getMovingState(){
    /* Generates the command for the controller.
     * 0x93 = Command for reading out movements
     */
    unsigned short sizeCommand = 1;
    unsigned short sizeResponse = 1;
    unsigned char response[sizeResponse];

    unsigned char command[] = {0x93};
    try{
        serialCom.writeSerialCom(command, sizeCommand, response, sizeResponse);
    }catch(std::string errorMessage){
        throw std::string("Pololu::getMovingState: " + errorMessage);
    }catch(...){
        throw std::string("Pololu::getMovingState: Unknown error, while writing/reading to port.");
    }
    return response[0]; // The return value is 1 if a servo is still moving, 0 if there is no moving.
}
