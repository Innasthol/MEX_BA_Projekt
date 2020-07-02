//============================================================================
// Name        : TestUnits.cpp
// Author      : Willi Penner
//
// Description :
//============================================================================
#include "Pololu.hpp"
#include "ServoMotor.hpp"
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

#ifdef _WIN32
    #include <windows.h>
#else
	#include <unistd.h>
#endif

/** \brief The function is used for waiting for a certain time, depending on the operating system Sleep () or usleep () is used.
 *
 *	\param milliseconds = Time to wait in milliseconds
 *
 */
void wait(unsigned long milliseconds){
	#ifdef _WIN32
		Sleep(milliseconds);
	#else
		usleep(milliseconds);
	#endif
}

/** \brief Function systematically tests the opening and closing of a serial connection
 *
 */
void testOpenClose (){
    //Define the port name and the baud rate
	#ifdef _WIN32
		const char* portName = "COM5";  		// Windows
	#else
		const char* portName = "/dev/ttyACM0";  // Linux
	#endif
	unsigned short baudRate = 9600;
    //Define a Pololu object
    Pololu conn(portName, baudRate);

    /**< Test 1 */
    //Open the serial port with the initialized port name and baud rate
    std::cout << "TEST_01 :: ";
    if (conn.openConnection()){
        std::cout << portName << ": Connection opened successfully" << std::endl;
    }
    //Open the same port again
    std::cout << "TEST_01 :: ";
    if (conn.openConnection()){
        std::cout << portName << ": Connection opened successfully" << std::endl;
    }

    /**< Test 2 */
    //Close the port
    std::cout << "TEST_02 :: ";
    if (conn.closeConnection()){
        std::cout << portName << ": Connection closed successfully" << std::endl;
    }
    //Close the same port again
    std::cout << "TEST_02 :: ";
    if (conn.closeConnection()){
        std::cout << portName << ": Connection closed successfully" << std::endl;
    }

    /**< Test 3 */
    //Reinitialize the serial port with an unknown name
    conn.initConnection("COM7", baudRate);
    //Open the newly initialized port
    std::cout << "TEST_03 :: ";
    if (conn.openConnection()){
        std::cout << "COM7" << ": Connection opened successfully" << std::endl;
    }

    /**< Test 4 */
    //Reinitialize the serial port with a known port, but not from the Pololu
    conn.initConnection("COM1", baudRate);
    //Open the port
    std::cout << "TEST_04 :: ";
    if (conn.openConnection()){
        std::cout << "COM1" << ": Connection opened successfully" << std::endl;
    }
    //Close the port
    std::cout << "TEST_04 :: ";
    if (conn.closeConnection()){
        std::cout << portName << ": Connection closed successfully" << std::endl;
    }

    /**< Test 5 */
    //Reinitialize the serial port with a Pololu known port
    conn.initConnection(portName, baudRate);
    for (int i = 0; i < 100; i++){
    	std::cout << "TEST_05_" << i << " :: ";
        if (conn.openConnection()){
            std::cout << portName << ": Connection opened successfully" << std::endl;
        }
    }
    conn.closeConnection();
}

/** \brief Function systematically tests the writing and reading of a serial connection.
 *  This function is not suitable for testing the methods on the robot manipulator.
 *  Please use it on a stand alone controller.
 */
