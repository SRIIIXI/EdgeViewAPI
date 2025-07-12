// ABNFMessage.cpp: implementation of the ABNFMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "AbnfMessage.hpp"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AbnfMessage::AbnfMessage()
{
	m_RequestBuffer.erase();
    m_Content = NULL;
	m_HasContent = false;
	m_Request.erase();
	m_URL.erase();
	m_Protocol.erase();
	m_Version.erase();
	m_ResponseText.erase();
	m_ResponseCode = -1;
	m_MessageType = -1;
    keyList.clear();
    valueList.clear();
    _ContentSize = 0;
}

AbnfMessage::AbnfMessage(const char* buffer)
{
	m_RequestBuffer.erase();
    m_Content = NULL;
    m_HasContent = false;
	m_Request.erase();
	m_URL.erase();
	m_Protocol.erase();
	m_Version.erase();
	m_ResponseText.erase();
	m_ResponseCode = -1;
	m_MessageType = -1;
	m_RequestBuffer = buffer;
    keyList.clear();
    valueList.clear();
    _ContentSize = 0;
}

void AbnfMessage::reset()
{
}

AbnfMessage::~AbnfMessage()
{

    if(	m_Content != NULL)
    {
        delete m_Content;
        m_Content = NULL;
    }

    m_RequestBuffer.erase();
    m_HasContent = false;
    m_Request.erase();
    m_URL.erase();
    m_Protocol.erase();
    m_Version.erase();
    m_ResponseText.erase();
    m_ResponseCode = -1;
    m_MessageType = -1;
    keyList.clear();
    valueList.clear();
    _ContentSize = 0;
}

void AbnfMessage::setHeader(const char* buffer)
{
	reset();
	m_RequestBuffer = buffer;
}

void AbnfMessage::attachBody(const char* buffer)
{
    if(	m_Content != NULL)
    {
        delete m_Content;
        m_Content = NULL;
    }

    m_Content = new char[_ContentSize];
    memcpy(m_Content,buffer,_ContentSize);
}

const char*	AbnfMessage::getRequest()
{
	return m_Request.c_str();
}

const char*	AbnfMessage::getProtocol()
{
	return m_Protocol.c_str();
}

const char*	AbnfMessage::getURL()
{
	return m_URL.c_str();
}

const char*	AbnfMessage::getVersion()
{
	return m_Version.c_str();
}

const char*	AbnfMessage::getResponseText()
{
	return m_ResponseText.c_str();
}

long AbnfMessage::getResponseCode()
{
	return m_ResponseCode;
}

long AbnfMessage::getMessageType()
{
	return m_MessageType;
}

const char*	AbnfMessage::getContent()
{
    return m_Content;
}

int AbnfMessage::getContentSize()
{
    return _ContentSize;
}


void AbnfMessage::getFieldValue(const char* fieldName, std::string &value)
{
    std::vector<std::string>::iterator iter = std::find(keyList.begin(), keyList.end(), fieldName);

    if(iter == keyList.end())
	{
		value = "";
	}
    else
    {
        int position = std::distance( keyList.begin(), iter) ;
        value = valueList.at(position);
    }
	return;
}


bool AbnfMessage::deSerialize()
{
	std::string fieldValueParams;
	std::string field, value;

	getLine(m_MessageLine);
	decodeMessageIdentificationLine(m_MessageLine.c_str());

	while(true)
	{
		getLine(fieldValueParams);
		processLine(fieldValueParams.c_str(), field, value);
		if(field.length() < 1)
		{
			break;
		}

        keyList.push_back(field);
        valueList.push_back(value);

		if(strcmp(field.c_str(),"Content-Length") == 0)
		{
			if(atoi(value.c_str()) > 0)
			{
				m_HasContent = true;
                _ContentSize = atoi(value.c_str());
			}
			else
			{
				m_HasContent = false;
			}
			break;;
		}
	}
	return true;
}

bool AbnfMessage::hasBody()
{
	return m_HasContent;
}

