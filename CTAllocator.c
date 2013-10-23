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

CTAllocator * CTAllocatorCreate()
{
	CTAllocator * allocator = malloc(sizeof(CTAllocator));
	allocator->count = 0;
	allocator->objects = NULL;
	return allocator;
}

void CTAllocatorRelease(CTAllocator * restrict allocator)
{
	for (int i = 0; i < allocator->count; i++)
	{
		free(allocator->objects[i]);
	}
    free(allocator->objects);
	free(allocator);
}

void * CTAllocatorAllocate(CTAllocator * restrict allocator, unsigned long size)
{
	if (!(allocator->objects = realloc(allocator->objects, sizeof(void *) * allocator->count + 1)))
	{
		return NULL;
	}
    void * ptr = malloc(size);
    memset(ptr, 0, size);
	return allocator->objects[allocator->count++] = ptr;
}

void CTAllocatorDeallocate(CTAllocator * restrict allocator, void * ptr)
{
    if (allocator->count)
	{
		int countOfKeys = 0;
		for (unsigned long i = 0; i < allocator->count; i++)
		{
			if (allocator->objects[i] == ptr)
			{
				++countOfKeys;
			}
		}
		if (countOfKeys)
		{
            void ** objects = malloc(sizeof(void *) * allocator->count - countOfKeys);
            for (unsigned long i = 0, count = 0; i < allocator->count; i++)
            {
                if (!(allocator->objects[i] == ptr))
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

void * CTAllocatorReallocate(CTAllocator * restrict allocator, void * ptr, unsigned long size)
{
    if (ptr)
    {
        int i;
        for (i = 0; i < allocator->count; i++)
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