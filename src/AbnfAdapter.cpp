#include "AbnfAdapter.hpp"
#include "Responder.hpp"

AbnfAdapter::AbnfAdapter()
{
	m_ResponderPtr = NULL;
}

AbnfAdapter::AbnfAdapter(Responder *inResponder)
{
	if(m_ResponderPtr)
	{
		delete m_ResponderPtr;
	}
	
	m_ResponderPtr = new Responder();

	m_ResponderPtr = inResponder;
	registerResponder(m_ResponderPtr);
}

AbnfAdapter::AbnfAdapter(SOCKET inSocket)
{
	if(m_ResponderPtr)
	{
		delete m_ResponderPtr;
	}
	
	m_ResponderPtr = new Responder();

    m_ResponderPtr->createSocket(inSocket);
	registerResponder(m_ResponderPtr);
}

bool AbnfAdapter::createClientAdapter(const char* host, int port)
{
	if(m_ResponderPtr)
	{
		delete m_ResponderPtr;
	}

	m_ResponderPtr = new Responder();

    if(!m_ResponderPtr->createSocket(host, port))
    {
        return false;
    }

    int errcode = -1;

    if(!m_ResponderPtr->connectSocket(errcode))
    {
        return false;
    }

    return true;
}


AbnfAdapter::~AbnfAdapter()
{
    if(m_ReceiverThread)
        pthread_cancel(m_ReceiverThread);
}

bool AbnfAdapter::OnMessage(const void* data)
{
    //if(m_ObserverPtr)
    //{
    //    m_ObserverPtr->handleNotification((void*)data,this);
    //}

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

    m_ResponderPtr->sendBuffer(httpheader);
    m_ResponderPtr->sendBuffer(httpbody);

    return true;
}

bool AbnfAdapter::sendPacket(const char* data, int len)
{
	if(!m_ResponderPtr)
	{
		return false;
	}

    if(!m_ResponderPtr->isConnected())
	{
		return false;
	}

    return m_ResponderPtr->sendBuffer(data,len);
}

bool AbnfAdapter::sendAbnfPacket(AbnfMessage &message)
{
	if(!m_ResponderPtr)
	{
		return false;
	}

    if(!m_ResponderPtr->isConnected())
    {
        return false;
    }

    std::string data;
    message.serialize(data);
    int len = data.length();
    return m_ResponderPtr->sendBuffer(data.c_str(), len);
}

bool AbnfAdapter::receiveAbnfPacket(AbnfMessage &message)
{
	if(!m_ResponderPtr)
	{
		return false;
	}

    std::string header;
    if(m_ResponderPtr->receiveString(header,(char*)"\r\n\r\n"))
    {
        message.setHeader(header.c_str());
        message.deSerialize();

        if(message.hasBody())
        {
            char *buffer = NULL;

            int len = message.getContentSize();

            buffer = new char[len+1];
            memset(buffer,0,len+1);

            if(m_ResponderPtr->receiveBuffer(buffer,len))
            {
                message.attachBody(buffer);
                delete buffer;
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
        if(receiveAbnfPacket(_Message))
        {
            OnMessage((void*)&_Message);
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
	m_ResponderPtr = inResponder;
}

bool AbnfAdapter::startResponder()
{
    memset(&m_ReceiverThread,0,sizeof(m_ReceiverThread));

    m_ReceiverTID = pthread_create(&m_ReceiverThread, NULL,&(AbnfAdapter::receiverThreadFunction),(void*)this);
    if(m_ReceiverTID !=0)
    {
        return false;
    }

    return true;
}

bool AbnfAdapter::releaseResponder()
{
    return m_ResponderPtr->closeSocket();
}

char* AbnfAdapter::getDeviceName()
{
	return &m_DeviceName[0];
}

Responder* AbnfAdapter::getResponder()
{
	return m_ResponderPtr;
}


void AbnfAdapter::setDeviceName(const char* name)
{
	strcpy(m_DeviceName,name);
}

void* AbnfAdapter::receiverThreadFunction(void *lpParameter)
{
	AbnfAdapter* ptr = (AbnfAdapter*)lpParameter;

	ptr->invokeHandler();

	return 0;
}

