#include "SerialCom.hpp"
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#else

#endif

/** \brief Kontruktor der Klasse SerialCom
 *
 */
SerialCom::SerialCom(){

}

/** \brief Kontruktor der Klasse SerialCom: Initialisiert eine serielle Verbindung
 *
 * \param portName (COM-Port-Name der vom Betriebssystem dem Pololu zugewiesen wird)
 * \param baudRate (Baudrate mit der die Communikation stattfinden soll)
 *
 */
SerialCom::SerialCom(const char* portName, unsigned short baudRate){
    portName_ = portName;
    baudRate_ = baudRate;
}

/** \brief Initialisiert eine serielle Verbindung. Versetzt die serielle Verbindung in den gleichen Zustand wie der Konstruktor.
 * Diese Funktion kann genutzt werden, um eine serielle Verbindung zu aendern bzw. auch die Baudrate zu veraendern.
 *
 * \param portName (COM-Port-Name der vom Betriebssystem dem Pololu zugewiesen wird)
 * \param baudRate (Baudrate mit der die Communikation stattfinden soll)
 *
 */
void SerialCom::initSerialCom(const char* portName, unsigned short baudRate){
    #ifdef _WIN32
        CloseHandle(port_);
    #else

    #endif
    portName_ = portName;
    baudRate_ = baudRate;
}

/** \brief Oeffnet eine serielle Verbindung zum COM-Port der bei der Initialisierung festgelegt wurde.
 *
 * \return Liefert einen boolschen Wert, ob das Oeffnen der seriellen Verbindung funktioniert hat oder nicht (1 = Verbindung geoeffnet; 0 = Verbindung fehlgeschlagen).
 *
 */
bool SerialCom::openSerialCom(){
    #ifdef _WIN32
        bool success = FALSE;
        DCB state;


        CloseHandle(port_);
        port_ = CreateFileA(portName_, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (port_ == INVALID_HANDLE_VALUE){
            perror("Failed to open Port\n");
            return 0;
        }

        success = FlushFileBuffers(port_);
        if (!success)
        {
            perror("Failed to flush serial port\n");
            CloseHandle(port_);
            return 0;
        }

        // Configure read and write operations to time out after 100 ms.
        COMMTIMEOUTS timeouts = { 0 };
        timeouts.ReadIntervalTimeout = 0;
        timeouts.ReadTotalTimeoutConstant = 100;
        timeouts.ReadTotalTimeoutMultiplier = 0;
        timeouts.WriteTotalTimeoutConstant = 100;
        timeouts.WriteTotalTimeoutMultiplier = 0;

        success = SetCommTimeouts(port_, &timeouts);
        if (!success)
        {
            perror("Failed to set serial timeouts\n");
            CloseHandle(port_);
            return 0;
        }

        state.DCBlength = sizeof(DCB);
        success = GetCommState(port_, &state);
        if (!success)
        {
            perror("Failed to get serial settings\n");
            CloseHandle(port_);
            return 0;
        }
        state.BaudRate = baudRate_;
        success = SetCommState(port_, &state);
        if (!success)
        {
            perror("Failed to set serial settings\n");
            CloseHandle(port_);
            return 0;
        }

        return 1;
    #else

    #endif
}

/** \brief Schliesst die gewuenschte serielle Verbindung.
 *
 */
bool SerialCom::closeSerialCom(){
    #ifdef _WIN32
        return CloseHandle(port_);
    #else

    #endif
}

/** \brief Funktion sendet ein erstelltes Kommando an den Controller.
 *
 * \param command (Das Kommando, das durch die Pololu-Klasse erstellt wird.
 * \return Liefert bei einem Fehler -1, bei erfolgreichen Set-Befehlen eine 1, und im Falle
 * von Get-Befehlen wird entweder die Position oder der Bewegungsstatus als Rueckgabewert zurueckgegeben.
 */
short SerialCom::writeSerialCom(unsigned char command[]){
    #ifdef _WIN32
        DWORD written;
        bool success;
        unsigned char newSetCommand[4];
        unsigned char newGetPositionCommand[2];
        unsigned char newGetMovingCommand[1];

        if ((command[0] == 0x84) || (command[0] == 0x87) || (command[0] == 0x89)){
            newSetCommand[0] = command[0];
            newSetCommand[1] = command[1];
            newSetCommand[2] = command[2];
            newSetCommand[3] = command[3];
            success = WriteFile(port_, newSetCommand, sizeof(newSetCommand), &written, NULL);
        }else if (command[0] == 0x90){
            newGetPositionCommand[0] = command[0];
            newGetPositionCommand[1] = command[1];
            success = WriteFile(port_, newGetPositionCommand, sizeof(newGetPositionCommand), &written, NULL);
        }else if (command[0] == 0x93){
            newGetMovingCommand[0] = command[0];
            success = WriteFile(port_, newGetMovingCommand, sizeof(newGetMovingCommand), &written, NULL);
        }
        if (!success){
            perror("Failed to write to port");
            return -1;
        }
        if (command[0] == 0x90){
            unsigned char newRead[2];
            success = ReadFile(port_, newRead, sizeof(newRead), &written, NULL);
            if (!success){
                perror("Failed to read from port\n");
                return -1;
            }
            return newRead[0] + 256 * newRead[1];
        }else if (command[0] == 0x93){
            unsigned char newRead[1];
            success = ReadFile(port_, newRead, sizeof(newRead), &written, NULL);
            if (!success){
                perror("Failed to read from port\n");
                return -1;
            }
            return newRead[0];
        }
        return 1;
    #else

    #endif
}

/** \brief Funktion liefert den initierten Port.
 *
 *  \return Rückgabe wert ist unter Windows der Port-HANDLE und unter LINUX der Port als Integer.
 */
#ifdef _WIN32
    HANDLE SerialCom::getPort(){
#else
    int SerialCom::getPort(){
#endif
    return port_;
}
