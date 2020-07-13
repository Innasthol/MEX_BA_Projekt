//============================================================================
// Name        : Pololu.cpp
// Author      : Willi Penner
//
// Description : Pololu source file. It contains the definition of the
//               functions of the Pololu class.
//============================================================================
#include "Pololu.hpp"
#include "SerialCom.hpp"
#include <string>
#include <iostream>

/** \brief Constructor executes serialCom.initSerialCom to initialize the serial connection. An object of the Pololu class must be
 *  initialized with the port name and baud rate using the constructor.
 *
 *  \param portName : The port name is used to open a serial connection via the port name for the controller specified by the operating system.
 *  \param baudRate : The baud rate determines the transmission speed at which communication between the PC and controller takes place.
 *
 */
Pololu::Pololu(const char* portName, unsigned short baudRate){serialCom.initSerialCom(portName, baudRate);}

/** \brief Function is used to open the serial connection. Function only calls
 * the openSerialCom function of the serialCom object.
 *
 * \return The return value is 1 when opening the port was successful, 0 when an error occurred.
 *
 */
bool Pololu::openConnection(){
    try{
        serialCom.openSerialCom();
    }catch (std::string &errorMessage){
        std::cout << errorMessage;
        return 0;
    }
    return 1;
}

/** \brief Function is used to close the serial connection. Function only calls
 * the closeSerialCom function of the serialCom object.
 *
 * \return The return value is 1 when closing the port was successful, 0 when an error occurred.
 *
 */
bool Pololu::closeConnection(){
    try{
        serialCom.closeSerialCom();
    }catch (std::string &errorMessage){
        std::cout << errorMessage;
        return 0;
    }
    return 1;
}

/** \brief Used to change the connection data. Sets the serial connection in the same state as the constructor, but with a new port name and baud rate
 *
 *  \param portName : The port name is used to open a serial connection via the port name for the controller specified by the operating system.
 *  \param baudRate : The baud rate determines the transmission speed at which communication between the PC and controller takes place.
 *
 */
void Pololu::initConnection(const char* portName, unsigned short baudRate){serialCom.initSerialCom(portName, baudRate);}

/** \brief Funktion is used to move a specific servo to a new position.
 *
 *  \param servo = Servo to move
 *  \param goToPosition = New position to be approached. The value for the new position is calculated from the position in microseconds times 4 (e.g. new position should be 1500 microseconds, then 6000 must be set in the function as new position)
 *
 *  \return The return value of the function is 1 if the new position was successfully set and 0 if an error occurred.
 *
 */
bool Pololu::setPosition(unsigned short servo, unsigned short goToPosition){
    /* Generates the command for the controller.
     * 0x84 = Pololu command for setting the position
     * servo = servo to address as a transfer parameter
     * goToPositiion = divided into 2 bytes, first the low bits, then the high bits
     */
    unsigned short sizeCommand = 4;
    unsigned char command[] = {0x84, (unsigned char)servo, (unsigned char)(goToPosition & 0x7F), (unsigned char)((goToPosition >> 7) & 0x7F)};
    try
    {
        serialCom.writeSerialCom(command, sizeCommand, NULL, 0);
    }
    catch (std::string &errorMessage)
    {
        throw std::string("Pololu::setPosition: " + errorMessage);
        return 0;
    }
    catch(...)
    {
        throw std::string("Pololu::setPosition: Unknown error, while writing to port.");
        return 0;
    }
    return 1;
}

/** \brief Function is used to set the speed for a servo with which it should move.
 *
 *  \param servo = Servo to set speed
 *  \param goToSpeed = Speed of the servo (speed value 1 = 0.25us / 10ms or speed value 100 = 25us / 10ms). A speed value of 0 means infinite speed, i.e. the maximum speed of the servo.
 *
 *  \return The return value of the function is 1 if the new speed was successfully set and 0 if an error occurred.
 *
 */
