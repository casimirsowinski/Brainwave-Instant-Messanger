// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SOCKETUDPCOMM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SOCKETUDPCOMM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SOCKETUDPCOMM_EXPORTS
#define SOCKETUDPCOMM_API __declspec(dllexport)
#else
#define SOCKETUDPCOMM_API __declspec(dllimport)
#endif

#include <stdio.h>
//#include <winsock2.h>

class SOCKETUDPCOMM_API CSocketUDPComm
{
private:
	static const int BIND_LOCAL_SEND_PORT = 55557;
    static const int INOUT_BUFFER_SIZE = 8192;

    WSADATA m_wsaData;
    int m_wsaerr;
    SOCKET m_sendSocket;
    SOCKET m_rcvSocket;
    int m_localListenPort;
    sockaddr_in m_remoteAddr;
    int m_remoteAddrLen;
    bool m_terminateNow;
    char m_inBuff[INOUT_BUFFER_SIZE];
    int m_inBuffSz;
    //int m_localBindPort;

    void (*m_rcvdDataCallBackPtr)(const char*, const char*);

    // Define private class methods. These are only called from
    // other class methods and not from "outside" the class.
    int InitWSALibrary();
    int CreateSocket(SOCKET &localSocket);
    int BindLocal(SOCKET &localSocket, int localBindPort);

    // Define a method to be the listener thread entry point and
    // make it static so it can be made a member of this class
    // for convenience (encapsulation!).
    static unsigned __stdcall StartServerCommThread(void* param)
    {
        CSocketUDPComm* ptr = (CSocketUDPComm*)param;
        // Just call another member function so can keep code in
        // the .cpp file.
        ptr->ReceiveFromRemoteUser();

        return 0;
    }

    int ReceiveFromRemoteUser();

public:

    // Define public class methods. These may be called from
    // other class methods and from "outside" the class.

    CSocketUDPComm();
    ~CSocketUDPComm();

    /*******************************************************************
    // Called when connecting to a remote system. Pass in the IP address
    // and port number of the remote computer to which to connect. Note
    // that the remote system must be listening on that port for comm to
    // work. The local port that the system uses (binds to) through
	// which to send the packet can be specified as an optional default
	// parameter. If creating multiple instances of this class, this
	// parameter should be used since only a single app may bind to a
	// port on a system.
    //
    // Returns 0 if all OK or -1 on error.
    *******************************************************************/
    int InitializeSenderSocket(
		char* remoteIPAddr,
		int sendPort,
		int localSendingBindPort = BIND_LOCAL_SEND_PORT);

    /*******************************************************************
    // Pass in the data to be sent to the remote system.
    //
    // Returns 0 if all OK or -1 on error.
    *******************************************************************/
    int SendData(char* dataToSend);

    /*******************************************************************
    // This method must be called to receive any data from a remote
    // system. The listening port must be specified. This class will
    // only receive data that is sent to this port. Must also specify
    // a callback function so this thread may pass any data received
    // to the calling application. The first argument of the callback
    // function is the data received and the second argument is the
    // IP address of the sender (remote computer sending the data).
    //
    // Returns 0 if all OK or -1 on error.
    *******************************************************************/
    int StartListenerThread(
        int listenPort,
        void (*callBackPtr)(const char*, const char*));

    /*******************************************************************
    // Query if this dll received information that says the application
    // should shutdown.
    //
    // Returns true if the app should shutdown and false otherwise.
    *******************************************************************/
    bool ShallTerminateNow()
    {
        return m_terminateNow;
    }

    /*******************************************************************
    // Use this method to tell this dll to shutdown now. All threads
    // will be terminated.
    *******************************************************************/
    void SetTerminateNow(bool val)
    {
        m_terminateNow = val;
    }
};
