//
//  CTArray.c
//  CTObject
//
//  Created by Charlotte Tortorella on 22/10/13.
//  Copyright (c) 2013 Charlotte Tortorella. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>
#include "CTArray.h"

CTArrayRef CTArrayCreate(CTAllocatorRef restrict alloc)
{
    CTArrayRef array = CTAllocatorAllocate(alloc, sizeof(CTArray));
    array->alloc = alloc;
    return array;
}

CTArrayRef CTArrayCreateWithObjects(CTAllocatorRef restrict alloc, ...)
{
	CTArrayRef retVal = CTArrayCreate(alloc);
	va_list list;
	va_start(list, alloc);
	for (CTObjectRef value = va_arg(list, CTObject*); value != NULL; value = va_arg(list, CTObject*))
	{
		CTArrayAddEntry2(retVal, value);
	}
	va_end(list);
	return retVal;
}

CTArrayRef CTArrayCopy(CTAllocatorRef alloc, const CTArray * restrict array)
{
	CTArrayRef new_array = CTArrayCreate(alloc);
	CTArrayEach(array, ^(CTObject *object) {
		CTArrayAddEntry2(new_array, CTObjectCopy(alloc, object));
	});
	return new_array;
}

void CTArrayRelease(CTArrayRef restrict array)
{
	CTArrayEach(array, ^(CTObject *object) {
		CTObjectRelease(object);
	});
	CTAllocatorDeallocate(array->alloc, array->elements);
	CTAllocatorDeallocate(array->alloc, array);
}

uint8_t CTArrayCompare(const CTArray * array1, const CTArray * array2)
{
	return array1->count == array2->count
	&&
	CTArrayAll(array1, ^uint8_t(const CTObject * object1) {
		return CTArrayAny(array2, ^uint8_t(const CTObject * object2) {
			return CTObjectCompare(object1, object2);
		});
	});
}

void CTArrayAddEntry(CTArrayRef restrict array, void * value, int8_t type)
{
	CTArrayAddEntry2(array, CTObjectCreate(array->alloc, value, type));
}

void CTArrayAddEntry2(CTArrayRef restrict array, CTObjectRef restrict value)
{
	assert(value);
    uint64_t index = array->count++;
	if (index >= array->size)
	{
		array->size = kArrayGrowthFactor * array->count;
		assert(array->elements = CTAllocatorReallocate(array->alloc, array->elements, sizeof(CTArray *) * array->size));
	}
    array->elements[index] = value;
}

CTObjectRef CTArrayEntry(const CTArray * restrict array, uint64_t index)
{
	assert(index < array->count);
	return array->elements[index];
}


void CTArraySetObjectAtIndex(CTArray * restrict array, uint64_t index, CTObjectRef restrict object)
{
	assert(index < array->count);
	array->elements[index] = object;
}

void CTArrayDeleteEntry(CTArrayRef restrict array, uint64_t index)
{
	assert(array->count > index);
	CTObjectRelease(array->elements[index]);
	memmove(array->elements + index, array->elements + index + 1, sizeof(CTObject **) * (--array->count - index));
}

uint64_t CTArrayIndexOfFirstEntry(const CTArray * restrict array, uint8_t (^cmpFn)(const CTObject * object))
{
	for (uint64_t i = 0; i < array->count; ++i)
	{
		if (cmpFn(array->elements[i]))
		{
			return i;
		}
	}
	return CT_NOT_FOUND;
}

uint64_t CTArrayIndexOfEntryByReference(const CTArray * restrict array, CTObject * restrict value)
{
    return CTArrayIndexOfFirstEntry(array, ^uint8_t(const CTObject *object) {
		return object == value;
	});
}

uint64_t CTArrayIndexOfEntryByValue(const CTArray * restrict array, CTObjectRef value)
{
	return CTArrayIndexOfFirstEntry(array, ^uint8_t(const CTObject *object) {
		return CTObjectCompare(object, value);
	});
}

void CTArrayEmpty(CTArrayRef restrict array)
{
	if (array->count)
	{
		CTArrayEach(array, ^(CTObject *object) {
			CTObjectRelease(object);
		});
		array->count = 0;
		array->size = 0;
		CTAllocatorDeallocate(array->alloc, array->elements);
		array->elements = NULL;
	}
}

inline CTObjectRef CTArrayObjectAtIndex(const CTArray * restrict array, uint64_t index)
{
	assert(index < array->count);
	return array->elements[index];
}

inline uint64_t CTArrayCount(const CTArray * restrict array)
{
	return array->count;
}

void CTArrayMapMutate(CTArrayRef restrict array, void (^mapFn)(const CTObject * object))
{
	for (uint64_t i = 0; i < array->count; ++i)
	{
		mapFn(array->elements[i]);
	}
}

CTArray * CTArrayMap(CTAllocatorRef alloc, const CTArray * restrict array, CTObject * (^mapFn)(const CTObject * object))
{
	CTArrayRef new_array = CTArrayCreate(alloc);
	for (uint64_t i = 0; i < array->count; ++i)
	{
		CTArrayAddEntry2(new_array, mapFn(array->elements[i]));
	}
	return new_array;
}

