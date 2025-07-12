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

	std::string	m_RequestBuffer;
    char*       m_Content;
	bool		m_HasContent;
	std::string	m_Request;
	std::string	m_URL;
	std::string	m_Protocol;
	std::string	m_Version;
	std::string	m_ResponseText;
	std::string m_MessageLine;
	long		m_ResponseCode;
	long		m_MessageType;
    int         _ContentSize;
};

}

#endif
