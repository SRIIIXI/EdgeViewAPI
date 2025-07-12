#ifndef	_ABNF_ADAPTER
#define	_ABNF_ADAPTER

#include "Defines.hpp"
#include "Responder.hpp"
#include "AbnfMessage.hpp"

extern "C"
{
	typedef struct ThreadReference;

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
        static THREAD_PROC_RETURN_TYPE receiverThreadFunction(void *lpParameter);
	    Responder		*responder_ptr;
	    char			device_name[33];
    private:
        ThreadReference*	thread_reference;
	    unsigned long		receiver_id;
        AbnfMessage 		message;
    };

}

#endif

