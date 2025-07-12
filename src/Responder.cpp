#include "Responder.hpp"
#include "StringHandler.hpp"

Responder::Responder()
{
    socket_handle = 0;
    is_connected = false;
    memset((void*)&server_address,0,sizeof(sockaddr_in));
    pre_fetched_buffer_size = 0;
    pre_fetched_buffer = NULL;
	server_name.clear();
	server_port = 0;
}

Responder::Responder(const Responder& other)
{
    memset((void*)&server_address,0,sizeof(sockaddr_in));

    pre_fetched_buffer_size = 0;

	if(pre_fetched_buffer != NULL)
	{
		delete pre_fetched_buffer;
	}

	pre_fetched_buffer = NULL;
	
	socket_handle = other.socket_handle;
	is_connected = other.is_connected;
	memcpy((void*)&server_address, (void*)&other.server_address, sizeof(sockaddr_in));
	server_port = other.server_port;

	if(other.pre_fetched_buffer_size > 0)
	{
		pre_fetched_buffer = new unsigned char[other.pre_fetched_buffer_size];
		pre_fetched_buffer_size = other.pre_fetched_buffer_size;
		memcpy((unsigned char*)&pre_fetched_buffer, (unsigned char*)&other.pre_fetched_buffer, pre_fetched_buffer_size);
	}
}

Responder& Responder::operator=( const Responder& other)
{
	 memset((void*)&server_address,0,sizeof(sockaddr_in));

    pre_fetched_buffer_size = 0;

	if(pre_fetched_buffer != NULL)
	{
		delete pre_fetched_buffer;
	}
	pre_fetched_buffer = NULL;
	
	socket_handle = other.socket_handle;
	is_connected = other.is_connected;
	memcpy((void*)&server_address, (void*)&other.server_address, sizeof(sockaddr_in));
	server_port = other.server_port;

	if(other.pre_fetched_buffer_size > 0)
	{
		pre_fetched_buffer = new unsigned char[other.pre_fetched_buffer_size];
		pre_fetched_buffer_size = other.pre_fetched_buffer_size;
		memcpy((unsigned char*)&pre_fetched_buffer, (unsigned char*)&other.pre_fetched_buffer, pre_fetched_buffer_size);
	}

	return *this;
}

Responder::~Responder()
{
	closeSocket();
	while(isConnected()==true)
	{
	}

    if(pre_fetched_buffer != NULL)
    {
        delete pre_fetched_buffer;
    }
}

bool Responder::createSocket(const char* servername, int serverport)
{
    server_name = servername;
    server_port = serverport;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(serverport);
    u_long nRemoteAddr;

    char ipbuffer[32]={0};
    strncpy(ipbuffer, servername, 31);

    bool ip = NetworkHelper::isIP4Address(ipbuffer);

    if(!ip)
    {
        nRemoteAddr = inet_addr(server_name.c_str());
        if (nRemoteAddr == INADDR_NONE)
        {
            hostent* pHE = gethostbyname(server_name.c_str());
            if (pHE == 0)
            {
                nRemoteAddr = INADDR_NONE;
                return false;
            }
            nRemoteAddr = *((u_long*)pHE->h_addr_list[0]);
            server_address.sin_addr.s_addr = nRemoteAddr;
        }
    }
    else
    {
        inet_pton (AF_INET, server_name.c_str(), &server_address.sin_addr);
    }


    socket_handle = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(socket_handle == INVALID_SOCKET)
    {
        return false;
    }


    return true;
}

bool Responder::createSocket(SOCKET inSocket)
{
    socket_handle = inSocket;
    is_connected = true;
	return true;
}


bool Responder::connectSocket(int &returncode)
{
    if(is_connected == true)
	{
		return true;
	}

	returncode = connect(socket_handle,(sockaddr*)&server_address, sizeof(sockaddr_in));

    if(returncode == SOCKET_ERROR)
	{
		returncode = errno;
		shutdown(socket_handle,2);
        closesocket(socket_handle);
		is_connected = false;

		return false;
	}

	is_connected = true;
	return true;
}

bool Responder::closeSocket()
{
    if(is_connected)
	{
        shutdown(socket_handle,0);
		closesocket(socket_handle);
	}	
    is_connected = false;
	return false;
}