void CTArrayFilterMutate(CTArray * restrict array, uint8_t (^filterFn)(const CTObject * object))
{
	for (uint64_t i = CTArrayCount(array); i > 0; --i)
	{
		if (!filterFn(array->elements[i - 1]))
		{
			CTArrayDeleteEntry(array, i - 1);
		}
	}
}

CTArray * CTArrayFilter(CTAllocatorRef alloc, const CTArray * restrict array, uint8_t (^filterFn)(const CTObject * object))
{
	CTArray * newArray = CTArrayCreate(alloc);
	for (uint64_t i = 0; i < array->count; ++i)
	{
		if (filterFn(array->elements[i]))
		{
			CTArrayAddEntry2(newArray, CTObjectCopy(alloc, array->elements[i]));
		}
	}
	return newArray;
}

uint8_t CTArrayAll(const CTArray * restrict array, uint8_t (^cmpFn)(const CTObject * object))
{
	uint8_t retVal = 1;
	
	for (uint64_t i = 0; i < array->count && retVal; ++i)
	{
		retVal &= cmpFn(array->elements[i]);
	}
	
	return retVal;
}

uint8_t CTArrayAllError(const CTArray * restrict array, CTErrorRef * error, uint8_t (^cmpFn)(const CTObject * object, CTError ** error))
{
	uint8_t retVal = 1;
	
	for (uint64_t i = 0; i < array->count && retVal; ++i)
	{
		retVal &= cmpFn(array->elements[i], error);
	}
	
	return retVal;
}

uint8_t CTArrayAny(const CTArray * restrict array, uint8_t (^cmpFn)(const CTObject * object))
{
	return CTArrayFirst(array, cmpFn) != NULL;
}

CTObject * CTArrayFirst(const CTArray * array, uint8_t (^cmpFn)(const CTObject * object))
{
	for (uint64_t i = 0; i < array->count; ++i)
	{
		if (cmpFn(array->elements[i]))
		{
			return array->elements[i];
		}
	}
	return NULL;
}

void CTArrayEach(const CTArray * restrict array, void (^eachFn)(CTObject * object))
{
	for (uint64_t i = 0; i < array->count; ++i)
	{
		eachFn(array->elements[i]);
	}
}

void CTArrayReverseEach(const CTArray * restrict array, void (^eachFn)(CTObject * object))
{
	for (uint64_t i = CTArrayCount(array); i > 0; --i)
	{
		eachFn(array->elements[i - 1]);
	}
}

CTArray * CTArrayReverse(CTAllocator * restrict alloc, const CTArray * restrict array)
{
	CTArray * new_array = CTArrayCreate(alloc);
	for (uint64_t i = CTArrayCount(array); i > 0; --i)
	{
		CTArrayAddEntry2(new_array, CTArrayObjectAtIndex(array, i - 1));
	}
	return new_array;
}

const CTArray * CTArraySubsetFromIndex(CTAllocatorRef alloc, const CTArray * array, uint64_t index)
{
	if (index > 0)
	{
		CTArrayRef new_array = CTArrayCreate(alloc);
		if (index < array->count)
		{
			new_array->elements = &array->elements[index];
			new_array->count = array->count - index;
		}
		return new_array;
	}
	return array;
}

CTObject * CTArrayReduce(CTObject * start, const CTArray * array, CTObject * (^redFn)(CTObject * accumulator, const CTObject * object))
{
	for (uint64_t i = 0; i < array->count; ++i)
	{
		start = redFn(start, array->elements[i]);
	}
	return start;
}

CTNumber * CTArrayMin(CTNumber * container, const CTArray * array)
{
	for (uint64_t i = 0; i < array->count; ++i)
	{
		assert(CTObjectType(array->elements[i]) == CTOBJECT_TYPE_NUMBER);
		const CTNumber * number = CTObjectValue(array->elements[i]);
		if (CTNumberDoubleValue(number) < CTNumberDoubleValue(container))
		{
			CTNumberSetDoubleValue(container, CTNumberDoubleValue(number));
		}
	}
	return container;
}

CTNumber * CTArrayMax(CTNumber * container, const CTArray * array)
{
	for (uint64_t i = 0; i < array->count; ++i)
	{
		assert(CTObjectType(array->elements[i]) == CTOBJECT_TYPE_NUMBER);
		const CTNumber * number = CTObjectValue(array->elements[i]);
		if (CTNumberDoubleValue(number) > CTNumberDoubleValue(container))
		{
			CTNumberSetDoubleValue(container, CTNumberDoubleValue(number));
		}
	}
	return container;
}

CTNumber * CTArrayAverage(CTNumber * container, const CTArray * array)
{
	for (uint64_t i = 0; i < array->count; ++i)
	{
		assert(CTObjectType(array->elements[i]) == CTOBJECT_TYPE_NUMBER);
		const CTNumber * number = CTObjectValue(array->elements[i]);
		CTNumberSetDoubleValue(container, CTNumberDoubleValue(container) + CTNumberDoubleValue(number) / array->count);
	}
	return container;
}

CTObjectRef CTObjectWithArray(CTAllocatorRef alloc, CTArray * restrict array)
{
	return CTObjectCreate(alloc, array, CTOBJECT_TYPE_ARRAY);
}
