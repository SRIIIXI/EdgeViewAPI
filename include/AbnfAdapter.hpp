#ifndef	_ABNF_ADAPTER
#define	_ABNF_ADAPTER

#include "Defines.hpp"
#include "Responder.hpp"
#include "AbnfMessage.hpp"

extern "C"
{

class AbnfAdapter
{
public:
    AbnfAdapter();
    AbnfAdapter(Responder *inResponder);
    AbnfAdapter(SOCKET inSocket);
    virtual ~AbnfAdapter();
    void registerResponder(Responder *inResponder);
    bool startResponder();
    bool createClientAdapter(const char* host, int port);
    bool receiveAbnfPacket(AbnfMessage &message);
    bool sendAbnfPacket(AbnfMessage &message);

    Responder* getResponder();
    virtual bool OnMessage(const void* data);
    bool releaseResponder();
    void setDeviceName(const char* name);
    char*	getDeviceName();

protected:
    bool sendPacket(const char* data, int len);
    virtual bool invokeHandler();
    static void* receiverThreadFunction(void *lpParameter);
	Responder		*m_ResponderPtr;
	char			m_DeviceName[33];
private:
    pthread_t		m_ReceiverThread;
	unsigned long		m_ReceiverTID;
    AbnfMessage 		_Message;
};

}

#endif