bool Responder::receiveString(std::string &ioStr, const char *delimeter)
{
    char	buffer[1024];
	long	returnvalue;
	std::string	data;
    std::string  currentLine, nextLine;

	data.erase();

    if(pre_fetched_buffer_size > 0)
	{
        if(strstr((char*)pre_fetched_buffer,delimeter)!=0)
		{
            StringHandler::split((const char*)pre_fetched_buffer,delimeter,currentLine,nextLine);

            ioStr = currentLine;
			currentLine.erase();

            delete pre_fetched_buffer;
            pre_fetched_buffer = NULL;
            pre_fetched_buffer_size = nextLine.length();

            if(pre_fetched_buffer_size > 0)
            {
                pre_fetched_buffer = new unsigned char[pre_fetched_buffer_size+1];
                memset(pre_fetched_buffer, 0, pre_fetched_buffer_size+1);
                memcpy(pre_fetched_buffer,nextLine.c_str(),pre_fetched_buffer_size);
            }

			return true;
		}

        data = (char*)pre_fetched_buffer;
        pre_fetched_buffer_size = 0;
        delete pre_fetched_buffer;
        pre_fetched_buffer = NULL;
	}

	while(true)
	{
        memset(&buffer[0],0,1024);
        returnvalue = recv(socket_handle,&buffer[0],1024,0);

		// Error or link down
		if(returnvalue < 1)
		{
            int error = errno;
            ioStr.clear();
			is_connected = false;
			return false;
		}

		data += buffer;

		if(strstr(data.c_str(),delimeter)!=0)
		{
            StringHandler::split(data,delimeter,currentLine, nextLine);

            pre_fetched_buffer_size = nextLine.length();
            
            if(pre_fetched_buffer_size > 0)
            {
                pre_fetched_buffer = new unsigned char[pre_fetched_buffer_size+1];
                memset(pre_fetched_buffer, 0, pre_fetched_buffer_size+1);
                memcpy(pre_fetched_buffer,nextLine.c_str(),pre_fetched_buffer_size);
            }

            ioStr = currentLine;

			data.erase();
			currentLine.erase();
			return true;
		}
	}
	return true;
}

bool Responder::receiveBuffer(char* ioBuffer,int len)
{
	char*	buffer = 0;
	long	bufferpos = 0;
	long	bytesread = 0;
	long	bytesleft = len;

    // If there are pre-fetched bytes left, we have to copy that first and relase memory

    if(pre_fetched_buffer_size > 0)
    {
        memcpy(ioBuffer, pre_fetched_buffer, pre_fetched_buffer_size);
        bytesleft = len - pre_fetched_buffer_size;
        bufferpos = pre_fetched_buffer_size;
        pre_fetched_buffer_size = 0;
        delete pre_fetched_buffer;
        pre_fetched_buffer = NULL;

        if(bytesleft < 1)
        {
            return true;
        }
    }

	while(true)
	{
		buffer = new char[bytesleft+1];
        memset(buffer, 0, bytesleft+1);
        bytesread = recv(socket_handle,buffer,bytesleft,0);

		// Error or link down
		if(bytesread < 1)
		{
            int error = errno;
			delete buffer;
			ioBuffer = 0;
			len	= 0;
			is_connected = false;
			return false;
		}

		memcpy(ioBuffer+bufferpos,buffer,bytesread);
		delete buffer;

		bufferpos = bufferpos + bytesread;

		bytesleft = bytesleft - bytesread;

		if(bufferpos >= len)
		{
			return true;
		}
	}
}

int Responder::pendingPreFetchedBufferSize()
{
    return pre_fetched_buffer_size;
}


bool Responder::sendBuffer(const char* data, int &len)
{
	if(!is_connected)
	{
		return false;
	}

	long sentsize =0;

    sentsize = send(socket_handle, data, len,0);
	if(sentsize==SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

bool Responder::sendBuffer(const std::string &str)
{
	int len = str.length();
	bool ret = sendBuffer(str.c_str(), len);
	return ret;
}

bool Responder::isConnected()
{
    return is_connected;
}

SOCKET Responder::getSocket()
{
	return socket_handle;
}
