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
#include "CTAllocator.h"

static CTAllocator * defaultAllocator;

void CTAllocatorReleasePrivate(CTAllocator * restrict allocator)
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

CTAllocator * CTAllocatorGetDefault()
{
	static int initialized = 0;
	
	if (!initialized)
	{
		defaultAllocator = CTAllocatorCreate();
		atexit(CTAllocatorReleaseDefaultAllocator);
		initialized = 1;
	}
	
	return defaultAllocator;
}

CTAllocator * CTAllocatorCreate()
{
	return calloc(1, sizeof(CTAllocator));
}

void CTAllocatorRelease(CTAllocator * restrict allocator)
{
	if (allocator != CTAllocatorGetDefault() && allocator)
	{
		CTAllocatorReleasePrivate(allocator);
	}
	else
	{
		fputs("Don't touch the default allocator\n", stderr);
	}
}

void * CTAllocatorAllocate(CTAllocator * restrict allocator, uint64_t size)
{
	allocator = allocator ? allocator : CTAllocatorGetDefault();
	if (!(allocator->objects = realloc(allocator->objects, sizeof(void *) * allocator->count + 1)))
	{
		return NULL;
	}
	return allocator->objects[allocator->count++] = calloc(1, size);
}

void CTAllocatorDeallocate(CTAllocator * restrict allocator, void * ptr)
{
	allocator = allocator ? allocator : CTAllocatorGetDefault();
    if (allocator->count)
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
            void ** objects = malloc(sizeof(void *) * allocator->count - countOfKeys);
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
	}
}

void * CTAllocatorReallocate(CTAllocator * restrict allocator, void * ptr, uint64_t size)
{
	allocator = !allocator ? CTAllocatorGetDefault() : allocator;
    if (ptr)
    {
        for (uint64_t i = 0; i < allocator->count; ++i)
        {
            if (allocator->objects[i] == ptr)
            {
                return allocator->objects[i] = realloc(ptr, size);
            }
        }
        return NULL;
    }
    return CTAllocatorAllocate(allocator, size);
}

void CTAllocatorTransferOwnership(CTAllocator * restrict allocator, CTAllocator * restrict dest, void * ptr)
{
	allocator = allocator ? allocator : CTAllocatorGetDefault();
	if (dest && dest != allocator)
	{
		if (allocator->count)
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
				void ** objects = malloc(sizeof(void *) * allocator->count - countOfKeys);
				for (unsigned long i = 0, count = 0; i < allocator->count; ++i)
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
		}
	}
}