void testSetGetMethods (){
	// Define the port name and the baud rate
	#ifdef _WIN32
		const char* portName = "COM4";  		// Windows
	#else
		const char* portName = "/dev/ttyACM0";  // Linux
	#endif
    // Defines a serial connections and opens it.
	Pololu conn(portName, 9600);
	conn.openConnection();

	// Defines three servomotors of two different types.
	// ServoMotor name(<portnumber on the controller>, <middle position of the servo>, <movement range in one direction>, <pointer to the serial connection>)
	ServoMotor SG_90_1(1, 5680, 3600, &conn);
	ServoMotor SG_90_2(2, 5680, 3600, &conn);
	ServoMotor DM1500_1(4, 5680, 3600, &conn);

	// Shows the min-, max-position and the 8bit-neutral for the servo channelsettings for the pololu controller.
	// The values have to be the same as in the Pololu Maestro Control Center.
	//
	// For example:
	// Pololu values for the servo channel settings at port 2:
	// Min:          520 µs
	// Max:          2320 µs
	// 8bit neutral: 1420 µs
	SG_90_1.showPololuValues();
	SG_90_2.showPololuValues();
	DM1500_1.showPololuValues();

	// Shows the min-,  mid- and max-position of the defined servos
	cout << "SERVO: SG_90_1 | SG_90_2 | DM1500_1 \n" << "Min:   " << SG_90_1.getMinPos() << "      " << SG_90_2.getMinPos() << "      " << DM1500_1.getMinPos()
			<< "\nMid:   " << SG_90_1.getMidPos() << "      " << SG_90_2.getMidPos() << "      " << DM1500_1.getMidPos() << "\nMax:   " << SG_90_1.getMaxPos()
			<< "      " << SG_90_2.getMaxPos() << "      " << DM1500_1.getMaxPos() << endl;

	// Set speed and acceleration for all servos
	SG_90_1.setSpeed(30);
	SG_90_1.setAccelaration(30);
	SG_90_2.setSpeed(30);
	SG_90_2.setAccelaration(30);
	DM1500_1.setSpeed(100);
	DM1500_1.setAccelaration(30);

	// Moves the servo 1 to the minimum or maximum position depending on the starting position.
	if (SG_90_1.getPositionInAbs() < SG_90_1.getMidPos()){
		SG_90_1.setPositionInAbs(SG_90_1.getMaxPos());
	}else{
		SG_90_1.setPositionInAbs(SG_90_1.getMinPos());
	}
	// Moves the servo 2 to the minimum or maximum position depending on the starting position.
	if (SG_90_2.getPositionInAbs() < SG_90_2.getMidPos()){
		SG_90_2.setPositionInAbs(SG_90_2.getMaxPos());
	}else{
		SG_90_2.setPositionInAbs(SG_90_2.getMinPos());
	}
	// Moves the servo 3 to the minimum or maximum position depending on the starting position (5 times).
	for (int i = 0; i < 5; i++){
		if (DM1500_1.getPositionInAbs() < DM1500_1.getMidPos()){
			DM1500_1.setPositionInAbs(DM1500_1.getMaxPos());
			while(conn.getMovingState());
		}else{
			DM1500_1.setPositionInAbs(DM1500_1.getMinPos());
			while(conn.getMovingState());
		}
	}

	// Moves servo 3 to the position of zero degree.
	DM1500_1.setPositionInDeg(0);
	while(conn.getMovingState());
	// Moves servo 3 to the position of maximum abs position.
	DM1500_1.setPositionInAbs(DM1500_1.getMaxPos());
	while(conn.getMovingState());
	// Moves servo 3 to the position of -45 degree.
	DM1500_1.setPositionInDeg(-45);
	while(conn.getMovingState());
	// Moves servo 3 to the position of PI/2 radiant.
	DM1500_1.setPositionInRad(-1.5);
	while(conn.getMovingState());
	// Moves servo 3 to the position of zero radiant.
	DM1500_1.setPositionInRad(0.0);
	while(conn.getMovingState());

	// Closes the connection.
	conn.closeConnection();
}

