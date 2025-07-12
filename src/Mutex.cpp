#include "Mutex.hpp"

typedef struct MutexReference
{
    #if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
         HANDLE mutex_object;
    #else
        pthread_mutex_t mutex_object;
    #endif
}MutexReference;

Mutex::Mutex()
{
    mutex_reference = nullptr;
	created = false;
}

Mutex::~Mutex()
{
}

bool Mutex::create()
{
    created = false;
    #if defined(WIN32) || defined(_WIN32)
        mutex_reference->mutex_object = ::CreateMutex(NULL, FALSE, NULL);

        if (mutex_reference->mutex_object != NULL)
        {
            created = true;
        }
    #else
        int rc = pthread_mutex_init(&mutex_reference->mutex_object, NULL);

        if (rc == 0)
        {
            return true;
        }
    #endif

    return created;
}

bool Mutex::lock()
{
	if(!created)
	{
		return false;
	}

    #if defined(WIN32) || defined(_WIN32)
        if (WaitForSingleObject(mutex_reference->mutex_object, INFINITE) == WAIT_OBJECT_0)
        {
            return true;
        }

    #else
        int rc = pthread_mutex_lock(&mutex_reference->mutex_object);
        if(rc == 0)
        {
            return true;
	    }
    #endif

	return false;
}

bool Mutex::tryLock()
{
	if(!created)
	{
		return false;
	}

    #if defined(WIN32) || defined(_WIN32)
        if (WaitForSingleObject(mutex_reference->mutex_object, 100) == WAIT_OBJECT_0)
        {
            return true;
        }
    #else
        int rc = pthread_mutex_trylock(&mutex_reference->mutex_object);
        if(rc == 0)
        {
            return true;
        }
    #endif

	return false;
}

bool Mutex::unlock()
{
	if(!created)
	{
		return false;
	}

    #if defined(WIN32) || defined(_WIN32)
        return (bool)ReleaseMutex(mutex_reference->mutex_object);
    #else
        int rc = pthread_mutex_unlock(&mutex_reference->mutex_object);
        if(rc == 0)
        {
            return true;
        }
    #endif

	return false;
}

bool Mutex::destroy()
{
	if(!created)
	{
		return false;
	}

    #if defined(WIN32) || defined(_WIN32)
        if (::CloseHandle(mutex_reference->mutex_object))
        {
            created = false;
            return true;
        }
        else
        {
            return false;
        }
    #else
        int rc = pthread_mutex_destroy(&mutex_reference->mutex_object);

        if(rc == 0)
        {
            created = false;
            return true;
        }
    #endif

	return false;
}
