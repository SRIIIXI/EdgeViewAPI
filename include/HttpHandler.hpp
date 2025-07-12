#ifndef	_HTTP_HANDLER
#define	_HTTP_HANDLER

#include "Defines.hpp"
#include "ResourceHandler.hpp"
#include "AbnfMessage.hpp"
#include "AbnfAdapter.hpp"

class HttpHandler : public ResourceHandler
{
public:
    HttpHandler();
    ~HttpHandler();
    void setRootFolder(const char* strRootDir);
    void handleProtocol(AbnfMessage* message, AbnfAdapter *sourceDevice);

protected:
    void handleHead(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice);
    void handleGet(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice);
    void handlePut(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice);
    void handlePost(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice);
    void handleDelete(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice);
    void handleTrace(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice);
    void handleOptions(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice);
    void handleConnect(AbnfMessage *messagePtr, AbnfAdapter *sourceDevice);

};

#endif
