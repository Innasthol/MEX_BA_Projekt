// ACHTUNG: Der Maestros serieller Modus muss auf "USB Dual Port" gesetzt sein.
// ACHTUNG: Zeile "const char* portName = "COM4";" muss angepasst werden.
#include "Pololu.hpp"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

//Notwendig fuer Windows, um Textausgaben zu taetigen und Funktionen zu testen.
#include <iostream>
#include <windows.h>

int main()
{
  const char* portName = "COM4";  // Windows, "\\\\.\\COM6" also works
  //const char* portName = "/dev/ttyACM0";  // Linux
  unsigned short baudRate = 9600;
  const unsigned int servo_1 = 0;
  const unsigned int servo_2 = 1;
  const unsigned short minPos = 2000;
  const unsigned short maxPos = 8000;

  Pololu conn(portName, baudRate);
  conn.openConnection();
  std::cout << "Aktuelle Position:" << conn.getPosition(servo_1) << std::endl;

  conn.setSpeed(servo_1, 0);
  conn.setAcceleration(servo_1, 0);
  if (conn.getPosition(servo_1) < 5000){
    conn.setPosition(servo_1, maxPos);
    std::cout << "Angefarene Position: 8000" << std::endl;
  }else{
    conn.setPosition(servo_1, minPos);
    std::cout << "Angefarene Position: 2000" << std::endl;
  }
  Sleep(2000);

  conn.setSpeed(servo_1, 50);
  conn.setAcceleration(servo_1, 50);
  Sleep(100);

  if (conn.getPosition(servo_1) < 5000){
    conn.setPosition(servo_1, maxPos);
    std::cout << "Angefarene Position: 8000" << std::endl;
  }else{
    conn.setPosition(servo_1, minPos);
    std::cout << "Angefarene Position: 2000" << std::endl;
  }
  Sleep(2000);
  return 0;
}
