#ifndef	_HTTP_SERVER
#define	_HTTP_SERVER

#include "Responder.hpp"
#include "ResourceHandler.hpp"
#include "HttpHandler.hpp"
#include "EventHandler.hpp"
#include "SignalHandler.hpp"

extern "C"
{


typedef enum RunState
{
    Running =0,
    NormalExit = 1,
    BindFailed = -1,
    ListenFailed = -2,
    StackFailure = -3
}RunState;

class HttpServer : public SignalCallback
{
public:
    HttpServer();
    virtual ~HttpServer();
    RunState run(int port);
	void stop();
    void cleanup();

protected:

    void suspend();
    void resume();
    void shutdown();
    void alarm();
    void reset();
    void childExit();
    void userdefined1();
    void userdefined2();

private:
    SOCKET listener_socket;
    ApplicationCallback*   callback_ptr;
};

}

#endif