bool Pololu::setSpeed(unsigned short servo, unsigned short goToSpeed){
    /* Generates the command for the controller.
     * 0x87 = Pololu command for setting the speed
     * servo = servo to address as a transfer parameter
     * goToSpeed = divided into 2 bytes, first the low bits, then the high bits
     */
    unsigned short sizeCommand = 4;
    unsigned char command[] = {0x87, (unsigned char)servo, (unsigned char)(goToSpeed & 0x7F), (unsigned char)((goToSpeed >> 7) & 0x7F)};
    try
    {
        serialCom.writeSerialCom(command, sizeCommand, NULL, 0);
    }
    catch (std::string &errorMessage)
    {
        throw std::string("Pololu::setSpeed: " + errorMessage);
        return 0;
    }
    catch(...)
    {
        throw std::string("Pololu::setSpeed: Unknown error, while writing to port.");
        return 0;
    }
    return 1;
}

/** \brief Function is used to set the acceleration for a servo with which it should reach the set speed.
 *
 *  \param servo = Servo to set acceleration
 *  \param goToAcceleration = Acceleration of the servo (acceleration value 1 = 0.25us / 10ms / 80ms or speed value 100 = 25us / 10ms / 80ms). A speed value of 0 means infinite acceleration, i.e. the maximum acceleration of the servo.
 *
 *  \return The return value of the function is 1 if the new acceleration was successfully set and 0 if an error occurred.
 *
 */
bool Pololu::setAcceleration(unsigned short servo, unsigned short goToAcceleration){
    /* Generates the command for the controller.
     * 0x89 = Pololu command for setting the acceleration
     * servo = servo to address as a transfer parameter
     * goToAcceleration = divided into 2 bytes, first the low bits, then the high bits
     */
    unsigned short sizeCommand = 4;

    unsigned char command[] = {0x89, (unsigned char)servo, (unsigned char)(goToAcceleration & 0x7F), (unsigned char)((goToAcceleration >> 7) & 0x7F)};
    try
    {
        serialCom.writeSerialCom(command, sizeCommand, NULL, 0);
    }
    catch (std::string &errorMessage)
    {
        throw std::string("Pololu::setAcceleration: " + errorMessage);
        return 0;
    }
    catch(...)
    {
        throw std::string("Pololu::setAcceleration: Unknown error, while writing to port.");
        return 0;
    }
    return 1;
}

/** \brief Function is used to read out the current position of a particular servo.
 *
 * 	\param servo = Servo whose current position is to be read out.
 *
 *	\return The return value is the current position of the selected servo. The position value supplied by the controller must still be multiplied by 4.
 *
 */
unsigned short Pololu::getPosition(unsigned short servo){
    /* Generates the command for the controller.
     * 0x90 = Pololu command for reading out the position
     * servo = servo to address as a transfer parameter
     */
    unsigned short sizeCommand = 2;
    unsigned short sizeResponse = 2;
    unsigned char response[sizeResponse];

    unsigned char command[] = {0x90, (unsigned char)servo};
    try
    {
        serialCom.writeSerialCom(command, sizeCommand, response, sizeResponse);
    }
    catch (std::string &errorMessage)
    {
        throw std::string("Pololu::getPosition: " + errorMessage);
    }
    catch(...)
    {
        throw std::string("Pololu::getPosition: Unknown error, while writing/reading to port.");
    }

    return response[0] + 256 * response[1];
}

/** \brief Function provides the movement status of all connected servos.
 *
 *	\return The return value is 1 while a servo is still in motion and 0 when all servos are at a standstill.
 *
 */
bool Pololu::getMovingState(){
    /* Generates the command for the controller.
     * 0x93 = Pololu command for reading out the movement of all servos
     */
    unsigned short sizeCommand = 1;
    unsigned short sizeResponse = 1;
    unsigned char response[sizeResponse];

    unsigned char command[] = {0x93};
    try
    {
        serialCom.writeSerialCom(command, sizeCommand, response, sizeResponse);
    }
    catch (std::string &errorMessage)
    {
        throw std::string("Pololu::getMovingState: " + errorMessage);
    }
    catch(...)
    {
        throw std::string("Pololu::getMovingState: Unknown error, while writing/reading to port.");
    }
    return response[0]; // The return value is 1 if a servo is still moving, 0 if there is no moving.
}
