#include "StdAfx.h"
#include "SerialCtrl.h"

#define DEFAULT_BAUDRATE 19200

SerialCtrl::SerialCtrl(void) :m_portStatus(FALSE), m_portHandle(NULL)
{
	m_portConfig.ByteSize = 8;            // Byte of the Data.
	m_portConfig.StopBits = ONESTOPBIT;   // Use one bit for stopbit.
	m_portConfig.Parity = NOPARITY;       // No parity bit
	m_portConfig.BaudRate = CBR_9600;     // Buadrate 9600 bit/sec
	m_portStatus = false;
}

SerialCtrl::~SerialCtrl(void)
{
	m_portHandle = NULL;
}

void SerialCtrl::SetPortStatus(BOOL bOnOff)
{
	m_portStatus = bOnOff;
}

BOOL SerialCtrl::GetPortStatus(void)
{
	return m_portStatus;
}

HANDLE SerialCtrl::GetPortHandle(void)
{
	return m_portHandle;
}



BOOL SerialCtrl::OpenPort(DCB dcb, const char * portName)
{

	if (m_portStatus == FALSE)  // if port is opened already, not open port again.
	{
		m_portHandle = CreateFile(portName,  // Specify port device: default "COM1"
			GENERIC_READ | GENERIC_WRITE,       // Specify mode that open device.
			0,                                  // the devide isn't shared.
			NULL,                               // the object gets a default security.
			OPEN_EXISTING,                      // Specify which action to take on file. 
			0,                                  // default.
			NULL);                              // default.

		// Get current configuration of serial communication port.
		if (GetCommState(m_portHandle,&m_portConfig) == 0)
		{
			AfxMessageBox("Get configuration port has problem.");
			return FALSE;
		}
		// Assign user parameter.
		m_portConfig.BaudRate = DEFAULT_BAUDRATE; // $$$$ THIS IS A BUG!!!! dcb.BaudRate;    // Specify buad rate of communicaiton.
		m_portConfig.StopBits = dcb.StopBits;    // Specify stopbit of communication.
		m_portConfig.Parity = dcb.Parity;        // Specify parity of communication.
		m_portConfig.ByteSize = dcb.ByteSize;    // Specify  byte of size of communication.

		// Set current configuration of serial communication port.
		if (SetCommState(m_portHandle,&m_portConfig) == 0)
		{
			AfxMessageBox("Set configuration port has problem.");
			return FALSE;
		}

		// instance an object of COMMTIMEOUTS.
		COMMTIMEOUTS comTimeOut;                   
		comTimeOut.ReadIntervalTimeout = 3;
		comTimeOut.ReadTotalTimeoutMultiplier = 3;
		comTimeOut.ReadTotalTimeoutConstant = 2;
		comTimeOut.WriteTotalTimeoutMultiplier = 3;
		comTimeOut.WriteTotalTimeoutConstant = 2;
		SetCommTimeouts(m_portHandle,&comTimeOut);		// set the time-out parameter into device control.
		m_portStatus = TRUE; 
		return TRUE;      
	}
	return FALSE;
}


BOOL SerialCtrl::Read(char * inputData, const unsigned int & sizeBuffer, unsigned long & length)
{
	if (ReadFile(m_portHandle,  // handle of file to read
		inputData,               // handle of file to read
		sizeBuffer,              // number of bytes to read
		&length,                 // pointer to number of bytes read
		NULL) == 0)              // pointer to structure for data
	{
		// AfxMessageBox("Reading of serial communication has problem.");
		return FALSE;
	}
	if (length > 0)
	{
		inputData[length] = NULL; // Assign end flag of message.
		return TRUE;  
	}  
	return TRUE;
}

BOOL SerialCtrl::Write(const char * outputData, const unsigned int & sizeBuffer, unsigned long & length)
{
	if (length > 0)
	{
		if (WriteFile(m_portHandle, // handle to file to write to
			outputData,              // pointer to data to write to file
			sizeBuffer,              // number of bytes to write
			&length,NULL) == 0)      // pointer to number of bytes written
		{
			AfxMessageBox("Reading of serial communication has problem.");
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL SerialCtrl::ClosePort(void)
{
	if (m_portStatus == TRUE)               // Port need to be open before.
	{
		m_portStatus = FALSE;                 // Update status
		if(CloseHandle(m_portHandle) == 0)    // Call this function to close port.
		{
			AfxMessageBox("Port Closing isn't successed.");
			return FALSE;
		}    
		return TRUE;
	}
	return FALSE;
}



