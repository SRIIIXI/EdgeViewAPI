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
    bool				is_connected;
    SOCKET				socket_handle;
    sockaddr_in			server_address;
    std::string			server_name;
    int					server_port;
    int					pre_fetched_buffer_size;
    unsigned char*		pre_fetched_buffer;
};

typedef Responder Client;

}

#endif

