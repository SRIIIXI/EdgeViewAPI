#include "ResourceHandler.hpp"

char textExtensions[] = {".txt.xml.html.dhtml.htm.php.log.wsdl.xsl.xsd.js.css"};
char imageExtensions[] = {".jpg.jpeg.gif.png.bmp.pic.dib.ico"};
char audioExtensions[] = {".wma.wav.mp3.aiff.snd.mp4.vox"};
char videoExtensions[] = {".wmv.avi.mpg.mpeg.dat.mov.3gp"};
char binaryExtensions[] = {".bin.sys.exe.so.dll.lib"};

ResourceHandler::ResourceHandler()
{
	file_descriptor = NULL;
    url_content = NULL;
}

ResourceHandler::~ResourceHandler()
{
	if(file_descriptor != NULL)
	{
		fclose(file_descriptor);
		file_descriptor = NULL;
	}

    if(url_content != NULL)
    {
        delete url_content;
        url_content = NULL;
    }
}

bool ResourceHandler::loadContent(const char* url)
{
    if(url_content != NULL)
    {
        delete url_content;
        url_content = NULL;
        content_length = 0;
    }

	 resolved_url = url;

    if(strcmp(resolved_url.c_str(),"/")==0)
	{
        resolved_url = resolved_url + root_document;
	}

    // All parameters passed to the URL must be discared here
    // Parameterized URL must be processed by custom handlers alone

    int pos = StringHandler::characterposition(url,'?');

    if(pos >= 0)
    {
        resolved_url[pos]='\0';
    }

	resolved_filename = server_root + resolved_url;
	file_descriptor = fopen(resolved_filename.c_str(),"rb");

	if(file_descriptor == NULL)
	{
        return false;
	}

	fseek(file_descriptor,0,SEEK_END);
	content_length = ftell(file_descriptor);
	rewind(file_descriptor);

    url_content = new char[content_length];
    memset(url_content, 0 , content_length);

	fseek(file_descriptor,0,SEEK_SET);
	fread(url_content,content_length,1,file_descriptor);

	fclose(file_descriptor);

	file_descriptor = NULL;

    std::string ext;

    DirectoryHandler::getExtension(resolved_url.c_str(), ext);
    StringHandler::replace(ext, '.', ' ');
    StringHandler::alltrim(ext);

    content_type = Binary;

    char contentTag[32] = {0};
    if(StringHandler::substringposition(textExtensions,ext.c_str())>-1)
	{
        sprintf(contentTag, "text/%s%c", ext.c_str(), '\0');
        content_type = Text;
	}

    if(StringHandler::substringposition(imageExtensions,ext.c_str())>-1)
	{
        sprintf(contentTag, "image/%s%c", ext.c_str(), '\0');
        content_type = Image;
	}

    if(StringHandler::substringposition(audioExtensions,ext.c_str())>-1)
	{
        sprintf(contentTag, "audio/%s%c", ext.c_str(), '\0');
        content_type = Audio;
	}

    if(StringHandler::substringposition(videoExtensions,ext.c_str())>-1)
	{
        sprintf(contentTag, "video/%s%c", ext.c_str(), '\0');
        content_type = Video;
	}

    if(StringHandler::substringposition(binaryExtensions,ext.c_str())>-1)
	{
        sprintf(contentTag, "application/octet-stream%c", '\0');
        content_type = Binary;
	}

    content_type_tag = contentTag;

    return true;
}

void ResourceHandler::setServerRoot(const char*  serverRoot)
{
	server_root = serverRoot;
}

void ResourceHandler::setRootDocument(const char *rootDoc)
{
    root_document = rootDoc;
}

void ResourceHandler::setServerAddress(const char* serveraddr)
{
    server_address = serveraddr;
}

void ApplicationCallback::setServerRoot(const char*  serverRoot)
{
    strServerRoot = serverRoot;
}

void ApplicationCallback::setServerAddress(const char* serveraddr)
{
   serverAddress = serveraddr;
}

