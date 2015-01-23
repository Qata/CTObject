//
//  CTAllocator.c
//  CTObject
//
//  Created by Carlo Tortorella on 21/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include "CTAllocator.h"

static CTAllocatorRef defaultAllocator;

void CTAllocatorReleasePrivate(CTAllocatorRef restrict allocator)
{
	for (int i = 0; i < allocator->count; ++i)
	{
		free(allocator->objects[i]);
	}
    free(allocator->objects);
	free(allocator);
}

void CTAllocatorReleaseDefaultAllocator()
{
	CTAllocatorReleasePrivate(defaultAllocator);
}

pthread_mutex_t default_lock;

void CTAllocatorInit()
{
	static volatile int initialized = 0;
	if (!initialized)
	{
		pthread_mutex_init(&default_lock, NULL);
		defaultAllocator = CTAllocatorCreate();
		atexit(CTAllocatorReleaseDefaultAllocator);
		initialized = 1;
	}
}

CTAllocatorRef CTAllocatorGetDefault()
{
	return defaultAllocator;
}

CTAllocatorRef CTAllocatorCreate()
{
	return calloc(1, sizeof(CTAllocator));
}

void CTAllocatorRelease(CTAllocatorRef restrict allocator)
{
	assert(allocator);
	if (allocator != CTAllocatorGetDefault())
	{
		CTAllocatorReleasePrivate(allocator);
	}
	else
	{
		fputs("Don't touch the default allocator\n", stderr);
	}
}

void * CTAllocatorAllocate(CTAllocatorRef restrict allocator, uint64_t size)
{
	assert(allocator);
	if (allocator == defaultAllocator)
	{
		assert(!pthread_mutex_lock(&default_lock));
	}
	
	if (!(allocator->objects = realloc(allocator->objects, sizeof(void *) * allocator->count + 1)))
	{
		abort();
	}
	
	void * retVal = allocator->objects[allocator->count++] = calloc(1, size);
	if (allocator == defaultAllocator)
	{
		pthread_mutex_unlock(&default_lock);
	}
	
	return retVal;
}

void CTAllocatorDeallocate(CTAllocatorRef restrict allocator, void * ptr)
{
	assert(allocator);
	if (allocator == defaultAllocator)
	{
		assert(!pthread_mutex_lock(&default_lock));
	}
    if (allocator->count > 0)
	{
		int countOfKeys = 0;
		for (unsigned long i = 0; i < allocator->count; ++i)
		{
			if (allocator->objects[i] == ptr)
			{
				++countOfKeys;
			}
		}
		
		if (countOfKeys)
		{
			if (allocator->count - countOfKeys)
			{
				void ** objects = calloc(sizeof(void *), allocator->count - countOfKeys);
				for (unsigned long i = 0, count = 0; i < allocator->count; ++i)
				{
					if (allocator->objects[i] != ptr)
					{
						objects[count++] = allocator->objects[i];
					}
					else
					{
						free(allocator->objects[i]);
					}
				}
				free(allocator->objects);
				allocator->objects = objects;
				allocator->count = allocator->count - countOfKeys;
			}
			else
			{
				for (unsigned long i = 0; i < allocator->count; ++i)
				{
					free(allocator->objects[i]);
				}
				free(allocator->objects);
				allocator->count = 0;
			}
        }
	}
	if (allocator == defaultAllocator)
	{
		assert(!pthread_mutex_unlock(&default_lock));
	}
}

void * CTAllocatorReallocate(CTAllocatorRef restrict allocator, void * ptr, uint64_t size)
{
	assert(allocator);
    if (ptr)
	{
		if (allocator == defaultAllocator)
		{
			pthread_mutex_lock(&default_lock);
		}
		void * retVal = NULL;
        for (uint64_t i = 0; i < allocator->count; ++i)
        {
            if (allocator->objects[i] == ptr)
            {
                retVal = allocator->objects[i] = realloc(ptr, size);
				break;
            }
		}
		if (allocator == defaultAllocator)
		{
			pthread_mutex_unlock(&default_lock);
		}
        return retVal;
    }
    return CTAllocatorAllocate(allocator, size);
}

void CTAllocatorTransferOwnership(CTAllocatorRef restrict allocator, CTAllocatorRef restrict dest, void * ptr)
{
	assert(allocator);
	assert(dest);
	assert(dest != allocator);
	if (allocator == defaultAllocator || dest == defaultAllocator)
	{
		assert(!pthread_mutex_lock(&default_lock));
	}
	if (allocator->count)
	{
		int countOfKeys = 0;
		for (uint64_t i = 0; i < allocator->count; ++i)
		{
			if (allocator->objects[i] == ptr)
			{
				++countOfKeys;
			}
		}
		
		if (countOfKeys)
		{
			if (allocator->count - countOfKeys)
			{
				void ** objects = calloc(sizeof(void *), allocator->count - countOfKeys);
				for (uint64_t i = 0, count = 0; i < allocator->count; ++i)
				{
					if (allocator->objects[i] != ptr)
					{
						objects[count++] = allocator->objects[i];
					}
					else
					{
						if ((dest->objects = realloc(dest->objects, sizeof(void *) * dest->count + 1)))
						{
							dest->objects[dest->count++] = allocator->objects[i];
						}
					}
				}
				free(allocator->objects);
				allocator->objects = objects;
				allocator->count = allocator->count - countOfKeys;
			}
			else
			{
				assert(dest->objects = realloc(dest->objects, sizeof(void *) * dest->count + allocator->count));
				for (uint64_t i = 0; i < allocator->count; ++i)
				{
					dest->objects[dest->count++] = allocator->objects[i];
				}
				free(allocator->objects);
				allocator->objects = NULL;
				allocator->count = 0;
			}
		}
	}
	if (allocator == defaultAllocator || dest == defaultAllocator)
	{
		assert(!pthread_mutex_unlock(&default_lock));
	}
}