// ABNFMessage.cpp: implementation of the ABNFMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "AbnfMessage.hpp"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AbnfMessage::AbnfMessage()
{
	request_buffer.erase();
    http_content = NULL;
	has_content = false;
	request.erase();
	http_url.erase();
	protocol.erase();
	version.erase();
	response_text.erase();
	response_code = -1;
	message_type = -1;
    keyList.clear();
    valueList.clear();
    content_size = 0;
}

AbnfMessage::AbnfMessage(const char* buffer)
{
	request_buffer.erase();
    http_content = NULL;
    has_content = false;
	request.erase();
	http_url.erase();
	protocol.erase();
	version.erase();
	response_text.erase();
	response_code = -1;
	message_type = -1;
	request_buffer = buffer;
    keyList.clear();
    valueList.clear();
    content_size = 0;
}

void AbnfMessage::reset()
{
}

AbnfMessage::~AbnfMessage()
{

    if(	http_content != NULL)
    {
        delete http_content;
        http_content = NULL;
    }

    request_buffer.erase();
    has_content = false;
    request.erase();
    http_url.erase();
    protocol.erase();
    version.erase();
    response_text.erase();
    response_code = -1;
    message_type = -1;
    keyList.clear();
    valueList.clear();
    content_size = 0;
}

void AbnfMessage::setHeader(const char* buffer)
{
	reset();
	request_buffer = buffer;
}

void AbnfMessage::attachBody(const char* buffer)
{
    if(	http_content != NULL)
    {
        delete http_content;
        http_content = NULL;
    }

    http_content = new char[content_size];
    memcpy(http_content,buffer,content_size);
}

const char*	AbnfMessage::getRequest()
{
	return request.c_str();
}

const char*	AbnfMessage::getProtocol()
{
	return protocol.c_str();
}

const char*	AbnfMessage::getURL()
{
	return http_url.c_str();
}

const char*	AbnfMessage::getVersion()
{
	return version.c_str();
}

const char*	AbnfMessage::getResponseText()
{
	return response_text.c_str();
}

long AbnfMessage::getResponseCode()
{
	return response_code;
}

long AbnfMessage::getMessageType()
{
	return message_type;
}

const char*	AbnfMessage::getContent()
{
    return http_content;
}

int AbnfMessage::getContentSize()
{
    return content_size;
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

	getLine(message_line);
	decodeMessageIdentificationLine(message_line.c_str());

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
				has_content = true;
                content_size = atoi(value.c_str());
			}
			else
			{
				has_content = false;
			}
			break;;
		}
	}
	return true;
}

bool AbnfMessage::hasBody()
{
	return has_content;
}

void AbnfMessage::getLine(std::string &line)
{
    if(request_buffer.length()<1)
    {
        line.clear();
        return;
    }

	std::string next;
    int pos = split(request_buffer.c_str(),"\r\n",line,next);

    if(pos == -1)
    {
        line = request_buffer;
        request_buffer.clear();
        return;
    }

	request_buffer = next;
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
	request.erase();
	http_url.erase();
	version.erase();
	protocol.erase();
	response_code = -1;
	response_text.erase();
	message_type = REQUEST;

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
		request = token1;
		http_url = token2;
        split(token3.c_str(),'/',protocol,version);
		message_type = REQUEST;
		return;
	}
	else
	{
        split(token1.c_str(),'/',protocol,version);
		response_code = atoi(token2.c_str());
		response_text = token3;
		message_type = RESPONSE;
		return;
	}
}

void AbnfMessage::encodeMessageIdentificationLine()
{
	char tempBuffer[1024];
	memset(tempBuffer,0,1024);
	if(message_type == RESPONSE)
	{
		sprintf(tempBuffer,"%s/%s %d %s\r\n",protocol.c_str(),version.c_str(),response_code,response_text.c_str());
	}
	else
	{
		sprintf(tempBuffer,"%s %s %s/%s\r\n",request.c_str(),http_url.c_str(),protocol.c_str(),version.c_str());
	}
	message_line = tempBuffer;
}

void AbnfMessage::setProtocolInformation(const char* request, const char* URL, const char* protocol, const char* version)
{
	message_type = REQUEST;
    keyList.clear();
    valueList.clear();
	protocol = protocol;
	version = version;
	request = request;
	http_url = URL;
}

void AbnfMessage::setProtocolInformation(const char* protocol, const char* version, long responsecode, const char* responsetext)
{
	message_type = RESPONSE;
    keyList.clear();
    valueList.clear();
    protocol = protocol;
	version = version;
	response_code = responsecode;
	response_text = responsetext;
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

	strcpy(buffer,message_line.c_str());

    int headercount = keyList.size();

    for(int index = 0; index < headercount; index++)
    {
        sprintf(temp,"%s: %s\r\n",((std::string)keyList[index]).c_str(),((std::string)valueList[index]).c_str());
        strcat(buffer,temp);
    }

    strcat(buffer,"\r\n\r\n");

	sipString = buffer;
}