void AbnfMessage::getLine(std::string &line)
{
    if(m_RequestBuffer.length()<1)
    {
        line.clear();
        return;
    }

	std::string next;
    int pos = split(m_RequestBuffer.c_str(),"\r\n",line,next);

    if(pos == -1)
    {
        line = m_RequestBuffer;
        m_RequestBuffer.clear();
        return;
    }

	m_RequestBuffer = next;
}

void AbnfMessage::processLine(const char *line, std::string &field, std::string &value)
{
    int delimeterpos = characterposition(line,':');

	field = "";
	value = "";
	int ctr = 0;
	for(ctr = 0; line[ctr] != 0; ctr++)
	{
        if(ctr < delimeterpos)
		{
			field += line[ctr];
		}

        if(ctr > delimeterpos+1)
		{
			value += line[ctr];
		}
        if(ctr == delimeterpos)
		{
			continue;
		}
	}
}

void AbnfMessage::decodeMessageIdentificationLine(const char* requestLine)
{
	m_Request.erase();
	m_URL.erase();
	m_Version.erase();
	m_Protocol.erase();
	m_ResponseCode = -1;
	m_ResponseText.erase();
	m_MessageType = REQUEST;

	int ws = 0;
	std::string token1, token2, token3;

	for(int index = 0; requestLine[index] != 0 ; index++)
	{
		if(requestLine[index] == ' ' || requestLine[index] == '\t')
		{
			ws++;
			continue;
		}
		if(ws > 2)
		{
			break;
		}

		if(ws == 0)
		{
			token1 += requestLine[index];
		}
		if(ws == 1)
		{
			token2 += requestLine[index];
		}
		if(ws == 2)
		{
			token3 += requestLine[index];
		}
	}

    if(characterposition(token1.c_str(),'/') == -1)
	{
		m_Request = token1;
		m_URL = token2;
        split(token3.c_str(),'/',m_Protocol,m_Version);
		m_MessageType = REQUEST;
		return;
	}
	else
	{
        split(token1.c_str(),'/',m_Protocol,m_Version);
		m_ResponseCode = atoi(token2.c_str());
		m_ResponseText = token3;
		m_MessageType = RESPONSE;
		return;
	}
}

void AbnfMessage::encodeMessageIdentificationLine()
{
	char tempBuffer[1024];
	memset(tempBuffer,0,1024);
	if(m_MessageType == RESPONSE)
	{
		sprintf(tempBuffer,"%s/%s %d %s\r\n",m_Protocol.c_str(),m_Version.c_str(),m_ResponseCode,m_ResponseText.c_str());
	}
	else
	{
		sprintf(tempBuffer,"%s %s %s/%s\r\n",m_Request.c_str(),m_URL.c_str(),m_Protocol.c_str(),m_Version.c_str());
	}
	m_MessageLine = tempBuffer;
}

void AbnfMessage::setProtocolInformation(const char* request, const char* URL, const char* protocol, const char* version)
{
	m_MessageType = REQUEST;
    keyList.clear();
    valueList.clear();
	m_Protocol = protocol;
	m_Version = version;
	m_Request = request;
	m_URL = URL;
}

void AbnfMessage::setProtocolInformation(const char* protocol, const char* version, long responsecode, const char* responsetext)
{
	m_MessageType = RESPONSE;
    keyList.clear();
    valueList.clear();
    m_Protocol = protocol;
	m_Version = version;
	m_ResponseCode = responsecode;
	m_ResponseText = responsetext;
}

void AbnfMessage::addHeader(const char* field, const char* value)
{
    keyList.push_back(field);
    valueList.push_back(value);
}

void AbnfMessage::serialize(std::string &sipString)
{
	encodeMessageIdentificationLine();

    char buffer[8096]={0};
    char temp[1025]={0};

	strcpy(buffer,m_MessageLine.c_str());

    int headercount = keyList.size();

    for(int index = 0; index < headercount; index++)
    {
        sprintf(temp,"%s: %s\r\n",((std::string)keyList[index]).c_str(),((std::string)valueList[index]).c_str());
        strcat(buffer,temp);
    }

    strcat(buffer,"\r\n\r\n");

	sipString = buffer;
}


