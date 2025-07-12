#ifndef _MUTEX
#define _MUTEX

#include "Defines.hpp"

extern "C"
{

	typedef struct MutexReference;

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
		MutexReference* mutex_reference;
		bool created;
	};

}

#endif
