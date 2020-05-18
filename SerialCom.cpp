#include "SerialCom.hpp"
#include <stdlib.h>
#include <iostream>

/** \brief Bei der Erzeugung eines Objektes vom Typ SerialCom werden dieser seriellen Schnittstelle ueber den Konstruktor der, dem Pololu vom System zugewiesenen, COM-Port und die Baudrate übergeben.
 *
 * \param portName_
 * \param baudRate_
 * \return Konstruktor hat keinen Rueckgabewert
 *
 */
SerialCom::SerialCom (const char* portName, unsigned int baudRate) {
    portName_ = portName;
    baudRate_ = baudRate;
}

/** \brief Funktion dient dem öffnen eines seriellen Ports mit den ueber den Konstruktor bereitgestellten Portnamen und der Baudrate.
 *         Die Funktion ist so aufgebaut, dass sie abhaengig vom Betriebssystem entweder ueber Windows bzw. ueber LINUX eine Verbindung aufbaut.
 *         LINUX ist noch nicht implementiert
 *
 * \param
 * \param
 * \return Liefert einen Wahrheitswert, ob der Verbindungsaufbau funtioniert hat.
 *
 */
bool SerialCom::openSerialCom () {
    #ifdef LINUX
        std::cout << "Es läuft Linux" << std::endl;
    #elif defined(WIN32) //Windows
        DCB commState;
        BOOL success;
        HANDLE port;

        port = CreateFile (portName_, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (port == INVALID_HANDLE_VALUE) {
            switch (GetLastError()) {
                case ERROR_ACCESS_DENIED:
                    fprintf(stderr, "Error: Access denied.  Try closing all other programs that are using the device.\n");
                    break;
                case ERROR_FILE_NOT_FOUND:
                    fprintf(stderr, "Error: Serial port not found.  "
                    "Make sure that \"%s\" is the right port name.  "
                    "Try closing all programs using the device and unplugging the "
                    "device, or try rebooting.\n", portName_);
                    break;
                default:
                    fprintf(stderr, "Error: Unable to open serial port.");
                    break;
            }
            return INVALID_HANDLE_VALUE;
        }

        success = GetCommState(port, &commState);
        if (!success)
        {
            fprintf(stderr, "Error: Unable to get comm state.");
            CloseHandle(port);
            return INVALID_HANDLE_VALUE;
        }
        commState.BaudRate = baudRate_;
        success = SetCommState(port, &commState);
        if (!success)
        {
            fprintf(stderr, "Error: Unable to set comm state.");
            CloseHandle(port);
            return INVALID_HANDLE_VALUE;
        }
        /** Flush out any bytes received from the device earlier. */
        success = FlushFileBuffers(port);
        if (!success)
        {
            fprintf(stderr, "Error: Unable to flush port buffers");
            CloseHandle(port);
            return INVALID_HANDLE_VALUE;
        }
        port_ = port;
        std::cout << "Port Opened" << std::endl;
        return true;
	#endif
}

bool SerialCom::closeSerialCom () {
    #ifdef LINUX

    #elif defined(WIN32)
        CloseHandle(port_);
        std::cout << "Port Closed" << std::endl;
    #endif // LINUX
}

bool SerialCom::writeSerialCom (unsigned char* command) {
    std::cout << portName_ << " : " << baudRate_ << std::endl;
};

bool SerialCom::readSerialCom (unsigned char* response) {
    std::cout << "Read" << std::endl;
};
