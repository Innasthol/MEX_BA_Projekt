//============================================================================
// Name        : Pololu.hpp
// Author      : Willi Penner
//
// Description : Pololu header file. It contains the declaration of the
//               IPololu interface and the Pololu class
//============================================================================
#ifndef POLOLU_HPP_INCLUDED
#define POLOLU_HPP_INCLUDED

#include "SerialCom.hpp"

/** \brief Interface to control a Pololu controller. The interface
 *  provides the basic functions for the control of servo motors.
 *
 */
class IPololu {
public:
	virtual ~IPololu(){};
    virtual bool setPosition(unsigned short servo, unsigned short goToPosition) = 0;
    virtual bool setSpeed(unsigned short servo, unsigned short goToSpeed) = 0;
    virtual bool setAcceleration(unsigned short servo, unsigned short goToAcceleration) = 0;
    virtual unsigned short getPosition(unsigned short servo) = 0;
    virtual bool getMovingState() = 0;
};

/** \brief Class for a Pololu object that contains a serial connection and provides basic functions
 *  for programming the controller.
 *
 *  \param serialCom = Is an object of the SerialCom class without initialization of port name and baud rate, the "port_" is NULL.
 */
class Pololu : public IPololu {
private:
    SerialCom serialCom;
public:
    /** \brief Constructor executes serialCom.initSerialCom to initialize the serial connection. An object of the Pololu class must be
     *  initialized with the port name and baud rate using the constructor.
     *
     *  \param portName : The port name is used to open a serial connection via the port name for the controller specified by the operating system.
     *  \param baudRate : The baud rate determines the transmission speed at which communication between the PC and controller takes place.
     *
     */
    Pololu(const char* portName, unsigned short baudRate);

    /** \brief Used to change the connection data. Sets the serial connection in the same state as the constructor, but with a new port name and baud rate
     *
     *  \param portName : The port name is used to open a serial connection via the port name for the controller specified by the operating system.
     *  \param baudRate : The baud rate determines the transmission speed at which communication between the PC and controller takes place.
     *
     */
    void initConnection(const char* portName, unsigned short baudRate);

    /** \brief Functions are used to open and close the serial connection. Functions only call
     * the openSerialCom and closeSerialCom functions of the serialCom object.
     *
     * \return Value is 1 when opening or closing was successful and 0 when an error occured.
     *
     */
    bool openConnection();
    bool closeConnection();

    /** \brief Funktion is used to move a specific servo to a new position.
     *
     *  \param servo = Servo to move
     *  \param goToPosition = New position to be approached. The value for the new position is calculated from the position in microseconds times 4 (e.g. new position should be 1500 microseconds, then 6000 must be set in the function as new position)
     *
     *  \return The return value of the function is 1 if the new position was successfully set and 0 if an error occurred.
     *
     */
    bool setPosition(unsigned short servo, unsigned short goToPosition);

    /** \brief Function is used to set the speed for a servo with which it should move.
     *
     *  \param servo = Servo to set speed
     *  \param goToSpeed = Speed of the servo (speed value 1 = 0.25us / 10ms or speed value 100 = 25us / 10ms). A speed value of 0 means infinite speed, i.e. the maximum speed of the servo.
     *
     *  \return The return value of the function is 1 if the new speed was successfully set and 0 if an error occurred.
     *
     */
    bool setSpeed(unsigned short servo, unsigned short goToSpeed);

    /** \brief Function is used to set the acceleration for a servo with which it should reach the set speed.
     *
     *  \param servo = Servo to set acceleration
     *  \param goToAcceleration = Acceleration of the servo (acceleration value 1 = 0.25us / 10ms / 80ms or speed value 100 = 25us / 10ms / 80ms). A speed value of 0 means infinite acceleration, i.e. the maximum acceleration of the servo.
     *
     *  \return The return value of the function is 1 if the new acceleration was successfully set and 0 if an error occurred.
     *
     */
    bool setAcceleration(unsigned short servo, unsigned short goToAcceleration);

    /** \brief Function is used to read out the current position of a particular servo.
     *
     * 	\param servo = Servo whose current position is to be read out.
     *
     *	\return The return value is the current position of the selected servo. The position value supplied by the controller must still be multiplied by 4.
     *
     */
    unsigned short getPosition(unsigned short servo);

    /** \brief Function provides the movement status of all connected servos.
     *
     *	\return The return value is 1 while a servo is still in motion and 0 when all servos are at a standstill.
     *
     */
    bool getMovingState();
};

#endif // POLOLU_HPP_INCLUDED
