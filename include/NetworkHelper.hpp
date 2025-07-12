#ifndef	_NETWORK_HELPER
#define	_NETWORK_HELPER

#include "Defines.hpp"

#if defined(_WIN32) || defined(WIN32)
#define LAST_ERROR    ::GetLastError()
#define SOCK_ERROR    ::WSAGetLastError()
#define closesocket(n) ::closesocket(n)
#else
#define LAST_ERROR    errno
#define SOCK_ERROR    errno
#define SOCKET int
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR	 (-1)
#define LPSOCKADDR sockaddr*
#define closesocket(n) close(n)
#endif



// Framework defined Network Events
const long STARTUPEVENT = 0;
const long NODEREGEVENT = 1;
const long NODEDISEVENT = 2;
const long NODECONEVENT = 3;
const long NODEURGEVENT = 4;
const long NETDATAEVENT = 5;
const long SVRSTOPEVENT = 6;

// Protocol packet differentiation
const long REQUEST = 10;
const long RESPONSE = 11;

extern "C"
{

class NetworkHelper
{
public:
	NetworkHelper();
	virtual ~NetworkHelper();
    static bool getEndpoint(const char* hostname, int port, sockaddr_in *socketAddress);
    static bool isIPAddress(char* str);
    static bool isIP6Address(char* str);
    static bool isIP4Address(char* str);
    static void getLocalHostName(char *hostname);
    static void getLocalIPAddress(const SOCKET newServerfd, char *ipaddress);
private:
};

}

#endif

