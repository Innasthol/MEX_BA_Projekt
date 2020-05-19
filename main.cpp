#include "SerialCom.hpp"
#include "Pololu.hpp"
#include <iostream>
#include "windows.h"

using namespace std;




int main()
{
    Pololu controller("COM4", 9600);

    controller.setPosition(0, 5000);
    Sleep(2000);
    controller.setPosition(1, 5000);
    Sleep(2000);
    controller.setPosition(0, 7000);
    Sleep(2000);
    controller.setPosition(1, 7000);
    Sleep(2000);
}