/** \brief Function for testing the fully assembled MEX robot manipulator.
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *
 *  This function may only be carried out on the MEX system under certain general conditions.
 *	Please check the settings in the Pololu Maestro Control Center first.
 *
 *  Settings for the servo motors in the Pololu Control Center:
 *  Serial Settings:
 *      Serial mode: USB Dual Port
 *  Channelsettings:
 *      NAME       | Min       | Max        | 8-bit neutral
 *      SERVO_01   | 512       | 2320       | 1420
 *      SERVO_02   | 512       | 2320       | 1420
 *      SERVO_03   | 512       | 2320       | 1420
 *      SERVO_04   | 480       | 1500       | 952
 * Status: set the servo to following positions(parking position)
 *      NAME       | Target
 *      SERVO_01   | 1420
 *      SERVO_02   | 2320
 *      SERVO_03   | 520
 *      SERVO_04   | 952
 *
 */
void testMEXMovementSetting2(){
    unsigned short speed = 10;
    unsigned short acceleration = 10;
    // Define the port name and the baud rate
   	#ifdef _WIN32
   		const char* portName = "COM5";  		// Windows
   	#else
   		const char* portName = "/dev/ttyACM2";  // Linux
   	#endif
    Pololu conn(portName, 9600);

    // Open connection to COM port.
    conn.openConnection();

    // Define the servos of the robot manipulator
    ServoMotor base(1, 5680, 3600, &conn);
    ServoMotor arm_1(2, 5680, 3600, &conn);
    ServoMotor arm_2(3, 5680, 3600, &conn);
    ServoMotor grip(4, 3808, 1888, &conn);

    base.showPololuValues();
    arm_1.showPololuValues();
    arm_2.showPololuValues();
    grip.showPololuValues();

    wait(5000);

    // Set speed and acceleration for all servos
    base.setSpeed(speed);
    base.setAccelaration(acceleration);
    arm_1.setSpeed(speed);
    arm_1.setAccelaration(acceleration);
    arm_2.setSpeed(speed);
    arm_2.setAccelaration(acceleration);
    grip.setSpeed(speed);
    grip.setAccelaration(acceleration);

    // Move into grabbing position
    base.setPositionInAbs(4080);
    while(conn.getMovingState());
    arm_1.setPositionInAbs(5488);
    while(conn.getMovingState());

    // Grab
    grip.setPositionInAbs(3320);
    while(conn.getMovingState());
    // Lift
    arm_1.setPositionInAbs(7840);

    // Move to new Location
    base.setPositionInAbs(8360);
    while(conn.getMovingState());

    // Drop
    arm_1.setPositionInAbs(5600);
    while(conn.getMovingState());
    grip.setPositionInAbs(3900);
    while(conn.getMovingState());
    arm_1.setPositionInAbs(7840);
    while(conn.getMovingState());

    // Go to Parking Position
    base.setPositionInAbs(5680);
    arm_1.setPositionInAbs(9280);
    arm_2.setPositionInAbs(2080);
    grip.setPositionInAbs(4000);

    // Close the serial Connection
    conn.closeConnection();
}

/** \brief Function for testing the fully assembled MEX robot manipulator.
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *
 *  This function may only be carried out on the MEX system under certain general conditions.
 *	Please check the settings in the Pololu Maestro Control Center first.
 *
 *  Settings for the servo motors in the Pololu Control Center:
 *  Serial Settings:
 *      Serial mode: USB Dual Port
 *  Channelsettings:
 *      NAME       | Min       | Max        | 8-bit neutral
 *      SERVO_01   | 480       | 2400       | 1420
 *      SERVO_02   | 480       | 2400       | 1500
 *      SERVO_03   | 480       | 2400       | 1380
 *      SERVO_04   | 480       | 1500       | 952
 * Status: set the servo to following positions(parking position)
 *      NAME       | Target
 *      SERVO_01   | 1420
 *      SERVO_02   | 2400
 *      SERVO_03   | 1380
 *      SERVO_04   | 952
 *
 */
