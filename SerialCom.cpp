#include "SerialCom.hpp"
#include <stdio.h>
#include <string>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#else

#endif

/** \brief SerialCom class constructor.
 *
 */
SerialCom::SerialCom(){

}

/** \brief SerialCom class constructor.: Initializes a serial connection.
 *
 * \param portName (COM port name assigned to the Pololu by the operating system)
 * \param baudRate (Baud rate with which the controller should communicate)
 *
 */
SerialCom::SerialCom(const char* portName, unsigned short baudRate){
    portName_ = portName;
    baudRate_ = baudRate;
}

/** \brief Initializes a serial connection. Puts the serial link in the same state as the constructor. This function can be used to change a serial connection or to change the baud rate.
 *
 * \param portName (COM port name assigned to the Pololu by the operating system)
 * \param baudRate (Baud rate with which the controller should communicate)
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

/** \brief Opens a serial connection to the COM port that was defined during initialization.
 *
 * \return Returns a Boolean value, whether the opening of the serial connection worked or not (1 = connection opened; 0 = connection failed).
 *
 */
bool SerialCom::openSerialCom(){
    #ifdef _WIN32
        bool success = FALSE;
        DCB state;

        CloseHandle(port_);
        port_ = CreateFileA(portName_, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (port_ == INVALID_HANDLE_VALUE){
            throw std::string("SerialCom::openSerialCom: Failed to open port.\n");
            return 0;
        }

        success = FlushFileBuffers(port_);
        if (!success)
        {
            throw std::string("SerialCom::openSerialCom: Failed to flush file buffer.\n");
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
            throw std::string("SerialCom::openSerialCom: Failed to set serial timeouts.\n");
            CloseHandle(port_);
            return 0;
        }

        state.DCBlength = sizeof(DCB);
        success = GetCommState(port_, &state);
        if (!success)
        {
            throw std::string("SerialCom::openSerialCom: Failed to get serial settings.\n");
            CloseHandle(port_);
            return 0;
        }
        state.BaudRate = baudRate_;
        success = SetCommState(port_, &state);
        if (!success)
        {
            throw std::string("SerialCom::openSerialCom: Failed to set serial settings.\n");
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
        if (CloseHandle(port_) == 0){
            throw std::string("SerialCom::closeSerialCom: Failed to close port.\n");
            return 0;
        }
        return 1;
    #else

    #endif
}

/** \brief Funktion sendet ein erstelltes Kommando an den Controller.
 *
 * \param command (Das Kommando, das durch die Pololu-Klasse erstellt wird.
 * \return Liefert bei einem Fehler -1, bei erfolgreichen Set-Befehlen eine 1, und im Falle
 * von Get-Befehlen wird entweder die Position oder der Bewegungsstatus als Rueckgabewert zurueckgegeben.
 */
bool SerialCom::writeSerialCom(unsigned char command[], unsigned short sizeCommand, unsigned char *response, unsigned short sizeResponse){
    #ifdef _WIN32
        DWORD bytesTrasfered;
        bool success = 0;

        //** Laenge des Kommandos ueberpruefen */
        if ((sizeCommand != 1) && (sizeCommand != 2) && (sizeCommand != 4)){
            throw std::string("SerialCom::writeSerialCom: wrong parameter sizeCommand, allowed parameter 1,2 or 4.");
        }

        //** Schreiben der Daten */
        success = WriteFile(port_, command, sizeCommand, &bytesTrasfered, NULL);
        if (!success){
            throw std::string("SerialCom::writeSerialCom: Failed to write to port.");
            return FALSE;
        }

        //** Ueberpruefen ob Daten gelesen werden muessen */
        if (sizeResponse > 0){
            success = ReadFile(port_, (void *)response, sizeResponse, &bytesTrasfered, NULL);
        }else{
            return TRUE; //Es sind keine Daten zu lesen. Rueckmeldung des erfolgreichen Schreiben der Daten.
        }
        if (!success){
            throw std::string("SerialCom::writeSerialCom: Failed to read from port.");
        }
        return TRUE; //Rueckmeldung das Daten geschrieben wurden und gelesene Daten im Array response gespeichert wurden.
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
