//============================================================================
// Name        : SerialCom.cpp
// Author      : Willi Penner
//
// Description : SerialCom source file. It contains the definition of the
//               functions of the SerialCom class.
//============================================================================
#include "SerialCom.hpp"
#include <stdio.h>
#include <string>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#else
	#include <fcntl.h>
	#include <unistd.h>
	#include <stdint.h>
	#include <termios.h>
	#include <stdbool.h>
#endif

/** \brief Constructors: A constructor without transfer parameters, so that when used in the Pololu class
 *  or another class, an object of type SerialCom can be created without having to pass values to it.
 *  And a constructor for creating an object with start parameters for the port name and the baud rate.
 *
 *  \param portName : The port name is used to open a serial connection via the port name for the controller specified by the operating system.
 *  \param baudRate : The baud rate determines the transmission speed at which communication between the PC and controller takes place.
 */
SerialCom::SerialCom(){
	#ifdef _WIN32
		port_ = NULL;
	#else
		port_ = 0;
	#endif
}
SerialCom::SerialCom(const char* portName, unsigned short baudRate){
	portName_ = portName;
	baudRate_ = baudRate;
	#ifdef _WIN32
		port_ = NULL;
	#else
		port_ = 0;
	#endif
}

/** \brief "initSerialCom" is used to initiate the SerialCom object with port name and baud rate.
 *  The function puts the object in the same state as the constructor with transfer parameters.
 *  It can be used to change the port name and the baud rate of a SerialCom object.
 *
 *  \param portName : The port name is used to open a serial connection via the port name for the controller specified by the operating system.
 *  \param baudRate : The baud rate determines the transmission speed at which communication between the PC and controller takes place.
 */
void SerialCom::initSerialCom(const char* portName, unsigned short baudRate){
    /**< Before a serial connection is reinitialized, a possible open connection is closed. */
    #ifdef _WIN32
        CloseHandle(port_);
    #else

    #endif
    portName_ = portName;
    baudRate_ = baudRate;
}

/** \brief Uses the set port name and the baud rate of the class to open a serial connection.
 *  If the opening is successful, the value in "port_" stands for the open connection and
 *  can be used for communication.
 *
 *  \return Returns TRUE on successful opening of a serial connection, otherwise it returns FALSE.
 */