void testMEXMovementSetting1(){
    unsigned short speed = 20;
    unsigned short acceleration = 10;
    // Define the port name and the baud rate
   	#ifdef _WIN32
   		const char* portName = "COM5";  		// Windows
   	#else
   		const char* portName = "/dev/ttyACM2";  // Linux
   	#endif
    Pololu conn(portName, 9600);

    // Open connection to COM port.
    conn.openConnection();

    // Define the servos of the robot manipulator
    ServoMotor base(1, 5680, 3600, &conn);
    ServoMotor arm_1(2, 6000, 3600, &conn);
    ServoMotor arm_2(3, 5520, 3600, &conn);
    ServoMotor grip(4, 3808, 1888, &conn);

    base.showPololuValues();
    arm_1.showPololuValues();
    arm_2.showPololuValues();
    grip.showPololuValues();

    wait(1000);

    // Set speed and acceleration for all servos
    base.setSpeed(speed);
    base.setAccelaration(acceleration);
    arm_1.setSpeed(speed);
    arm_1.setAccelaration(acceleration);
    arm_2.setSpeed(speed);
    arm_2.setAccelaration(acceleration);
    grip.setSpeed(speed);
    grip.setAccelaration(acceleration);

    // Move into starting position
    base.setPositionInAbs(base.getMidPos());
    arm_1.setPositionInAbs(arm_1.getMidPos());
    arm_2.setPositionInAbs(arm_2.getMidPos());
    grip.setPositionInAbs(grip.getMidPos());
    while(conn.getMovingState());

    // Move into grabbing position
    base.setPositionInDeg(-40);
    arm_1.setPositionInDeg(45);
    arm_2.setPositionInDeg(-90);
    while(conn.getMovingState());
    arm_1.setPositionInAbs(6880);
    while(conn.getMovingState());

    // Grab
    grip.setPositionInAbs(3080);
    while(conn.getMovingState());

    // Lift
    arm_1.setPositionInRad(M_PI/2);
    while(conn.getMovingState());

    // Move to new Location
    base.setPositionInDeg(70);
    while(conn.getMovingState());

    // Drop
    arm_1.setPositionInAbs(7050);
    while(conn.getMovingState());
    grip.setPositionInAbs(grip.getMaxPos());
    while(conn.getMovingState());

    // Lift arm
    arm_1.setPositionInRad(M_PI/2);
    while(conn.getMovingState());

    // Move into starting position
    base.setPositionInAbs(base.getMidPos());
    arm_1.setPositionInAbs(arm_1.getMidPos());
    arm_2.setPositionInAbs(arm_2.getMidPos());
    grip.setPositionInAbs(grip.getMidPos());
    while(conn.getMovingState());

    // Wave
    grip.setSpeed(100);
    grip.setAccelaration(100);
    arm_2.setPositionInDeg(90);
    while(conn.getMovingState());
    for (int i = 0; i < 8; i++){
    	if (grip.getPositionInAbs() > grip.getMidPos()){
    		grip.setPositionInAbs(grip.getMinPos());
    		while(conn.getMovingState());
    	}else{
    		grip.setPositionInAbs(grip.getMaxPos());
    		while(conn.getMovingState());
    	}
    }
    grip.setSpeed(speed);
    grip.setAccelaration(acceleration);

    // Go to Parking Position
    base.setPositionInAbs(base.getMidPos());
    arm_1.setPositionInAbs(arm_1.getMaxPos());
    arm_2.setPositionInAbs(arm_2.getMidPos());
    grip.setPositionInAbs(grip.getMidPos());

    // Close the serial Connection
    conn.closeConnection();
}

/** \brief Function for testing the fully assembled MEX robot manipulator.
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *
 *  This function may only be carried out on the MEX system under certain general conditions.
 *	Please check the settings in the Pololu Maestro Control Center first.
 *
 *  Settings for the servo motors in the Pololu Control Center:
 *  Serial Settings:
 *      Serial mode: USB Dual Port
 *  Channelsettings:
 *      NAME       | Min       | Max        | 8-bit neutral
 *      SERVO_01   | 660       | 2460       | 1560
 *      SERVO_02   | 580       | 2380       | 1480
 *      SERVO_03   | 640       | 2440       | 1540
 *      SERVO_04   | 1023      | 1897       | 1460
 * Status: set the servo to following positions(parking position)
 *      NAME       | Target
 *      SERVO_01   | 1560
 *      SERVO_02   | 1480
 *      SERVO_03   | 1540
 *      SERVO_04   | 1460
 *
 */
