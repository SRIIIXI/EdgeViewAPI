#include "Mutex.hpp"

Mutex::Mutex()
{
	created = false;
}

Mutex::~Mutex()
{
}

bool Mutex::lock()
{
	if(!created)
	{
		return false;
	}

    int rc = pthread_mutex_lock(&mutex);
    if(rc == 0)
    {
        return true;
    }

	return false;
}

bool Mutex::tryLock()
{
	if(!created)
	{
		return false;
	}

    int rc = pthread_mutex_trylock(&mutex);
    if(rc == 0)
    {
        return true;
    }

	return false;
}

bool Mutex::unlock()
{
	if(!created)
	{
		return false;
	}

    int rc = pthread_mutex_unlock(&mutex);
    if(rc == 0)
    {
        return true;
    }

	return false;
}

bool Mutex::create()
{
    int rc = pthread_mutex_init(&mutex, NULL);

    if(rc == 0)
    {
        return true;
    }

	return false;
}

bool Mutex::destroy()
{
	if(!created)
	{
		return false;
	}

    int rc = pthread_mutex_destroy(&mutex);

    if(rc == 0)
    {
        created = false;
        return true;
    }

	return false;
}
