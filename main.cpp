#include "SerialCom.hpp"
#include "Pololu.hpp"
#include <iostream>

using namespace std;

int main()
{
    SerialCom test("COM5", 9600);
    unsigned char anweisung[4];
    bool erfolg;

    erfolg = test.openSerialCom();
    cout << erfolg << endl;
    test.writeSerialCom(anweisung);
    test.closeSerialCom();

    cout << "Hello world!" << endl;
}
