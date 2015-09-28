//
//  CTArray.c
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
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
	for (uint64_t i = 0; i < array->count; ++i)
	{
		CTArrayAddEntry2(new_array, CTObjectCopy(alloc, CTArrayObjectAtIndex(array, i)));
	}
	return new_array;
}

void CTArrayRelease(CTArrayRef restrict array)
{
	for (uint64_t i = 0; i < array->count; ++i)
    {
        CTObjectRelease(array->elements[i]);
    }
	CTAllocatorDeallocate(array->alloc, array->elements);
	CTAllocatorDeallocate(array->alloc, array);
}

uint8_t CTArrayCompare(const CTArray * array1, const CTArray * array2)
{
	if (array1->count == array2->count)
	{
		for (uint64_t i = 0; i < array1->count; ++i)
		{
			CTObjectRef entry1 = CTArrayEntry(array1, i);
			CTObjectRef entry2 = NULL;
			for (uint64_t j = 0; j < array2->count; ++j)
			{
				CTObjectRef entryTemp = CTArrayEntry(array2, j);
				if (CTObjectCompare(entry1, entryTemp))
				{
					entry2 = entryTemp;
					break;
				}
			}
			if (!entry2) return 0;
		}
		return 1;
	}
	return 0;
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
		assert(array->elements = CTAllocatorReallocate(array->alloc, array->elements, sizeof(CTArrayRef) * array->size));
	}
    array->elements[index] = value;
}

CTObjectRef CTArrayEntry(const CTArray * restrict array, uint64_t index)
{
	assert(index < array->count);
	return array->elements[index];
}


void CTArrayDeleteEntry(CTArrayRef restrict array, uint64_t index)
{
	assert(array->count > index);
	CTObjectRelease(array->elements[index]);
	memmove(array->elements + index, array->elements + index + 1, sizeof(CTObjectRef *) * (--array->count - index));
}

uint64_t CTArrayIndexOfEntryByReference(CTArrayRef restrict array, CTObjectRef restrict value)
{
    for (uint64_t i = 0; i < array->count; ++i)
    {
        if (array->elements[i] == value)
        {
            return i;
        }
    }
    return CT_NOT_FOUND;
}

uint64_t CTArrayIndexOfEntryByValue(CTArrayRef restrict array, CTObjectRef value)
{
    for (uint64_t i = 0; i < array->count; ++i)
    {
        if (CTObjectCompare(array->elements[i], value))
        {
            return i;
        }
    }
    return CT_NOT_FOUND;
}

void CTArrayEmpty(CTArrayRef restrict array)
{
	if (array->count)
	{
		for (uint64_t i = 0; i < array->count; ++i)
		{
			CTObjectRelease(array->elements[i]);
		}
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

inline uint64_t CTArrayCount(CTArrayRef restrict array)
{
	return array->count;
}

void CTArrayMap(CTArrayRef restrict array, void (^mapFn)(CTObjectRef))
{
	for (uint64_t i = 0; i < array->count; ++i)
	{
		mapFn(array->elements[i]);
	}
}

CTArrayRef CTArrayCopyMap(CTAllocatorRef alloc, const CTArray * restrict array, void (^mapFn)(CTObjectRef))
{
	CTArrayRef newArray = CTArrayCopy(alloc, array);
	CTArrayMap(newArray, mapFn);
	return newArray;
}

void CTArrayFilter(CTArrayRef restrict array, uint8_t (^filterFn)(CTObjectRef))
{
	for (int64_t i = array->count - 1; i >= 0; --i)
	{
		if (!filterFn(array->elements[i]))
		{
			CTArrayDeleteEntry(array, i);
		}
	}
}

CTArrayRef CTArrayCopyFilter(CTAllocatorRef alloc, const CTArray * restrict array, uint8_t (^filterFn)(CTObjectRef))
{
	CTArrayRef newArray = CTArrayCreate(alloc);
	for (uint64_t i = 0; i < array->count; ++i)
	{
		if (filterFn(array->elements[i]))
		{
			CTArrayAddEntry2(newArray, CTObjectCopy(alloc, array->elements[i]));
		}
	}
	return newArray;
}

uint8_t CTArrayAll(const CTArray * restrict array, uint8_t (^cmpFn)(const CTObject *))
{
	uint8_t retVal = 1;
	
	for (uint64_t i = 0; i < array->count && retVal; ++i)
	{
		retVal &= cmpFn(array->elements[i]);
	}
	
	return retVal;
}

uint8_t CTArrayAllError(const CTArray * restrict array, CTErrorRef * error, uint8_t (^cmpFn)(const CTObject *, CTErrorRef *))
{
	uint8_t retVal = 1;
	
	for (uint64_t i = 0; i < array->count && retVal; ++i)
	{
		retVal &= cmpFn(array->elements[i], error);
	}
	
	return retVal;
}

uint8_t CTArrayAny(const CTArray * restrict array, uint8_t (^cmpFn)(const CTObject *))
{
	uint8_t retVal = 0;
	
	for (uint64_t i = 0; i < array->count && !retVal; ++i)
	{
		retVal |= cmpFn(array->elements[i]);
	}
	
	return retVal;
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

CTObjectRef CTObjectWithArray(CTAllocatorRef alloc, CTArray * restrict array)
{
	return CTObjectCreate(alloc, array, CTOBJECT_TYPE_ARRAY);
}
