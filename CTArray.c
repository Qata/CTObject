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
#include "CTArray.h"

CTArray * CTArrayCreate(CTAllocator * restrict alloc)
{
    CTArray * array = CTAllocatorAllocate(alloc, sizeof(CTArray));
    array->alloc = alloc;
    return array;
}

void CTArrayRelease(CTArray * restrict array)
{
	for (uint64_t i = 0; i < array->count; i++)
    {
        CTObjectRelease(array->elements[i]);
    }
	CTAllocatorDeallocate(array->alloc, array->elements);
	CTAllocatorDeallocate(array->alloc, array);
}

void CTArrayAddEntry(CTArray * restrict array, CTObject * restrict value)
{
    uint64_t index = array->count++;
    
	assert((array->elements = CTAllocatorReallocate(array->alloc, array->elements, sizeof(CTArray *) * array->count)));
    
    array->elements[index] = value;
}

void CTArrayDeleteEntry(CTArray * restrict array, uint64_t index)
{
    if (array->count > index)
	{
		CTObject ** values = CTAllocatorAllocate(array->alloc, sizeof(CTObject *) * array->count - 1);
        for (uint64_t i = 0, count = 0; i < array->count; i++)
        {
            if (!(i == index))
            {
                values[count++] = array->elements[i];
            }
            else
            {
				CTObjectRelease(array->elements[i]);
            }
        }
        CTAllocatorDeallocate(array->alloc, array->elements);
        array->elements = values;
        --array->count;
	}
}

uint64_t CTArrayIndexOfEntry(CTArray * restrict array, CTObject * restrict value)
{
    for (uint64_t i = 0; i < array->count; i++)
    {
        if (array->elements[i] == value)
        {
            return i;
        }
    }
    return -1;
}

void CTArrayEmpty(CTArray * restrict array)
{
	for (long long i = array->count; i >= 0x0; i--)
    {
        CTArrayDeleteEntry(array, i);
    }
}

CTObject * CTArrayObjectAtIndex(CTArray * restrict array, uint64_t index)
{
	return index < array->count ? array->elements[index] : NULL;
}