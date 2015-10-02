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
#include "CTError.h"
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
CTArrayRef CTArrayCopy(CTAllocatorRef restrict alloc, const CTArray * restrict array);

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
uint8_t CTArrayCompare(const CTArray * array1, const CTArray * array2);

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
CTObjectRef CTArrayEntry(const CTArray * restrict array, uint64_t index);

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
CTObjectRef CTArrayObjectAtIndex(const CTArray * restrict array, uint64_t index);

/**
 * Return the index of the CTObjectRef in the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param value	The CTObject pointer to search for in the array.
 * @return		The index of the CTObject, if not found, NULL is returned.
 **/
uint64_t CTArrayIndexOfEntryByReference(const CTArray * restrict array, CTObjectRef value);

/**
 * Return the index of the CTObjectRef in the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param value	The CTObject to search for in the array.
 * @return		The index of the CTObject, if not found, NULL is returned.
 **/
uint64_t CTArrayIndexOfEntryByValue(const CTArray * restrict array, CTObjectRef value);

/**
 * Return the count of the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @return		The count of the CTArray. The result is identical to using array->count.
 **/
uint64_t CTArrayCount(const CTArray * restrict array);

/**
 * Apply a function to every element of the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param mapFn	A function to apply to every CTObject in the array.
 * @return		An eldritch void.
 **/
void CTArrayMapMutate(CTArray * restrict array, void (^mapFn)(CTObject * object));

/**
 * Copy the array and apply a function to every element then return the new array.
 * @param alloc	A properly initialised CTAllocator that was created with CTAllocatorCreate.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param mapFn	A function to apply to every CTObject in the array.
 * @return		A new CTArrayRef
 **/
CTArrayRef CTArrayMap(CTAllocatorRef alloc, const CTArray * restrict array, CTObject * (^mapFn)(CTObject * object));

/**
 * Remove all objects in the array that the filter block doesn't return true for.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param filterFn	A function to apply to every CTObject in the array.
 * @return		An eldritch void.
 **/
void CTArrayFilterMutate(CTArrayRef restrict array, uint8_t (^filterFn)(CTObject * object));

/**
 * Copy all the elements from the array for which the filter block returns true and return the new array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param filterFn	A function to apply to every CTObject in the array.
 * @return		A new CTArrayRef
 **/
CTArrayRef CTArrayFilter(CTAllocatorRef alloc, const CTArray * restrict array, uint8_t (^filterFn)(CTObject * object));

/**
 * Apply a comparison function to every element of the array and return true if they all pass.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param cmpFn	A function to apply to every CTObject in the array.
 * @return		An 8-bit boolean.
 **/
uint8_t CTArrayAll(const CTArray * restrict array, uint8_t (^cmpFn)(const CTObject * object));

/**
 * Apply a comparison function to every element of the array and return true if they all pass.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param error	A CTErrorRef pointer that is passed into the comparison function.
 * @param cmpFn	A function to apply to every CTObject in the array.
 * @return		An 8-bit boolean.
 **/
uint8_t CTArrayAllError(const CTArray * restrict array, CTErrorRef * error, uint8_t (^cmpFn)(const CTObject * object, CTErrorRef * error));

/**
 * Apply a comparison function to every element of the array and return true if any of them pass.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param cmpFn	A function to apply to every CTObject in the array.
 * @return		An 8-bit boolean.
 **/
uint8_t CTArrayAny(const CTArray * restrict array, uint8_t (^cmpFn)(const CTObject * object));

/**
 * Apply a function to every element of the array.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param eachFn	A function to apply to every CTObject in the array.
 **/
void CTArrayEach(const CTArray * restrict array, void (^eachFn)(CTObject * object));

/**
 * Apply a function to every element of the array in reverse.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param eachFn	A function to apply to every CTObject in the array.
 **/
void CTArrayReverseEach(const CTArray * restrict array, void (^eachFn)(CTObject * object));

/**
 * Create a subset of a CTArray starting at a given index.
  * @param alloc	A properly initialised CTAllocator that was created with CTAllocatorCreate.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param index	The index that the new array should start from.
 * @return		An 8-bit boolean.
 **/
const CTArray * CTArraySubsetFromIndex(CTAllocatorRef alloc, const CTArray * array, uint64_t index);

/**
 * Fold the array into a single value.
 * @param start	A properly initialised CTObject.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @param redFn	A function to apply to every CTObject in the array.
 **/
CTObject * CTArrayReduce(CTObject * start, const CTArray * array, CTObject * (^redFn)(CTObject * accumulator, const CTObject * object));

/**
 * Return a CTObject encasing the CTArray passed.
 * @param array	A properly initialised CTArray that was created with CTArrayCreate*.
 * @return		The CTArray wrapped in a CTObject. The result is identical to using CTObjectCreate.
 **/
CTObjectRef CTObjectWithArray(CTAllocatorRef alloc, CTArray * restrict array);