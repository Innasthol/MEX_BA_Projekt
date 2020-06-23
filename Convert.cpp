//============================================================================
// Name        : Convert.cpp
// Author      : Willi Penner
//
// Description : Contains functions to convert radians or degrees into
//               absolute position values and the other way.
//============================================================================
#include "Convert.hpp"
#include <string>
#include <iostream>
#include <cmath>

/** \brief Converts an float radiant into an absolute position for the servo controller
 *
 *	\param radiant = Radiant as a float (range is between -pi/2 and pi/2)
 *
 *	\return Return value is the absolut position for the servo as an unsigned short
 *
 * */
unsigned short RadToPos (float radiant){
    unsigned short position = 6000;
	if ((int)(100 * radiant) > (int)(100 * (M_PI/2)) || (int)(100 * radiant) < (int)(100 * (-M_PI/2))){
		throw std::string("Main::RadToPos: Radiant is out of range.");
	}else{
		position = (1500 + (radiant * 180 / M_PI * 10)) * 4;
		if (position < 2400){
			return 2400;
		}else{
			return position;
		}
	}
}

/** \brief Converts a short degree into an absolute position for the servo controller
 *
 *	\param degree = Degree as a short (range is between -90 and 90 degree)
 *
 *	\return Return value is the absolut position for the servo as an unsigned short
 *
 * */
unsigned short DegToPos (short degree){
	if (degree > 90 || degree < -90){
		throw std::string("Main::DegToPos: Degree is out of range.");
	}else{
		return (1500 + (degree * 10)) * 4;
	}
}

/** \brief Converts an unsigned short position into radiant
 *
 *	\param position = Position as a unsigned short (range is between 2400 and 9600)
 *
 *	\return Return value is the position in radiant
 *
 * */
float PosToRad (unsigned short position){
	if (position < 2400 || position > 9600){
		throw std::string("Convert::PosToRad: Position is out of range.");
	}else{
		return (position / 4 - 1500) * M_PI / 10 / 180;
	}
}

/** \brief Converts an unsigned short position into degree
 *
 *	\param position = Position as a unsigned short (range is between 2400 and 9600)
 *
 *	\return Return value is the position in degree
 *
 * */
short PosToDeg (unsigned short position){
	if (position < 2400 || position > 9600){
		throw std::string("Convert::PosToDeg: Position is out of range.");
	}else{
		return (position / 4 - 1500) / 10;
	}
}
