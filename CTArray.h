//
//  CTArray.h
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include "CTObject.h"
/**
 * An object that keeps an array of type independent, dynamically allocated elements.
 **/
typedef struct
{
    CTAllocatorRef alloc;
    uint64_t count;
	uint64_t size;
    CTObjectRef* elements;
} CTArray, * CTArrayRef;

/**
 * Create an array with the specified allocator.
 * @param alloc	A properly initialised CTAllocator that was created with CTAllocatorCreate.
 * @return		Returns an initialised CTArray with a count of zero.
 **/
CTArrayRef CTArrayCreate(CTAllocatorRef restrict alloc);

/**
 * Copy the specified array and everything within using the supplied allocator.
 * @param alloc	A properly initialised CTAllocator that was created with CTAllocatorCreate.
 * @return		Returns an initialised CTArray with the same count as the array supplied. All objects in the array will also be allocated with the supplied allocator.
 **/
CTArrayRef CTArrayCopy(CTAllocatorRef restrict alloc, CTArrayRef array);

/**
 * Create an array with the specified allocator and initialise it with a NULL-terminated list of CTObjects.
 * @param alloc	A properly initialised CTAllocator that was created with CTAllocatorCreate.
 * @return		Returns an initialised CTArray with a count of the amount of passed CTObjects.
 **/
CTArrayRef CTArrayCreateWithObjects(CTAllocatorRef restrict alloc, ...);

/**
 * Release an allocated array and all objects within.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @return		A dark void, filled with eldritch creatures, the sight of which would cause any human to lose all connections to reality.
 **/
void CTArrayRelease(CTArrayRef restrict array);

/**
 * Compare two CTArray objects
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @return		A value indicating equality, 0 = false, 1 = true.
 **/
uint8_t CTArrayCompare(CTArrayRef array1, CTArrayRef array2);

/**
 * Add a CTObject to the end of the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param value	A void pointer to be used in the creation of a CTObject.
 * @param type	An 8 bit integer used to identify the type of the object being created.
 * @return		A dark void, filled with eldritch creatures, the sight of which would cause any human to lose all connections to reality.
 * @see CTObjectCreate
 **/
void CTArrayAddEntry(CTArrayRef restrict array, void * value, int8_t type);

/**
 * Add a CTObject to the end of the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param value	A properly initialised CTObject that was created with CTObjectCreate.
 * @return		A dark void, filled with eldritch creatures, the sight of which would cause any human to lose all connections to reality.
 * @see CTObjectCreate
 **/
void CTArrayAddEntry2(CTArrayRef restrict array, CTObjectRef restrict value);

/**
 * Return the CTObject at the specified index.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @return		A CTObject stored at the specified index, or NULL if the index oversteps the array size.
 **/
CTObjectRef CTArrayEntry(const CTArrayRef restrict array, uint64_t index);

/**
 * Delete the CTObject at the specified index.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param index	The index of the object to be deleted.
 * @return		A dark void, filled with eldritch creatures, the sight of which would cause any human to lose all connections to reality.
 **/
void CTArrayDeleteEntry(CTArrayRef restrict array, uint64_t index);

/**
 * Delete every CTObject in the specified array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @return		A dark void, filled with eldritch creatures, the sight of which would cause any human to lose all connections to reality.
 **/
void CTArrayEmpty(CTArrayRef restrict array);

/**
 * Return the CTObject at the specified index.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param index	The index of the object to be returned.
 * @return		The CTObject to be found at the specified index, if the index is beyond the bounds of the array, NULL is returned.
 **/
CTObjectRef CTArrayObjectAtIndex(CTArrayRef restrict array, uint64_t index);

/**
 * Return the index of the CTObjectRef in the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param value	The CTObject pointer to search for in the array.
 * @return		The index of the CTObject, if not found, NULL is returned.
 **/
uint64_t CTArrayIndexOfEntryByReference(CTArrayRef restrict array, CTObjectRef value);

/**
 * Return the index of the CTObjectRef in the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param value	The CTObject to search for in the array.
 * @return		The index of the CTObject, if not found, NULL is returned.
 **/
uint64_t CTArrayIndexOfEntryByValue(CTArrayRef restrict array, CTObjectRef value);

/**
 * Return the count of the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @return		The count of the CTArray. The result is identical to using array->count.
 **/
uint64_t CTArrayCount(CTArrayRef restrict array);

/**
 * Return a CTObject encasing the CTArray passed.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @return		The CTArray wrapped in a CTObject. The result is identical to using CTObjectCreate.
 **/
CTObjectRef CTObjectWithArray(CTAllocatorRef alloc, CTArrayRef restrict array);