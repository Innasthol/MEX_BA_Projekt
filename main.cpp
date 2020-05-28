#include "Pololu.hpp"
#include <iostream>
#include <windows.h>

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
#define MINPOS   2200
#define GOHOME   5000
#define MAXPOS   7800

using namespace std;

/** \brief Funktion testet systematisch das Oeffnen und Schliessen einer seriellen Verbindung
 *
 */
void testOpenClose (){
    //Festlegen des Portnamen und der Baudrate(noch nicht in Benutzung)
    const char* portName = "COM4";  // Windows, "\\\\.\\COM6" also works
    //const char* portName = "/dev/ttyACM0";  // Linux
    unsigned short baudRate = 9600;
    //Definieren eines Pololu-Objektes
    Pololu conn(portName, baudRate);

    /**< Test 1 */
    //Oeffnen des seriellen Ports mit den Initialisierten Portnamen und Baudrate;
    if (conn.openConnection()){
        std::cout << "TEST_01 :: " << portName << ": Connection opened successfully" << std::endl;
    }
    //Oeffnen des selben Ports nochmal
    if (conn.openConnection()){
        std::cout << "TEST_01 :: " << portName << ": Connection opened successfully" << std::endl;
    }

    /**< Test 2 */
    //Schliessen des Ports
    if (conn.closeConnection()){
        std::cout << "TEST_02 :: " << portName << ": Connection closed successfully" << std::endl;
    }else{
        std::cout << "TEST_02 :: " << portName << ": No connection to close" << std::endl;
    }
    //Erneutes Schliessen eines bereits geschlossenen Ports
    if (conn.closeConnection()){
        std::cout << "TEST_02 :: " << portName << ": Connection closed successfully" << std::endl;
    }else{
        std::cout << "TEST_02 :: " << portName << ": No connection to close" << std::endl;
    }

    /**< Test 3 */
    //Neu initialisieren des seriellen Ports mit unbekanntem Namen
    conn.initConnection("COM7", baudRate);
    //Oeffnen des neu initialisierten Ports
    if (conn.openConnection()){
        std::cout << "TEST_03 :: " << "COM7" << ": Connection opened successfully" << std::endl;
    }else{
        std::cout << "TEST_03 :: " << "COM7" << ": Unknown portname" << std::endl;
    }

    /**< Test 4 */
    //Neu initialisieren des seriellen Ports mit einem bekannten Port, jedoch nicht Pololu
    conn.initConnection("COM1", baudRate);
    //Oeffnen des Ports
    if (conn.openConnection()){
        std::cout << "TEST_04 :: " << "COM1" << ": Connection opened successfully" << std::endl;
    }
    //Schliessen des Ports
    if (conn.closeConnection()){
        std::cout << "TEST_04 :: "<< portName << ": Connection closed successfully" << std::endl;
    }else{
        std::cout << "TEST_04 :: "<< portName << ": No connection to close" << std::endl;
    }

    /**< Test 5 */
    //Neu initialisieren des seriellen Ports mit Pololu-Port
    conn.initConnection(portName, baudRate);
    for (int i = 0; i < 10; i++){
        if (conn.openConnection()){
            std::cout << "TEST_05 :: " << portName << ": Connection opened successfully" << std::endl;
        }
    }
    conn.closeConnection();
};

/** \brief Funktion testet systematisch das Schreiben und Lesen einer seriellen Verbindung
 *
 */
void testSetGetMethoden () {
    Pololu conn("COM4", 9600);
    conn.openConnection();

    //cout << "Current Position: " << conn.getPosition(SERVO_01) << endl;

    conn.setSpeed(SERVO_01, 100);
    conn.setAcceleration(SERVO_01, 0);

    for (int i=0; i<10; i++){
        if (conn.getPosition(SERVO_01) < 5000){
            conn.setPosition(SERVO_01, MAXPOS);
            while(conn.getMovingState()){
                //cout << "waiting" << endl;
            }
        }else{
            conn.setPosition(SERVO_01, MINPOS);
            while(conn.getMovingState()){
                //cout << "waiting" << endl;
            }
        }
        cout << "Position: " << conn.getPosition(SERVO_01) << endl;
    }
}

int main()
{
    //testOpenClose();
    testSetGetMethoden();
}
