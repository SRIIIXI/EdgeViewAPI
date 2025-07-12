#include "ResourceHandler.hpp"

char textExtensions[] = {".txt.xml.html.dhtml.htm.php.log.wsdl.xsl.xsd.js.css"};
char imageExtensions[] = {".jpg.jpeg.gif.png.bmp.pic.dib.ico"};
char audioExtensions[] = {".wma.wav.mp3.aiff.snd.mp4.vox"};
char videoExtensions[] = {".wmv.avi.mpg.mpeg.dat.mov.3gp"};
char binaryExtensions[] = {".bin.sys.exe.so.dll.lib"};

ResourceHandler::ResourceHandler()
{
	fileDescriptor = NULL;
    urlContent = NULL;
}

ResourceHandler::~ResourceHandler()
{
	if(fileDescriptor != NULL)
	{
		fclose(fileDescriptor);
		fileDescriptor = NULL;
	}

    if(urlContent != NULL)
    {
        delete urlContent;
        urlContent = NULL;
    }
}

bool ResourceHandler::loadContent(const char* url)
{
    if(urlContent != NULL)
    {
        delete urlContent;
        urlContent = NULL;
        contentLength = 0;
    }

	 resolvedURL = url;

    if(strcmp(resolvedURL.c_str(),"/")==0)
	{
        resolvedURL = resolvedURL + rootDocument;
	}

    // All parameters passed to the URL must be discared here
    // Parameterized URL must be processed by custom handlers alone

    int pos = StringHandler::characterposition(url,'?');

    if(pos >= 0)
    {
        resolvedURL[pos]='\0';
    }

	resolvedFileName = strServerRoot + resolvedURL;
	fileDescriptor = fopen(resolvedFileName.c_str(),"rb");

	if(fileDescriptor == NULL)
	{
        return false;
	}

	fseek(fileDescriptor,0,SEEK_END);
	contentLength = ftell(fileDescriptor);
	rewind(fileDescriptor);

    urlContent = new char[contentLength];
    memset(urlContent, 0 , contentLength);

	fseek(fileDescriptor,0,SEEK_SET);
	fread(urlContent,contentLength,1,fileDescriptor);

	fclose(fileDescriptor);

	fileDescriptor = NULL;

	String ext;

    DirectoryHandler::getExtension(resolvedURL.c_str(), ext);
    StringHandler::replace(ext, '.', ' ');
    StringHandler::alltrim(ext);

    contentype = Binary;

    char contentTag[32] = {0};
    if(StringHandler::substringposition(textExtensions,ext.c_str())>-1)
	{
        sprintf(contentTag, "text/%s%c", ext.c_str(), '\0');
        contentype = Text;
	}

    if(StringHandler::substringposition(imageExtensions,ext.c_str())>-1)
	{
        sprintf(contentTag, "image/%s%c", ext.c_str(), '\0');
        contentype = Image;
	}

    if(StringHandler::substringposition(audioExtensions,ext.c_str())>-1)
	{
        sprintf(contentTag, "audio/%s%c", ext.c_str(), '\0');
        contentype = Audio;
	}

    if(StringHandler::substringposition(videoExtensions,ext.c_str())>-1)
	{
        sprintf(contentTag, "video/%s%c", ext.c_str(), '\0');
        contentype = Video;
	}

    if(StringHandler::substringposition(binaryExtensions,ext.c_str())>-1)
	{
        sprintf(contentTag, "application/octet-stream%c", '\0');
        contentype = Binary;
	}

    contentTypeTag = contentTag;

    return true;
}

void ResourceHandler::setServerRoot(const char*  serverRoot)
{
	strServerRoot = serverRoot;
}

void ResourceHandler::setRootDocument(const char *rootDoc)
{
    rootDocument = rootDoc;
}

void ResourceHandler::setServerAddress(const char* serveraddr)
{
    serverAddress = serveraddr;
}

void ApplicationCallback::setServerRoot(const char*  serverRoot)
{
    strServerRoot = serverRoot;
}

void ApplicationCallback::setServerAddress(const char* serveraddr)
{
   serverAddress = serveraddr;
}