void testMEXMovementSetting3(){
    unsigned short speed = 30;
    unsigned short acceleration = 200;
    // Define the port name and the baud rate
   	#ifdef _WIN32
   		const char* portName = "COM5";  		// Windows
   	#else
   		const char* portName = "/dev/ttyACM2";  // Linux
   	#endif
    Pololu conn(portName, 9600);

    // Open connection to COM port.
    conn.openConnection();

    // Define the servos of the robot manipulator
    ServoMotor arm_0(1, 6240, 3600, &conn);
    ServoMotor arm_1(2, 5920, 3600, &conn);
    ServoMotor arm_2(3, 6160, 3600, &conn);
    ServoMotor arm_3(4, 5840, 1748, &conn);

    arm_0.showPololuValues();
    arm_1.showPololuValues();
    arm_2.showPololuValues();
    arm_3.showPololuValues();

    wait(1000);

    // Set speed and acceleration for all servos
    arm_0.setSpeed(speed);
    arm_0.setAccelaration(acceleration);
    arm_1.setSpeed(speed);
    arm_1.setAccelaration(acceleration);
    arm_2.setSpeed(speed);
    arm_2.setAccelaration(acceleration);
    arm_3.setSpeed(speed);
    arm_3.setAccelaration(acceleration);
    while(conn.getMovingState());

    // Move into starting position
    arm_0.setPositionInAbs(arm_0.getMidPos());
    arm_1.setPositionInAbs(arm_1.getMidPos());
    arm_2.setPositionInAbs(arm_2.getMidPos());
    arm_3.setPositionInAbs(arm_3.getMidPos());
    while(conn.getMovingState());

    wait(1000);

    arm_3.setPositionInAbs(7000);
    while(conn.getMovingState());
    // Point top
    arm_0.setPositionInAbs(6792);
    arm_1.setPositionInAbs(5800);
    arm_2.setPositionInAbs(9040);
    while(conn.getMovingState());

    arm_3.setPositionInAbs(7500);
    while(conn.getMovingState());

    // Point top left
    arm_0.setPositionInAbs(5704);
    arm_1.setPositionInAbs(4752);
    arm_2.setPositionInAbs(9452);
    while(conn.getMovingState());

    // Point bottom left
    arm_0.setPositionInAbs(4404);
    arm_1.setPositionInAbs(3860);
    arm_2.setPositionInAbs(9600);
    while(conn.getMovingState());

    // Point bottom right
    arm_0.setPositionInAbs(4544);
    arm_1.setPositionInAbs(3880);
    arm_2.setPositionInAbs(8856);
    while(conn.getMovingState());

    // Point top right
    arm_0.setPositionInAbs(5840);
    arm_1.setPositionInAbs(4800);
    arm_2.setPositionInAbs(8780);
    while(conn.getMovingState());

    // Point top
    arm_0.setPositionInAbs(6792);
    arm_1.setPositionInAbs(5800);
    arm_2.setPositionInAbs(9040);
    while(conn.getMovingState());

    arm_3.setPositionInAbs(7000);
    while(conn.getMovingState());

    // Move into starting position
    arm_0.setPositionInAbs(arm_0.getMidPos());
    arm_1.setPositionInAbs(arm_1.getMidPos());
    arm_2.setPositionInAbs(arm_2.getMidPos());
    arm_3.setPositionInAbs(arm_3.getMidPos());
    while(conn.getMovingState());


    // Close the serial Connection
    conn.closeConnection();
}
