#include "AbnfAdapter.hpp"
#include "Responder.hpp"

typedef struct ThreadReference
{
#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
    HANDLE thread_object;
#else
    pthread_t thread_object;
#endif
}ThreadReference;

AbnfAdapter::AbnfAdapter()
{
	responder_ptr = NULL;
}

AbnfAdapter::AbnfAdapter(Responder *inResponder)
{
	if(responder_ptr)
	{
		delete responder_ptr;
	}
	
	responder_ptr = new Responder();

	responder_ptr = inResponder;
	registerResponder(responder_ptr);
}

AbnfAdapter::AbnfAdapter(SOCKET inSocket)
{
	if(responder_ptr)
	{
		delete responder_ptr;
	}
	
	responder_ptr = new Responder();

    responder_ptr->createSocket(inSocket);
	registerResponder(responder_ptr);
}

bool AbnfAdapter::createClientAdapter(const char* host, int port)
{
	if(responder_ptr)
	{
		delete responder_ptr;
	}

	responder_ptr = new Responder();

    if(!responder_ptr->createSocket(host, port))
    {
        return false;
    }

    int errcode = -1;

    if(!responder_ptr->connectSocket(errcode))
    {
        return false;
    }

    return true;
}


AbnfAdapter::~AbnfAdapter()
{
    if (thread_reference->thread_object)
    {
        #if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
                SuspendThread(thread_reference->thread_object);
                TerminateThread(thread_reference->thread_object, 0);
                CloseHandle(thread_reference->thread_object);
        #else
                pthread_cancel(thread_reference->thread_object);
        #endif

        delete thread_reference;
        thread_reference = NULL;    
    }
}

bool AbnfAdapter::OnMessage(const void* data)
{
    char buffer[1024] = {0};
    std::string httpheader;
    std::string contenttag = "text/html";
    long contentLength = 0;
    std::string httpbody = "<!DOCTYPE html><html><body><h1>My First Heading</h1><p>My first paragraph.</p></body></html>";

    contentLength = httpbody.length();

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"HTTP/1.1 200 OK\r\n%c",'\0');
    httpheader = buffer;

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"Cache-Control: private\r\n%c",'\0');
    httpheader += buffer;

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"Content-Type: %s\r\n%c",contenttag.c_str(),'\0');
    httpheader += buffer;

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"Server: localhost\r\n%c",'\0');
    httpheader += buffer;

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"Content-Length: %ld\r\n\r\n%c",contentLength,'\0');
    httpheader += buffer;

    responder_ptr->sendBuffer(httpheader);
    responder_ptr->sendBuffer(httpbody);

    return true;
}

bool AbnfAdapter::sendPacket(const char* data, int len)
{
	if(!responder_ptr)
	{
		return false;
	}

    if(!responder_ptr->isConnected())
	{
		return false;
	}

    return responder_ptr->sendBuffer(data,len);
}

bool AbnfAdapter::sendAbnfPacket(AbnfMessage &message)
{
	if(!responder_ptr)
	{
		return false;
	}

    if(!responder_ptr->isConnected())
    {
        return false;
    }

    std::string data;
    message.serialize(data);
    int len = data.length();
    return responder_ptr->sendBuffer(data.c_str(), len);
}

bool AbnfAdapter::receiveAbnfPacket(AbnfMessage &message)
{
	if(!responder_ptr)
	{
		return false;
	}

    std::string header;
    if(responder_ptr->receiveString(header,(char*)"\r\n\r\n"))
    {
        message.setHeader(header.c_str());
        message.deSerialize();

        if(message.hasBody())
        {
            char *buffer = NULL;

            int len = message.getContentSize();

            buffer = new char[len+1];
            memset(buffer,0,len+1);

            if(responder_ptr->receiveBuffer(buffer,len))
            {
                message.attachBody(buffer);
                delete [] buffer;
                buffer = NULL;
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }

    return true;
}


bool AbnfAdapter::invokeHandler()
{
	while(true)
	{
        if(receiveAbnfPacket(message))
        {
            OnMessage((void*)&message);
        }
		else
		{
			return false;
		}
	}
	return true;
}

void AbnfAdapter::registerResponder(Responder *inResponder)
{
	responder_ptr = inResponder;
}

bool AbnfAdapter::startResponder()
{
    thread_reference = new ThreadReference;
    memset(&thread_reference->thread_object,0,sizeof(ThreadReference::thread_object));

    #if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
        thread_reference->thread_object = CreateThread(NULL, NULL, &AbnfAdapter::receiverThreadFunction, (LPVOID)this, CREATE_SUSPENDED, &receiver_id);
        if (thread_reference->thread_object == NULL)
        {
            return false;
        }
        ResumeThread(thread_reference->thread_object);
    #else
        receiver_id = pthread_create(&thread_reference->thread_object, NULL,&(AbnfAdapter::receiverThreadFunction),(void*)this);
        if(receiver_id !=0)
        {
            return false;
        }
    #endif

    return true;
}

bool AbnfAdapter::releaseResponder()
{
    return responder_ptr->closeSocket();
}

char* AbnfAdapter::getDeviceName()
{
	return &device_name[0];
}

Responder* AbnfAdapter::getResponder()
{
	return responder_ptr;
}


void AbnfAdapter::setDeviceName(const char* name)
{
	strcpy(device_name,name);
}

THREAD_PROC_RETURN_TYPE AbnfAdapter::receiverThreadFunction(void* lpParameter)
{
    AbnfAdapter* ptr = (AbnfAdapter*)lpParameter;
    ptr->invokeHandler();
	return 0;
}

