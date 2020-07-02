//============================================================================
// Name        : ServoMotor.hpp
// Author      : Willi Penner
//
// Description : ServoMotor source file. It contains the definition of the
//               functions of the ServoMotor class.
//============================================================================
#include "ServoMotor.hpp"
#include "Pololu.hpp"
#include <iostream>
#include <cmath>

/** \brief ServoMotor class constructor. An object of the ServoMotor type must be initiated via the constructor.
 *
 * 	\param servo = is the slot number on the controller board to which the servo is connected
 *  \param startingPosition = is the center position of the servo motor (must be determined in the Pololu Maestro Control Center)
 *  \param delta = is the range of motion that the servo can reach from the center position
 *  \param *connection = is a pointer to the Pololu object for the serial connection
 *
 * */
ServoMotor::ServoMotor(unsigned short servo, unsigned short startingPosition, unsigned short delta, Pololu *connection){
	servoNumber_ = servo-1;
	startingPosition_ = startingPosition;
	delta_ = delta;
	connection_ = connection;
}

/** \brief Returns the port to which the servo is connected to the controller board.
 *
 *  \return The return value is the servoNumber+1 because the internal counting of the controller starts at 0.
 *
 */
unsigned short ServoMotor::getServoNumber(){
	return servoNumber_+1;
}

/** \brief Returns the minimum position the servo is able to reach.
 *
 *  \return The return value is the set startingPosition minus the set delta
 *
 */
unsigned short ServoMotor::getMinPos (){
	return startingPosition_ - delta_ ;
}

/** \brief Returns the center position of the servo.
 *
 *  \return The return value is the set startingPosition.
 *
 */
unsigned short ServoMotor::getMidPos (){
	return startingPosition_;
}

/** \brief Returns the maximum position the servo is able to reach.
 *
 *  \return The return value is the set startingPosition plus the set delta.
 *
 */
unsigned short ServoMotor::getMaxPos (){
	return startingPosition_ + delta_ ;
}

/** \brief Function causes the servo to move to a specific new position (position
 *  is specified as an absolute value in a range from 5860 +- 3600).
 *
 *  \param newPosition as an absolut value
 *
 *  \return The return value is the return value of the setPosition funktion of the Pololu object.
 */
bool ServoMotor::setPositionInAbs(unsigned short newPosition){
	if (newPosition > (startingPosition_ + delta_) || newPosition < (startingPosition_ - delta_)){
		throw std::string("ServoMotor::setPositionInAbs: Abs position is out of range (startingPosition +- delta).");
	}else{
		return connection_->setPosition(servoNumber_, newPosition);
	}
}

/** \brief Function causes the servo to move to a specific new position (position is set with a value between -90 and 90 degrees).
 *
 *  \param newPosition in degrees
 *
 *  \return The return value is the return value of the setPosition funktion of the Pololu object.
 */
bool ServoMotor::setPositionInDeg(short newPosition){
	if (newPosition > 90 || newPosition < -90){
		throw std::string("ServoMotor::setPositionInDeg: Degree is out of range (-90 - 90).");
	}else{
		return connection_->setPosition(servoNumber_, startingPosition_ + newPosition * 10 * 4);
	}
}

/** \brief Function causes the servo to move to a specific new position (position is set with a radiant between -PI/2 and PI/2).
 *
 *  \param newPosition as radiant
 *
 *  \return The return value is the return value of the setPosition funktion of the Pololu object.
 */
bool ServoMotor::setPositionInRad(float newPosition){
	if ((int)(100 * newPosition) > (int)(100 * (M_PI/2)) || (int)(100 * newPosition) < (int)(100 * (-M_PI/2))){
		throw std::string("ServoMotor::setPositionInRad: Radiant is out of range (-PI/2 - +PI/2).");
	}else{
		return connection_->setPosition(servoNumber_, (startingPosition_ + (newPosition * 180 / M_PI) * 10 * 4));
	}
}

/** \brief Sets the speed at which the servo should move (speed range is between 0 and 255)
 *
 *  \param newSpeed = Speed value (0 stands for the maximum speed of the servo, 1 is (0.25 microseconds) / (10 milliseconds),
 *  255 is (63,75 microseconds) / (10 milliseconds))
 *
 *  \return The return value is the return value of the setSpeed funktion of the Pololu object.
 */
bool ServoMotor::setSpeed(unsigned short newSpeed){
	if (newSpeed > 255 || newSpeed < 0){
		throw std::string("ServoMotor::setSpeed: Speed is out of range (0 - 255).");
	}else{
		return connection_->setSpeed(servoNumber_, newSpeed);
	}
}

/** \brief Sets the acceleration with which the set speed should be reached (acceleration range is between 0 and 255)
 *
 *  \param newAcceleration = Acceleration value (0 stands for the maximum acceleration of the servo, 1 is (0.25 microseconds) / (10 milliseconds) / (80 milliseconds),
 *  255 is (63,75 microseconds) / (10 milliseconds) / (80 milliseconds))
 *
 *  \return The return value is the return value of the setAcceleration funktion of the Pololu object.
 */
bool ServoMotor::setAccelaration(unsigned short newAcceleration){
	if (newAcceleration > 255 || newAcceleration < 0){
		throw std::string("ServoMotor::setAcceleration: Acceleration is out of range (0 - 255).");
	}else{
		return connection_->setAcceleration(servoNumber_, newAcceleration);
	}
}

/** \brief Returns the position of the servo as absolute value.
 *
 *  \return The return value is the return value of the getPosition function of the Pololu object.
 *
 */
unsigned short ServoMotor::getPositionInAbs(){
	return connection_->getPosition(servoNumber_);
}

/** \brief Returns the position of the servo in degrees.
 *
 *  \return The return value is the return value of the getPosition function of the Pololu object converted into degrees.
 *
 */
short ServoMotor::getPositionInDeg(){
	return (connection_->getPosition(servoNumber_) / 4 - startingPosition_) / 10;
}

/** \brief Returns the position of the servo as radiant.
 *
 *  \return The return value is the return value of the getPosition function of the Pololu object converted into radiant.
 *
 */
float ServoMotor::getPositionInRad(){
	return (connection_->getPosition(servoNumber_) / 4 - startingPosition_) * M_PI / 10 / 180;
}

/** \brief Shows which settings have to be made in the Pololu Maestro Control Center for the servo,
 *  based on the starting position and the delta for the specific servo.
 */
void ServoMotor::showPololuValues (){
	std::cout << "Pololu values for the servo channel settings at port " << servoNumber_ + 1 << ": \n";
	std::cout << "Min:          " << (startingPosition_ - delta_) / 4 << " \xB5s \n";
	std::cout << "Max:          " << (startingPosition_ + delta_) / 4 << " \xB5s \n";
	std::cout << "8bit neutral: " << (startingPosition_) / 4 << " \xB5s \n\n";
}
