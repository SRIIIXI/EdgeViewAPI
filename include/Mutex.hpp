#ifndef _MUTEX
#define _MUTEX

#include "Defines.hpp"

#define MUTEX pthread_mutex_t

extern "C"
{

class Mutex
{
public:
	Mutex();
	~Mutex();
	bool create();
	bool destroy();
	bool lock();
	bool tryLock();
	bool unlock();
private:
	MUTEX mutex;
	bool created;
};

}

#endif
