#ifndef _ABNF_MESSAGE
#define _ABNF_MESSAGE

#include "Defines.hpp"
#include "StringHandler.hpp"
#include "NetworkHelper.hpp"

extern "C"
{

typedef enum ContentType
{
	Text=0,
	Image=1,
	Audio=2,
	Video=3,
	Binary=4
}ContentType;

class AbnfMessage  : public StringHandler
{
public:
    AbnfMessage();
    AbnfMessage(const char* buffer);
    virtual ~AbnfMessage();

	// Incoming packet functions
    void setHeader(const char* buffer);
    bool deSerialize();

	// Outgoing packet functions
	// Request
    void setProtocolInformation(const char* request, const char* URL, const char* protocol, const char* version);
	// Response
    void setProtocolInformation(const char* protocol, const char* version, long responsecode, const char* responsetext);
    void addHeader(const char* field, const char* value);
    void serialize(std::string &sipString);

	// Common for transmission/reception
    void	attachBody(const char* buffer);

	// Reset all internal data - useful when we reuse the packet
	void reset();

    bool	hasBody();
    const char*	getRequest();
    const char*	getProtocol();
    const char*	getURL();
    const char*	getVersion();
    const char*	getResponseText();
    const char*	getContent();
    long	getResponseCode();
    long	getMessageType();
    void	getFieldValue(const char* fieldName, std::string &value);
    int getContentSize();

private:
    std::vector<std::string> keyList;
    std::vector<std::string> valueList;

    void decodeMessageIdentificationLine(const char* messageLine);
    void encodeMessageIdentificationLine();
    void processLine(const char* line, std::string &field, std::string &value);
    void getLine(std::string &line);

	std::string	request_buffer;
    char*       http_content;
	bool		has_content;
	std::string	request;
	std::string	http_url;
	std::string	protocol;
	std::string	version;
	std::string	response_text;
	std::string message_line;
	long		response_code;
	long		message_type;
    int         content_size;
};

}

#endif
