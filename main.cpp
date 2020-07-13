//============================================================================
// Name        : main.cpp
// Author      : Willi Penner
//
// Description : Contains the main function. It also contains test functions
//               to test the connection to the controller and the movement of
//       		 the servos.
//============================================================================
#include "TestUnits.hpp"
#include <iostream>

int main()
{
	//testOpenClose();

	//testSetGetMethods();

	//testMEXMovementSetting1();
    //testMEXMovementSetting2();

	//Einzeltest für SerialCom und Pololu
	//  testSerialCom();
	testPololu();
}
