//
//  CTAllocator.h
//  CTObject
//
//  Created by Carlo Tortorella on 21/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include <stdint.h>

/**
 * An object that acts as a front to malloc, realloc and free in order to keep track of allocated memory.
 **/
typedef struct
{
	uint64_t count;
	void ** objects;
} CTAllocator;

/**
 * Get the CTAllocator used by the entire program for allocation and destruction of smaller objects.
 * @return	Returns the program's CTAllocator that can be used to allocate and deallocate memory.
 **/
CTAllocator * CTAllocatorGetDefault();

/**
 * Create a CTAllocator to use in allocation and destruction of memory.
 * @return	Returns an initialised CTAllocator that can be used to allocate and deallocate memory.
 **/
CTAllocator * CTAllocatorCreate();
/**
 * Deallocate all memory associated with a specified CTAllocator, including that used by the allocator itself.
 * @param allocator	A properly initialised CTAllocator that was created with CTAllocatorCreate.
 **/
void CTAllocatorRelease(CTAllocator * restrict allocator);
/**
 * Allocate memory and add a pointer to it to the objects array within the specified CTAllocator, to be used when CTAllocatorRelease is called.
 * @return	Returns a block of memory created with malloc.
 **/
void * CTAllocatorAllocate(CTAllocator * restrict allocator, unsigned long size);
/**
 * Reallocate memory to be the specified size if a pointer to it exists in the objects array.
 * @param allocator	A properly initialised CTAllocator that was created with CTAllocatorCreate.
 * @param ptr		A pointer to the chunk of memory to resize.
 * @param size		The size to reallocate the block as.
 * @return			Returns a block of memory created with realloc.
 **/
void * CTAllocatorReallocate(CTAllocator * restrict allocator, void * ptr, unsigned long size);
/**
 * Deallocate memory at the given address and remove it from the objects array.
 * @param allocator	A properly initialised CTAllocator that was created with CTAllocatorCreate.
 * @param ptr		A pointer to the chunk of memory to deallocate.
 * @return			A dark void, filled with eldritch creatures, the sight of which would cause any human to lose all connections to reality.
 **/
void CTAllocatorDeallocate(CTAllocator * restrict allocator, void * ptr);