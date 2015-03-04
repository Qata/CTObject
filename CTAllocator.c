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

CTAllocatorRef CTAllocatorCreate()
{
	return calloc(1, sizeof(CTAllocator));
}

void CTAllocatorEmpty(CTAllocatorRef restrict allocator)
{
	for (int i = 0; i < allocator->count; ++i)
	{
		free(allocator->objects[i]);
	}
	allocator->count = 0;
}

void CTAllocatorRelease(CTAllocatorRef restrict allocator)
{
	CTAllocatorEmpty(allocator);
	free(allocator->objects);
	free(allocator);
}

void * CTAllocatorAllocate(CTAllocatorRef restrict allocator, uint64_t size)
{
	assert(allocator);
	uint64_t index = allocator->count++;
	if (index >= allocator->size)
	{
		allocator->size = kArrayGrowthFactor * allocator->count;
		assert(allocator->objects = realloc(allocator->objects, sizeof(void *) * allocator->size));
	}
	return allocator->objects[index] = calloc(1, size);
}

void CTAllocatorDeallocate(CTAllocatorRef restrict allocator, void * ptr)
{
	assert(allocator);
	if (allocator->count > 0)
	{
		for (uint64_t i = 0; i < allocator->count; ++i)
		{
			if (allocator->objects[i] == ptr)
			{
				free(allocator->objects[i]);
				memmove(allocator->objects + i, allocator->objects + i + 1, sizeof(void *) * (--allocator->count - i));
				break;
			}
		}
	}
}

void * CTAllocatorReallocate(CTAllocatorRef restrict allocator, void * ptr, uint64_t size)
{
	assert(allocator);
	if (ptr)
	{
		for (uint64_t i = 0; i < allocator->count; ++i)
		{
			if (allocator->objects[i] == ptr)
			{
				return allocator->objects[i] = realloc(ptr, size);
			}
		}
	}
	return CTAllocatorAllocate(allocator, size);
}