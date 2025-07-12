#ifndef _RESOURCEHANDLER
#define _RESOURCEHANDLER

#include "Defines.hpp"
#include "StringHandler.hpp"
#include "Directory.hpp"
#include "AbnfMessage.hpp"
#include "Responder.hpp"

extern "C"
{

class ApplicationCallback
{
public:
    ApplicationCallback(){}
    virtual ~ApplicationCallback(){}
    virtual bool userDataFunction(const void* ptr, long len)=0;
    void setServerRoot(const char* serverRoot);
    void setServerAddress(const char* serveraddr);
    String	serverAddress, strServerRoot;
};

class ResourceHandler
{
public:
	ResourceHandler();
	~ResourceHandler();
    void setServerRoot(const char*  serverRoot);
    void setRootDocument(const char*  rootDoc);
    void setServerAddress(const char* serveraddr);

    bool loadContent(const char* url);
    virtual void handleProtocol(AbnfMessage* message, Responder* sourceDevice)=0;
    virtual void handleProtocol(AbnfMessage* message)=0;
protected:
    String	serverAddress;
    long contentLength;
    char *urlContent;
    String contentTypeTag;
    ContentType contentype;
    String	strServerRoot, resolvedURL, resolvedFileName, rootDocument;
private:
    FILE*	fileDescriptor;
};

}

#endif