bool SerialCom::openSerialCom(){
    #ifdef _WIN32
        /**< Opening a serial connection in windows. */
        bool success = FALSE;
        DCB state;

        /**< If there is still an open connection, it will be closed before opening it again. */
        CloseHandle(port_);
        /**< Opens a serial connection using the CreateFileA function from <windows.h>. Port_ is
        opened with read and write access. */
        port_ = CreateFileA(portName_, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (port_ == INVALID_HANDLE_VALUE){
            throw std::string("SerialCom::openSerialCom: Failed to open port.\n");
            return 0;
        }
        /**< Flushes the file buffer of the opened connection. */
        success = FlushFileBuffers(port_);
        if (!success)
        {
            throw std::string("SerialCom::openSerialCom: Failed to flush file buffer.\n");
            CloseHandle(port_);
            return 0;
        }
        /**< Configure read and write operations to time out after 100 ms. */
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
        /**< Reads the connection status of the serial connection. If the reading of
        the connection status was successful, the baud rate for the communication is set. */
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
        bool success = false;

        /**< If there is still an open connection, it will be closed before opening it again. */
        close(port_);
        /**< Opens a serial connection using the CreateFileA function from <windows.h>. Port_ is
             opened with read and write access. */
        port_ = open(portName_, O_RDWR | O_NOCTTY);
        if (port_ == -1){
        	throw std::string("SerialCom::openSerialCom: Failed to open port.\n");
            return 0;
        }
        /**< Flushes the file buffer of the opened connection. */
        success = tcflush(port_, TCIOFLUSH);
        if (success){
        	throw std::string("SerialCom::openSerialCom: Failed to flush file buffer.\n");
        	close(port_);
        	return 0;
        }
        // Get the current configuration of the serial port.
        struct termios options;
        success = tcgetattr(port_, &options);
        if (success){
        	throw std::string("SerialCom::openSerialCom: Failed to get serial settings.\n");
            close(port_);
            return 0;
        }
        // Turn off any options that might interfere with our ability to send and
        // receive raw binary bytes.
        options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
        options.c_oflag &= ~(ONLCR | OCRNL);
        options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

        // Set up timeouts: Calls to read() will return as soon as there is
        // at least one byte available or when 100 ms has passed.
        options.c_cc[VTIME] = 1;
        options.c_cc[VMIN] = 0;

        // This code only supports certain standard baud rates. Supporting
        // non-standard baud rates should be possible but takes more work.
        cfsetospeed(&options, B9600);
        cfsetispeed(&options, cfgetospeed(&options));
        success = tcsetattr(port_, TCSANOW, &options);
        if (success){
        	throw std::string("SerialCom::openSerialCom: Failed to set serial settings.\n");
            close(port_);
            return 0;
        }
        return 1;
    #endif
}

/** \brief Closes a serial connection.
 *
 *  \return Returns TRUE on successful closing of a serial connection, otherwise it returns FALSE.
 */
bool SerialCom::closeSerialCom(){
    #ifdef _WIN32
        if (CloseHandle(port_) == 0){
        	throw std::string("SerialCom::closeSerialCom: Failed to close port.\n");
        	return 0;
        }
        return 1;
    #else
        close(port_);
        return 1;
    #endif
}

/** \brief "writeSerialCom" is used to write commands to the controller via the open serial connection.
 *
 * \param command[] : Contains the command to be sent (size of 1, 2 or 4 bytes, depending on the command).
 * \param sizeCommand : Contains the size of the command (1, 2 or 4).
 * \param response : If the command to be sent expects a return value from the controller, the writeSerialCom is given a pointer to a response array. This can be 1 or 2 bytes in size. If no return value is expected, the pointer is NULL.
 * \param sizeResponse : Contains the size of the command (1 or 2, in case of no expected return value it is 0).
 *
 * \return Returns TRUE on successful writing to a serial connection, otherwise it returns FALSE.
 */
bool SerialCom::writeSerialCom(unsigned char command[], unsigned short sizeCommand, unsigned char *response, unsigned short sizeResponse){
    #ifdef _WIN32
        DWORD bytesTrasfered; //Is given to the write or read command as a pointer. After executing the WriteFile or ReadFile, bytesTranfered contains the number of bytes transmitted or received.
        bool success = 0;

        //** Check the length of the command */
        if ((sizeCommand != 1) && (sizeCommand != 2) && (sizeCommand != 4)){
            throw std::string("SerialCom::writeSerialCom: wrong parameter sizeCommand, allowed parameter 1,2 or 4.");
        }

        //** Sending the command to the controller via port_. */
        success = WriteFile(port_, command, sizeCommand, &bytesTrasfered, NULL);
        if (!success){
            throw std::string("SerialCom::writeSerialCom: Failed to write to port.");
            return FALSE;
        }

        //** Check whether data needs to be read. */
        if (sizeResponse > 0){
            success = ReadFile(port_, (void *)response, sizeResponse, &bytesTrasfered, NULL);
        }else{
            return TRUE; //No need to read data. Confirmation of the successful writing of the data.
        }
        if (!success){
            throw std::string("SerialCom::writeSerialCom: Failed to read from port.");
        }
        return TRUE; //Confirmation that data was written and read data were saved in the response array.
    #else
        return 1;
    #endif
}

/** \brief Returns the port HANDLE under Windows or the integer value for the connection under Linux.
 *
 * \return port_
 */
#ifdef _WIN32
    HANDLE SerialCom::getPort(){
#else
    int SerialCom::getPort(){
#endif
    return port_;
}
