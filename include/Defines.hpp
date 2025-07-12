#ifndef _OS_DEFINITIONS
#define _OS_DEFINITIONS

	// C headers
	#include <stdarg.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <time.h>
	#include <signal.h>
	#include <limits.h>
	#include <malloc.h>
	#include <math.h>

	// C++ Headers
	#include <iostream>
	#include <string>
	#include <cstring>
	#include <sstream>
	#include <vector>
	#include <map>
	#include <list>
	#include <cctype>
	#include <ctime>
	#include <ios>
	#include <fstream>
	#include <algorithm>
	#include <bitset>
	#include <deque>
	#include <filesystem>
	#include <chrono>

	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
		#undef UNICODE
		#include <WinSock2.h>
		#include <Ws2tcpip.h>
		#include <WinBase.h>
		#include <Windows.h>
		#include <process.h>
		#include <direct.h>
		#include <TlHelp32.h>
		#include <psapi.h>
	#else
		// OS Headers
		#include <unistd.h>
		#include <dirent.h>
		#include <errno.h>
		#include <sys/types.h>
		#include <sys/stat.h>
		#include <sys/ipc.h>
		#include <sys/msg.h>
		#include <sys/socket.h>
		#include <sys/ioctl.h>
		#include <netinet/in.h>
		#include <sys/time.h>
		#include <sys/wait.h>
		#include <sys/select.h>
		#include <netdb.h>
		#include <arpa/inet.h>
		#include <pthread.h>
		#include <fcntl.h>
		#include <pwd.h>
	#endif

	#if defined(_WIN32) || defined(WIN32)
		#define strtoull(str, endptr, base) _strtoui64(str, endptr, base)
		#define sleep(n) ::Sleep(n*1000)
		#define getpid()	_getpid()
		#define pid_t    long
		#define THREAD_PROC_RETURN_TYPE DWORD WINAPI
	#else
		#define THREAD_PROC_RETURN_TYPE *void
	#endif

#endif