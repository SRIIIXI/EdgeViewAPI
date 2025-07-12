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

typedef std::vector<std::string> string_list;
typedef std::map<std::string, std::string> key_value_list;

#endif
