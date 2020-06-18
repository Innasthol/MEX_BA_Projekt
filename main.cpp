//============================================================================
// Name        : main.cpp
// Author      : Willi Penner
//
// Description : Contains the main function. It also contains test functions
//               to test the connection to the controller and the movement of
//       		 the servos.
//============================================================================
#include "Pololu.hpp"
#include <string>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#else
	#include <unistd.h>
#endif

// Define 12 possible servos for a Pololu Maestro 12.
#define SERVO_01    0
#define SERVO_02    1
#define SERVO_03    2
#define SERVO_04    3
#define SERVO_05    4
#define SERVO_06    5
#define SERVO_07    6
#define SERVO_08    7
#define SERVO_09    8
#define SERVO_10    9
#define SERVO_11   10
#define SERVO_12   11
// Define constants for the minimum, maximum and middle position of a robot arm.
#define ARM_MINPOS   1984
#define ARM_MIDPOS   5792
#define ARM_MAXPOS   9216
// Define constants for the minimum, maximum and middle position of the robot gripper.
#define GRIB_MINPOS   1984
#define GRIB_MIDPOS   3808
#define GRIB_MAXPOS   5632

using namespace std;

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
    //const char* portName = "COM4";  // Windows
    const char* portName = "/dev/ttyACM0";  // Linux
    unsigned short baudRate = 9600;
    //Define a Pololu object
    Pololu conn(portName, baudRate);

    /**< Test 1 */
    //Open the serial port with the initialized port name and baud rate
    if (conn.openConnection()){
        std::cout << "TEST_01 :: " << portName << ": Connection opened successfully" << std::endl;
    }
    //Open the same port again
    if (conn.openConnection()){
        std::cout << "TEST_01 :: " << portName << ": Connection opened successfully" << std::endl;
    }

    /**< Test 2 */
    //Close the port
    if (conn.closeConnection()){
        std::cout << "TEST_02 :: " << portName << ": Connection closed successfully" << std::endl;
    }else{
        std::cout << "TEST_02 :: " << portName << ": No connection to close" << std::endl;
    }
    //Close the same port again
    if (conn.closeConnection()){
        std::cout << "TEST_02 :: " << portName << ": Connection closed successfully" << std::endl;
    }else{
        std::cout << "TEST_02 :: " << portName << ": No connection to close" << std::endl;
    }

    /**< Test 3 */
    //Reinitialize the serial port with an unknown name
    conn.initConnection("COM7", baudRate);
    //Open the newly initialized port
    if (conn.openConnection()){
        std::cout << "TEST_03 :: " << "COM7" << ": Connection opened successfully" << std::endl;
    }else{
        std::cout << "TEST_03 :: " << "COM7" << ": Unknown portname" << std::endl;
    }

    /**< Test 4 */
    //Reinitialize the serial port with a known port, but not from the Pololu
    conn.initConnection("COM1", baudRate);
    //Open the port
    if (conn.openConnection()){
        std::cout << "TEST_04 :: " << "COM1" << ": Connection opened successfully" << std::endl;
    }
    //Close the port
    if (conn.closeConnection()){
        std::cout << "TEST_04 :: "<< portName << ": Connection closed successfully" << std::endl;
    }else{
        std::cout << "TEST_04 :: "<< portName << ": No connection to close" << std::endl;
    }

    /**< Test 5 */
    //Reinitialize the serial port with a Pololu known port
    conn.initConnection(portName, baudRate);
    for (int i = 0; i < 10; i++){
        if (conn.openConnection()){
            std::cout << "TEST_05 :: " << portName << ": Connection opened successfully" << std::endl;
        }
    }
    conn.closeConnection();
};

/** \brief Function systematically tests the writing and reading of a serial connection
 *
 */
void testSetGetMethoden () {
    unsigned short speed = 100;
    unsigned short acceleration = 10;
    unsigned short servo = SERVO_01;
    Pololu conn("COM4", 9600);
    cout << "Create Pololu-Object: Object created." << endl;

    /** Testing to open a serial port */
    cout << "Open a serial connection: ";
    if (conn.openConnection()){
        cout << "Connection opened." << endl;
    }

    /** Testing to set speed and acceleration to a servo */
    if (conn.setSpeed(servo, speed)){
        cout << "Set speed of Servo " << servo+1 << " to " << speed << " (" << speed*0.25/10 << " us/ms): Successfully" << endl;
    }else{
        cout << "Set speed of Servo " << servo+1 << " to " << speed << " (" << speed*0.25/10 << " us/ms): Failed" << endl;
    }
    if (conn.setAcceleration(servo, acceleration)){
        cout << "Set acceleration of Servo " << servo+1 << " to " << acceleration << " (" << acceleration*0.25/10/80 << " us/(ms)^2): Successfully" << endl;
    }else{
        cout << "Set acceleration of Servo " << servo+1 << " to " << acceleration << " (" << acceleration*0.25/10/80 << " us/(ms)^2): Failed" << endl;
    }

    /** Testing to read the current position of the servo */
    cout << "Current position of servo " << servo+1 << " is " << conn.getPosition(servo) << endl;

    /** Test the setting of different positions for a servo and check whether a servo is still in motion */
    for (int i = 0; i < 5; i++){
        if (conn.getPosition(servo) < ARM_MIDPOS){
            cout << i << ". Servo " << servo+1 << " goes to position " << ARM_MAXPOS << endl;
            conn.setPosition(servo, ARM_MAXPOS);
            while(conn.getMovingState()){}
        }else{
            cout << i << ". Servo " << servo+1 << " goes to position " << ARM_MINPOS << endl;
            conn.setPosition(servo, ARM_MINPOS);
            while(conn.getMovingState()){}
        }
    }

    /** Testing to change the speed while servo is moving */
    conn.setPosition(servo, ARM_MIDPOS);
    while(conn.getMovingState()){}
    conn.setSpeed(servo, 5);
    conn.setPosition(servo, ARM_MAXPOS);
    wait(1000);
    conn.setSpeed(servo, 200);
}

