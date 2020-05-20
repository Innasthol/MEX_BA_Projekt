#include "Pololu.hpp"
#include <iostream>
#include "windows.h"

using namespace std;

void testOpenAndCloseSerialCom (){

}

int main()
{
    Pololu controller("COM4", 9600);

    controller.openConnection();

    controller.setPosition(0, 8000);
    controller.setPosition(1, 8000);
    Sleep(2000);
    controller.setPosition(0, 4000);
    controller.setPosition(1, 4000);
    Sleep(2000);
    controller.setSpeed(0, 40);
    Sleep(2000);
    controller.setPosition(0, 8000);
    controller.setPosition(1, 8000);
    Sleep(10000);
    controller.setSpeed(0, 0);
    Sleep(2000);
    controller.setPosition(0, 4000);
    controller.setPosition(1, 4000);

    controller.closeConnection();
}

