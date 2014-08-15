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
 * An object that keeps an array of type independent elements.
 **/
typedef struct
{
    CTAllocator * alloc;
    uint64_t count;
    CTObject ** elements;
} CTArray;

/**
 * Create an array with the specified allocator.
 * @param alloc	A properly initialised CTAllocator that was created with CTAllocatorCreate.
 * @return		Returns an initialised CTArray with a count of zero.
 **/
CTArray * CTArrayCreate(CTAllocator * restrict alloc);

/**
 * Create an array with the specified allocator and initialise it with a NULL-terminated list of CTObjects.
 * @param alloc	A properly initialised CTAllocator that was created with CTAllocatorCreate.
 * @return		Returns an initialised CTArray with a count of the amount of passed CTObjects.
 **/
CTArray * CTArrayCreateWithObjects(CTAllocator * restrict alloc, ...);

/**
 * Release an allocated array and all objects within.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @return		A dark void, filled with eldritch creatures, the sight of which would cause any human to lose all connections to reality.
 **/
void CTArrayRelease(CTArray * restrict array);

/**
 * Compare two CTArray objects
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @return		A value indicating equality, 0 = false, 1 = true.
 **/
uint8_t CTArrayCompare(CTArray * array1, CTArray * array2);

/**
 * Add a CTObject to the end of the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param value	A void pointer to be used in the creation of a CTObject.
 * @param type	An 8 bit integer used to identify the type of the object being created.
 * @return		A dark void, filled with eldritch creatures, the sight of which would cause any human to lose all connections to reality.
 * @see CTObjectCreate
 **/
void CTArrayAddEntry(CTArray * restrict array, void * value, int8_t type);

/**
 * Add a CTObject to the end of the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param value	A properly initialised CTObject that was created with CTObjectCreate.
 * @return		A dark void, filled with eldritch creatures, the sight of which would cause any human to lose all connections to reality.
 * @see CTObjectCreate
 **/
void CTArrayAddEntry2(CTArray * restrict array, CTObject * restrict value);

/**
 * Return the CTObject at the specified index.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @return		A CTObject stored at the specified index, or NULL if the index oversteps the array size.
 **/
CTObject * CTArrayEntry(const CTArray * restrict array, uint64_t index);

/**
 * Delete the CTObject at the specified index.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param index	The index of the object to be deleted.
 * @return		A dark void, filled with eldritch creatures, the sight of which would cause any human to lose all connections to reality.
 **/
void CTArrayDeleteEntry(CTArray * restrict array, uint64_t index);

/**
 * Delete every CTObject in the specified array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @return		A dark void, filled with eldritch creatures, the sight of which would cause any human to lose all connections to reality.
 **/
void CTArrayEmpty(CTArray * restrict array);

/**
 * Return the CTObject at the specified index.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param index	The index of the object to be returned.
 * @return		The CTObject to be found at the specified index, if the index is beyond the bounds of the array, NULL is returned.
 **/
CTObject * CTArrayObjectAtIndex(CTArray * restrict array, uint64_t index);

/**
 * Return the index of the CTObject * in the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param value	The CTObject pointer to search for in the array.
 * @return		The index of the CTObject, if not found, NULL is returned.
 **/
uint64_t CTArrayIndexOfEntryByReference(CTArray * restrict array, CTObject * value);

/**
 * Return the index of the CTObject * in the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param value	The CTObject to search for in the array.
 * @return		The index of the CTObject, if not found, NULL is returned.
 **/
uint64_t CTArrayIndexOfEntryByValue(CTArray * restrict array, CTObject * value);

/**
 * Return the count of the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @return		The count of the CTArray. The result is identical to using array->count.
 **/
uint64_t CTArrayCount(CTArray * restrict array);

/**
 * Return a CTObject encasing the CTArray passed.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @return		The CTArray wrapped in a CTObject. The result is identical to using CTObjectCreate.
 **/
CTObject * CTObjectWithArray(CTArray * restrict array);