/** \brief Function for testing the fully assembled MEX system.
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *  WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
 *
 *  This function may only be carried out on the MEX system under certain general conditions.
 *
 *  Settings for the servo motors in the Pololu Control Center:
 *  Serial Settings:
 *      Serial mode: USB Dual Port
 *
 *  Channelsettings:
 *      NAME       | Min       | Max        | 8-bit neutral
 *      SERVO_01   | 496       | 2304       | 1448
 *      SERVO_02   | 496       | 2304       | 1448
 *      SERVO_03   | 496       | 2304       | 1448
 *      SERVO_04   | 496       | 1408       | 952
 *
 * Status: set the servo to following positions(parking position)
 *      NAME       | Target
 *      SERVO_01   | 1448
 *      SERVO_02   | 2304
 *      SERVO_03   | 1448
 *      SERVO_04   | 952
 *
 */
void testMEXMovement(){
    unsigned short speed = 30;
    unsigned short acceleration = 10;
    Pololu conn("COM5", 9600);

    /**< Open connection to COM port. */
    conn.openConnection();

    /**< Set speed and acceleration for all servos */
    for (unsigned short i = 0; i < 4; i++){
        conn.setSpeed(i, speed);
        conn.setAcceleration(i, acceleration);
    }

    /**< leaving the parking position */
    conn.setPosition(SERVO_02, ARM_MIDPOS);
    while(conn.getMovingState()){}
    wait(1000);

    /**< sequenz one */
    // go in position for grabbing
    conn.setPosition(SERVO_02, 8400);
    conn.setPosition(SERVO_01, 4160);
    while(conn.getMovingState()){}
    wait(1000);
    conn.setPosition(SERVO_03, 1984);
    conn.setPosition(SERVO_02, 7560);
    while(conn.getMovingState()){}
    // grabbing the box
    conn.setPosition(SERVO_04, 2600);
    while(conn.getMovingState()){}
    wait(1000);
    // liftig the box and moving to the right side
    conn.setPosition(SERVO_02, 9216);
    conn.setPosition(SERVO_01, 8200);
    while(conn.getMovingState()){}
    wait(1000);
    // putting the box down
    conn.setPosition(SERVO_02, 7560);
    while(conn.getMovingState()){}
    wait(1000);
    // release the box
    conn.setPosition(SERVO_04, 3808);
    while(conn.getMovingState()){}
    wait(1000);
    // raise the arm and move it to the starting position
    conn.setPosition(SERVO_02, ARM_MAXPOS);
    while(conn.getMovingState()){}
    conn.setPosition(SERVO_01, ARM_MIDPOS);
    conn.setPosition(SERVO_03, ARM_MIDPOS);
    while(conn.getMovingState()){}
    conn.setPosition(SERVO_02, ARM_MIDPOS);

    /**< goodbey sequenz */
    // lifts the arm
    conn.setPosition(SERVO_03, 9216);
    while(conn.getMovingState()){}
    // sets the speed of the grabber to a higher level
    conn.setSpeed(SERVO_04, 200);
    // sets the acceleration of the grabber to a higher level
    conn.setAcceleration(SERVO_04, 100);
    wait(1000);
    // waving with the grabber
    for (int i = 0; i < 5; i++){
        if (conn.getPosition(SERVO_04) <= GRIB_MIDPOS){
            conn.setPosition(SERVO_04, GRIB_MAXPOS);
            while(conn.getMovingState()){}
        }else{
            conn.setPosition(SERVO_04, GRIB_MINPOS);
            while(conn.getMovingState()){}
        }
    }
    // sets the speed and acceleration to the level at the beginning
    conn.setSpeed(SERVO_04, speed);
    conn.setAcceleration(SERVO_04, acceleration);
    // moving to the starting position
    conn.setPosition(SERVO_04, GRIB_MIDPOS);
    conn.setPosition(SERVO_03, ARM_MIDPOS);
    while(conn.getMovingState()){}
    // moving into parking position
    // necessary, because the robot arm falls down due to its own weight when the power is switched off
    conn.setPosition(SERVO_02, ARM_MAXPOS);
}


int main()
{
    testOpenClose();

    //testSetGetMethoden();

    //testMEXMovement();
}
