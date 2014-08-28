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

CTArray * CTArrayCreate(CTAllocator * restrict alloc)
{
    CTArray * array = CTAllocatorAllocate(alloc, sizeof(CTArray));
    array->alloc = alloc;
    return array;
}

CTArray * CTArrayCreateWithObjects(CTAllocator * restrict alloc, ...)
{
	CTArray * retVal = CTArrayCreate(alloc);
	va_list list;
	va_start(list, alloc);
	for (CTObject * value = va_arg(list, CTObject*); value != NULL; value = va_arg(list, CTObject*))
	{
		CTArrayAddEntry2(retVal, value);
	}
	va_end(list);
	return retVal;
}

void CTArrayRelease(CTArray * restrict array)
{
	for (uint64_t i = 0; i < array->count; ++i)
    {
        CTObjectRelease(array->elements[i]);
    }
	CTAllocatorDeallocate(array->alloc, array->elements);
	CTAllocatorDeallocate(array->alloc, array);
}

uint8_t CTArrayCompare(CTArray * array1, CTArray * array2)
{
	if (array1->count == array2->count)
	{
		for (uint64_t i = 0; i < array1->count; ++i)
		{
			CTObject * entry1 = CTArrayEntry(array1, i);
			CTObject * entry2 = NULL;
			for (uint64_t j = 0; j < array2->count; ++j)
			{
				CTObject * entryTemp = CTArrayEntry(array2, j);
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

void CTArrayAddEntry(CTArray * restrict array, void * value, int8_t type)
{
	CTArrayAddEntry2(array, CTObjectCreate(array->alloc, value, type));
}

void CTArrayAddEntry2(CTArray * restrict array, CTObject * restrict value)
{
    uint64_t index = array->count++;
    
	assert(array->elements = CTAllocatorReallocate(array->alloc, array->elements, sizeof(CTArray *) * array->count));
    
    array->elements[index] = value;
}

CTObject * CTArrayEntry(const CTArray * restrict array, uint64_t index)
{
	if (index < array->count)
	{
		return array->elements[index];
	}
	return NULL;
}

void CTArrayDeleteEntry(CTArray * restrict array, uint64_t index)
{
    if (array->count > index)
	{
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
        array->elements = CTAllocatorReallocate(array->alloc, array->elements, sizeof(CTObject *) * --array->count);
	}
}

uint64_t CTArrayIndexOfEntryByReference(CTArray * restrict array, CTObject * restrict value)
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

uint64_t CTArrayIndexOfEntryByValue(CTArray * restrict array, CTObject * value)
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

void CTArrayEmpty(CTArray * restrict array)
{
	for (long long i = array->count; i >= 0; i--)
    {
        CTArrayDeleteEntry(array, i);
    }
}

CTObject * CTArrayObjectAtIndex(CTArray * restrict array, uint64_t index)
{
	return index < array->count ? array->elements[index] : NULL;
}

uint64_t CTArrayCount(CTArray * restrict array)
{
	return array->count;
}

CTObject * CTObjectWithArray(CTAllocator * alloc, CTArray * restrict array)
{
	return CTObjectCreate(alloc, array, CTOBJECT_TYPE_ARRAY);
}