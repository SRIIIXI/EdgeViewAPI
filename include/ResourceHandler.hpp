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
    std::string	serverAddress, strServerRoot;
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
    std::string	server_address;
    long content_length;
    char *url_content;
    std::string content_type_tag;
    ContentType content_type;
    std::string	server_root, resolved_url, resolved_filename, root_document;
private:
    FILE*	file_descriptor;
};

}

#endif
