#include "HttpHandler.hpp"

HttpHandler::HttpHandler()
{
}

HttpHandler::~HttpHandler()
{
}

void HttpHandler::handleProtocol(AbnfMessage* message, AbnfAdapter *sourceDevice)
{
    if(message->getMessageType() == REQUEST)
	{
        printf("[HTTPServerLite]: %s %s\n",message->getRequest(), message->getURL());

        if(strcmp((const char*)message->getRequest(),"GET")==0)
		{
			handleGet(message,sourceDevice);
		}

        if(strcmp((const char*)message->getRequest(),"PUT")==0)
		{
			handlePut(message,sourceDevice);
		}

        if(strcmp((const char*)message->getRequest(),"HEAD")==0)
		{
			handleHead(message,sourceDevice);
		}

        if(strcmp((const char*)message->getRequest(),"POST")==0)
		{
			handlePost(message,sourceDevice);
		}

        if(strcmp((const char*)message->getRequest(),"DELETE")==0)
		{
			handleDelete(message,sourceDevice);
		}

        if(strcmp((const char*)message->getRequest(),"TRACE")==0)
		{
			handleTrace(message,sourceDevice);
		}

        if(strcmp((const char*)message->getRequest(),"OPTIONS")==0)
		{
			handleOptions(message,sourceDevice);
		}

        if(strcmp((const char*)message->getRequest(),"CONNECT")==0)
		{
			handleConnect(message,sourceDevice);
		}
	}
	else
	{
        printf("Response = %ld %s\n",message->getResponseCode(), message->getResponseText());
	}

    if(message->hasBody())
	{
        printf("HTTP Body\n%s\n",message->getContent());
	}
}

void HttpHandler::handleHead(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice)
{
}

void HttpHandler::handleGet(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice)
{
    if(urlContent != NULL)
    {
        delete urlContent;
        urlContent = NULL;
        contentLength = 0;
    }

	char *httpbody = NULL;
	int httplen = 0;
	String httpheader;
	char buffer[1024];
    String contenttag ="";

    if(loadContent(messagePtr->getURL()))
	{
        contenttag = contentTypeTag;

        printf("[HTTPServerLite]: %s\n\n",contenttag.c_str());

		sprintf(buffer,"HTTP/1.1 200 OK\r\n%c",'\0');
		httpheader = buffer;

		sprintf(buffer,"Cache-Control: private\r\n%c",'\0');
		httpheader += buffer;

		sprintf(buffer,"Content-Type: %s\r\n%c",contenttag.c_str(),'\0');
		httpheader += buffer;

		sprintf(buffer,"Server: ggnhnb745.in002.siemens.net\r\n%c",'\0');
		httpheader += buffer;	

        sprintf(buffer,"Content-Length: %ld\r\n\r\n%c",contentLength,'\0');
		httpheader += buffer;
	}
	else
	{
		String notFound ="<html><head><title>Not Found</title></head><body lang=EN-US><div class=Section1><p class=MsoNormal>404 Requested URL \"";
        notFound += messagePtr->getURL();
		notFound +=	"\" not found</p></div></body></html>";
		long messageLen = notFound.length();

		sprintf(buffer,"HTTP/1.1 404 Not Found\r\n%c",'\0');
		httpheader = buffer;

		sprintf(buffer,"Cache-Control: private\r\n%c",'\0');
		httpheader += buffer;

		sprintf(buffer,"Content-Type: text/html\r\n%c",'\0');
		httpheader += buffer;

		sprintf(buffer,"Server: ggnhnb745.in002.siemens.net\r\n%c",'\0');
		httpheader += buffer;
		
		sprintf(buffer,"Content-Length: %d\r\n\r\n%c",messageLen,'\0');
		httpheader += buffer;

		httpbody = new char[notFound.length() + 1];
		memcpy(httpbody,notFound.c_str(),notFound.length());
		httplen = notFound.length();
	}

	int headerlen = httpheader.length();
    sourceDevice->getResponder()->sendBuffer(httpheader.c_str(), headerlen);

    if(contentLength > 0)
	{
        int blen = contentLength;
        sourceDevice->getResponder()->sendBuffer(urlContent, blen);
	}

    if(urlContent != NULL)
    {
        delete urlContent;
        urlContent = NULL;
        contentLength = 0;
    }

	if(httpbody != NULL)
    {
		delete httpbody;
    }

}

void HttpHandler::handlePut(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice)
{
}

void HttpHandler::handlePost(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice)
{
}

void HttpHandler::handleDelete(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice)
{
}

void HttpHandler::handleTrace(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice)
{
}

void HttpHandler::handleOptions(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice)
{
}

void HttpHandler::handleConnect(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice)
{
}

void HttpHandler::setRootFolder(const char* strRootDir)
{
    setServerRoot(strRootDir);
}
