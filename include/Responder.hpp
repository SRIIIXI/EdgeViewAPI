#ifndef	_RESPONDER
#define	_RESPONDER

#include "Defines.hpp"
#include "NetworkHelper.hpp"
#include "StringHandler.hpp"

extern "C"
{

class Responder
{
public:
	Responder();
	Responder(const Responder& other);
	Responder& operator=( const Responder& other);

	virtual ~Responder();
	bool createSocket(const char* servername, int serverport);
	bool createSocket(SOCKET inSocket);
	bool connectSocket(int &returncode);
	bool closeSocket();
	bool isConnected();

	bool sendBuffer(const char* data, int &len);
    bool sendBuffer(const std::string &str);

    bool receiveBuffer(char* ioBuffer,int len);
    bool receiveString(std::string &ioStr, const char* delimeter);

	SOCKET getSocket();

    int pendingPreFetchedBufferSize();
private:
    bool				_Connected;
    SOCKET				_Socket;
    sockaddr_in			_ServerAddress;
    std::string			_ServerName;
    int					_ServerPort;
    int					_PreFetchedBufferSize;
    unsigned char*		_PreFetchedBuffer;
};

typedef Responder Client;

}

#endif

