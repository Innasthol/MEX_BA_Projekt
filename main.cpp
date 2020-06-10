#include "Pololu.hpp"
#include <windows.h>
#include <string>
#include <iostream>

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
#define ARM_MINPOS   1984
#define ARM_MIDPOS   5792
#define ARM_MAXPOS   9216
#define GREIFER_MINPOS   1984
#define GREIFER_MIDPOS   3808
#define GREIFER_MAXPOS   5632



using namespace std;

/** \brief Function systematically tests the opening and closing of a serial connection
 *
 */
void testOpenClose (){
    //Define the port name and the baud rate
    const char* portName = "COM4";  // Windows, "\\\\.\\COM6" also works
    //const char* portName = "/dev/ttyACM0";  // Linux
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
    Pololu conn("COM5", 9600);
    cout << "Create Pololu-Object: Object created." << endl;

    /** Testing to open a serial port */
    cout << "Open a serial connection: ";
    if (conn.openConnection()){
        cout << "Connection opened." << endl;
    }

    /** Testing to set speed and acceleration to a servo */
    if (conn.setSpeed(servo, speed)){
        cout << "Set speed of Servo " << servo << " to " << speed << " (" << speed*0.25/10 << " us/ms): Successfully" << endl;
    }else{
        cout << "Set speed of Servo " << servo << " to " << speed << " (" << speed*0.25/10 << " us/ms): Failed" << endl;
    }
    if (conn.setAcceleration(servo, acceleration)){
        cout << "Set acceleration of Servo " << servo << " to " << acceleration << " (" << acceleration*0.25/10/80 << " us/(ms)^2): Successfully" << endl;
    }else{
        cout << "Set acceleration of Servo " << servo << " to " << acceleration << " (" << acceleration*0.25/10/80 << " us/(ms)^2): Failed" << endl;
    }

    /** Testing to read the current position of the servo */
    cout << "Current position of servo " << servo << " is " << conn.getPosition(servo) << endl;

    /** Test the setting of different positions for a servo and check whether a servo is still in motion */
    for (int i = 0; i < 5; i++){
        if (conn.getPosition(servo) < ARM_MIDPOS){
            cout << i << ". Servo " << servo+1 << " goes to position " << ARM_MAXPOS << endl;
            conn.setPosition(servo, ARM_MAXPOS);
            while(conn.getMovingState()){
                //cout << "waiting" << endl;
            }
        }else{
            cout << i << ". Servo " << servo+1 << " goes to position " << ARM_MINPOS << endl;
            conn.setPosition(servo, ARM_MINPOS);
            while(conn.getMovingState()){
                //cout << "waiting" << endl;
            }
        }
    }
}

void testMEXMovement(){
    unsigned short speed = 30;
    unsigned short acceleration = 10;
    Pololu conn("COM5", 9600);

    //** Open connection to COM port
    conn.openConnection();

    //** Set speed and acceleration for all servos
    for (unsigned short i = 0; i < 4; i++){
        conn.setSpeed(i, speed);
        conn.setAcceleration(i, acceleration);
    }

    //** leaving parking position
    conn.setPosition(SERVO_02, ARM_MIDPOS);
    while(conn.getMovingState()){}
    Sleep(1000);

    //** sequenz one
    conn.setPosition(SERVO_02, 8400);
    conn.setPosition(SERVO_01, 4160);
    while(conn.getMovingState()){}
    Sleep(1000);
    conn.setPosition(SERVO_03, 1984);
    conn.setPosition(SERVO_02, 7560);
    while(conn.getMovingState()){}
    conn.setPosition(SERVO_04, 2600);
    while(conn.getMovingState()){}
    Sleep(1000);
    conn.setPosition(SERVO_02, 9216);
    conn.setPosition(SERVO_01, 8200);
    while(conn.getMovingState()){}
    Sleep(1000);
    conn.setPosition(SERVO_02, 7560);
    while(conn.getMovingState()){}
    Sleep(1000);
    conn.setPosition(SERVO_04, 3808);
    while(conn.getMovingState()){}
    Sleep(1000);
    conn.setPosition(SERVO_02, ARM_MAXPOS);
    while(conn.getMovingState()){}
    conn.setPosition(SERVO_01, ARM_MIDPOS);
    conn.setPosition(SERVO_03, ARM_MIDPOS);
    while(conn.getMovingState()){}
    conn.setPosition(SERVO_02, ARM_MIDPOS);

    //** goodbey sequenz
    conn.setPosition(SERVO_03, 9216);
    while(conn.getMovingState()){}
    conn.setSpeed(SERVO_04, 200);
    conn.setAcceleration(SERVO_04, 100);
    Sleep(1000);
    // wave
    for (int i = 0; i < 5; i++){
        if (conn.getPosition(SERVO_04) <= GREIFER_MIDPOS){
            conn.setPosition(SERVO_04, GREIFER_MAXPOS);
            while(conn.getMovingState()){}
        }else{
            conn.setPosition(SERVO_04, GREIFER_MINPOS);
            while(conn.getMovingState()){}
        }
    }
    conn.setSpeed(SERVO_04, speed);
    conn.setAcceleration(SERVO_04, acceleration);
    conn.setPosition(SERVO_04, GREIFER_MIDPOS);
    conn.setPosition(SERVO_03, ARM_MIDPOS);
    while(conn.getMovingState()){}

    // go into parking position
    conn.setPosition(SERVO_02, ARM_MAXPOS);
}


int main()
{
    //testOpenClose();
    //testSetGetMethoden();
    testMEXMovement();
}
