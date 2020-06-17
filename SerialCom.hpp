//============================================================================
// Name        : SerialCom.hpp
// Author      : Willi Penner
//
// Description : SerialCom header file. It contains the declaration of the
//               ISerialCom interface and the SerialCom class
//============================================================================
#ifndef SERIALCOM_HPP_INCLUDED
#define SERIALCOM_HPP_INCLUDED

#ifdef _WIN32
    #include <windows.h>
#else

#endif

/** \brief Interface for a serial connection via a COM port
 *  Prescribes at least the included pure virtual functions for initiating,
 *  opening, closing and writing a serial connection.
 */
class ISerialCom {
public:
	virtual ~ISerialCom(){};
    virtual void initSerialCom(const char* portName, unsigned short baudRate) = 0;
    virtual bool openSerialCom() = 0;
    virtual bool closeSerialCom() = 0;
    virtual bool writeSerialCom(unsigned char command[], unsigned short sizeCommand, unsigned char *response, unsigned short sizeResponse) = 0;
};

/** \brief SerialCom is a class that inherits the functions from the ISerialCom interface.
 *  The functionality of the interface is expanded by the function getPort ().
 *
 *  \param portName_ : The port name is used to open a serial connection via the port name for the controller specified by the operating system.
 *  \param baudRate_ : The baud rate determines the transmission speed at which communication between the PC and controller takes place.
 *  \param port_ : If a serial connection is opened successfully, depending on the operating system, a HANDLE (under Windows) or an integer value (under Linux) is defined, via which the communication takes place.
 */
class SerialCom : public ISerialCom {
private:
    const char* portName_ = NULL;
    unsigned short baudRate_ = 0;
    /**< depending on the operating system, a HANDLE (under Windows) or an integer value (under Linux) for an open connection */
    #ifdef _WIN32
        HANDLE port_;
    #else
        int port_;
    #endif
public:
    /** \brief Constructors: A constructor without transfer parameters, so that when used in the Pololu class
     *  or another class, an object of type SerialCom can be created without having to pass values to it.
     *  And a constructor for creating an object with start parameters for the port name and the baud rate.
     *
     *  \param portName : The port name is used to open a serial connection via the port name for the controller specified by the operating system.
     *  \param baudRate : The baud rate determines the transmission speed at which communication between the PC and controller takes place.
     */
    SerialCom();
    SerialCom(const char* portName, unsigned short baudRate);

    /** \brief "initSerialCom" is used to initiate the SerialCom object with port name and baud rate.
     *  The function puts the object in the same state as the constructor with transfer parameters.
     *  It can be used to change the port name and the baud rate of a SerialCom object.
     *
     *  \param portName : The port name is used to open a serial connection via the port name for the controller specified by the operating system.
     *  \param baudRate : The baud rate determines the transmission speed at which communication between the PC and controller takes place.
     */
    void initSerialCom(const char* portName, unsigned short baudRate);

    /** \brief Uses the set port name and the baud rate of the class to open a serial connection.
     *  If the opening is successful, the value in "port_" stands for the open connection and
     *  can be used for communication.
     *
     *  \return Returns TRUE on successful opening of a serial connection, otherwise it returns FALSE.
     */
    bool openSerialCom();

    /** \brief Closes a serial connection.
     *
     *  \return Returns TRUE on successful closing of a serial connection, otherwise it returns FALSE.
     */
     bool closeSerialCom();

    /** \brief "writeSerialCom" is used to write commands to the controller via the open serial connection.
     *
     * \param command[] : Contains the command to be sent (size of 1, 2 or 4 bytes, depending on the command).
     * \param sizeCommand : Contains the size of the command (1, 2 or 4).
     * \param response : If the command to be sent expects a return value from the controller, the writeSerialCom is given a pointer to a response array. This can be 1 or 2 bytes in size. If no return value is expected, the pointer is NULL.
     * \param sizeResponse : Contains the size of the command (1 or 2, in case of no expected return value it is 0).
     *
     * \return Returns TRUE on successful writing to a serial connection, otherwise it returns FALSE.
     */
    bool writeSerialCom(unsigned char command[], unsigned short sizeCommand, unsigned char *response, unsigned short sizeResponse);

    /** \brief Returns the port HANDLE under Windows or the integer value for the connection under Linux.
     *
     *
     * \return port_
     */
    #ifdef _WIN32
        HANDLE getPort();
    #else
        int getPort();
    #endif
};

#endif // SERIALCOM_HPP_INCLUDED
