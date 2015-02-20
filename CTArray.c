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
#include "CTArray.h"

const double kArrayGrowthFactor = 1.625;

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

CTArrayRef CTArrayCopy(CTAllocatorRef alloc, CTArrayRef array)
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

uint8_t CTArrayCompare(CTArrayRef array1, CTArrayRef array2)
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

CTObjectRef CTArrayEntry(const CTArrayRef restrict array, uint64_t index)
{
	if (index < array->count)
	{
		return array->elements[index];
	}
	return NULL;
}


void CTArrayDeleteEntry(CTArrayRef restrict array, uint64_t index)
{
	assert(array->count > index);
	for (uint64_t i = 0; i < array->count; ++i)
	{
		if (i != index)
		{
			array->elements[i - (i >= index)] = array->elements[i];
		}
		else
		{
			CTObjectRelease(array->elements[i]);
		}
	}
	--array->count;
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

inline CTObjectRef CTArrayObjectAtIndex(CTArrayRef restrict array, uint64_t index)
{
	assert(index < array->count);
	return array->elements[index];
}

inline uint64_t CTArrayCount(CTArrayRef restrict array)
{
	return array->count;
}

CTObjectRef CTObjectWithArray(CTAllocatorRef alloc, CTArrayRef restrict array)
{
	return CTObjectCreate(alloc, array, CTOBJECT_TYPE_ARRAY);
}