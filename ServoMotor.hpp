//============================================================================
// Name        : ServoMotor.hpp
// Author      : Willi Penner
//
// Description : Headerfile contains the interface IServoMotor and the class
//               ServoMotor with its functions to control a specific servo.
//============================================================================
#include "Pololu.hpp"
#include <cmath>

#ifndef SERVOMOTOR_HPP_
#define SERVOMOTOR_HPP_

/** \brief Interface to control a servo. The interface
 *  provides the basic functions for the control of a specific servo motor.
 *
 */
class IServoMotor {
public:
	virtual ~IServoMotor(){};
	virtual unsigned short getServoNumber() = 0;
	virtual unsigned short getMinPos() = 0;
	virtual unsigned short getMidPos() = 0;
	virtual unsigned short getMaxPos() = 0;
	virtual bool setPositionInAbs(unsigned short newPosition) = 0;
	virtual bool setPositionInDeg(short newPosition) = 0;
	virtual bool setPositionInRad(float newPosition) = 0;
	virtual bool setSpeed(unsigned short newSpeed) = 0;
	virtual bool setAccelaration(unsigned short newAcceleration) = 0;
	virtual unsigned short getPositionInAbs() = 0;
	virtual short getPositionInDeg() = 0;
	virtual float getPositionInRad() = 0;
	virtual void showPololuValues (unsigned short& min, unsigned short& mid, unsigned short& max) = 0;
};

/** \brief Class for the ServoMotor object
 *
 *	\param servoNumber_ = Port to which the servo is connected on the controller
 *	\param startingPosition_ = The center position of the servo, this position is determined in the Pololu Maestro Control Center.
 *	\param delta_ = Range of values that the servo can turn left and right.
 *	\param *connection_ = Pointer to an object of the type Pololu for the serial connection.
 */
class ServoMotor : public IServoMotor{
private:
	const short maxDeg = 90;				//maximum degree allowed
	const float maxRad = M_PI/2;			//maximum radiant allowed, M_PI is the constant from <cmath> for the number Pi
	const short maxSpeed = 255;				//maximum value for the speed
	const short maxAcceleration = 255;      //maximum value for the acceleration
	const short minSpeed = 1;				//minimum value for the speed
	const short minAcceleration = 1;  //minimum value for the acceleration
	const short conFactorDegToPos = 10;	    //conversion factor from degrees to position
	const short conFactorMyToPos = 4;		//conversion factor to convert µs (position value of a servo) to position values
	unsigned short servoNumber_;
	unsigned short startingPosition_;	//startPosition is the center position of a servo, in most cases it is value of 6000 (1500µs * 4)
	unsigned short delta_;
	Pololu *connection_ = NULL;
public:
	/** \brief ServoMotor class constructor. An object of the ServoMotor type must be initiated via the constructor.
	 *
	 * 	\param servo = is the slot number on the controller board to which the servo is connected
	 *  \param startingPosition = is the center position of the servo motor (must be determined in the Pololu Maestro Control Center)
	 *  \param delta = is the range of motion that the servo can reach from the center position
	 *  \param *connection = is a pointer to the Pololu object for the serial connection
	 *
	 * */
	ServoMotor(unsigned short servo, unsigned short startingPosition, unsigned short delta, Pololu *connection);

	/** \brief Returns the port to which the servo is connected to the controller board.
	 *
	 *  \return The return value is the servoNumber+1 because the internal counting of the controller starts at 0.
	 *
	 */
	unsigned short getServoNumber();

	/** \brief Returns the minimum position the servo is able to reach.
	 *
	 *  \return The return value is the set startingPosition minus the set delta
	 *
	 */
	unsigned short getMinPos ();

	/** \brief Returns the center position of the servo.
	 *
	 *  \return The return value is the set startingPosition.
	 *
	 */
	unsigned short getMidPos ();

	/** \brief Returns the maximum position the servo is able to reach.
	 *
	 *  \return The return value is the set startingPosition plus the set delta.
	 *
	 */
	unsigned short getMaxPos ();

	/** \brief Function causes the servo to move to a specific new position (position
	 *  is specified as an absolute value in a range from 5860 +- 3600).
	 *
	 *  \param newPosition as an absolut value
	 *
	 *  \return The return value is the return value of the setPosition funktion of the Pololu object.
	 */
	bool setPositionInAbs(unsigned short newPosition);

	/** \brief Function causes the servo to move to a specific new position (position is set with a value between -90 and 90 degrees).
	 *
	 *  \param newPosition in degrees
	 *
	 *  \return The return value is the return value of the setPosition funktion of the Pololu object.
	 */
	bool setPositionInDeg(short newPosition);

	/** \brief Function causes the servo to move to a specific new position (position is set with a radiant between -PI/2 and PI/2).
	 *
	 *  \param newPosition as radiant
	 *
	 *  \return The return value is the return value of the setPosition funktion of the Pololu object.
	 */
	bool setPositionInRad(float newPosition);

	/** \brief Sets the speed at which the servo should move (speed range is between 0 and 255)
	 *
	 *  \param newSpeed = Speed value (0 stands for the maximum speed of the servo, 1 is (0.25 microseconds) / (10 milliseconds),
	 *  255 is (63,75 microseconds) / (10 milliseconds))
	 *
	 *  \return The return value is the return value of the setSpeed funktion of the Pololu object.
	 */
	bool setSpeed(unsigned short newSpeed);

	/** \brief Sets the acceleration with which the set speed should be reached (acceleration range is between 0 and 255)
	 *
	 *  \param newAcceleration = Acceleration value (0 stands for the maximum acceleration of the servo, 1 is (0.25 microseconds) / (10 milliseconds) / (80 milliseconds),
	 *  255 is (63,75 microseconds) / (10 milliseconds) / (80 milliseconds))
	 *
	 *  \return The return value is the return value of the setAcceleration funktion of the Pololu object.
	 */
	bool setAccelaration(unsigned short newAcceleration);

	/** \brief Returns the position of the servo as absolute value.
	 *
	 *  \return The return value is the return value of the getPosition function of the Pololu object.
	 *
	 */
	unsigned short getPositionInAbs();

	/** \brief Returns the position of the servo in degrees.
	 *
	 *  \return The return value is the return value of the getPosition function of the Pololu object converted into degrees.
	 *
	 */
	short getPositionInDeg();

	/** \brief Returns the position of the servo as radiant.
	 *
	 *  \return The return value is the return value of the getPosition function of the Pololu object converted into radiant.
	 *
	 */
	float getPositionInRad();

	/** \brief Shows which settings have to be made in the Pololu Maestro Control Center for the servo,
	 *  based on the starting position and the delta for the specific servo.
	 */
	void showPololuValues (unsigned short& min, unsigned short& mid, unsigned short& max);
};

#endif /* SERVOMOTOR_HPP_ */
