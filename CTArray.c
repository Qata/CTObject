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
    array->count = 0;
    array->elements = NULL;
    array->alloc = alloc;
    return array;
}

void CTArrayRelease(CTArray * array)
{
	for (uint64_t i = 0; i < array->count; i++)
    {
        CTStringRelease(array->elements[i]);
    }
	CTAllocatorDeallocate(array->alloc, array->elements);
	CTAllocatorDeallocate(array->alloc, array);
}

void CTArrayAddEntry(CTArray * restrict array, const char * restrict value)
{
    unsigned long index = array->count++;
    
	assert((array->elements = CTAllocatorReallocate(array->alloc, array->elements, sizeof(CTArray *) * array->count)));
    
    array->elements[index] = CTStringCreate(array->alloc, value);
}

void CTArrayDeleteEntry(CTArray * restrict array, unsigned long index)
{
    if (array->count > index)
	{
		CTString ** values = CTAllocatorAllocate(array->alloc, sizeof(CTString *) * array->count - 1);
        for (unsigned long i = 0, count = 0; i < array->count; i++)
        {
            if (!(i == index))
            {
                values[count++] = array->elements[i];
            }
            else
            {
                CTAllocatorDeallocate(array->alloc, array->elements[i]);
            }
        }
        CTAllocatorDeallocate(array->alloc, array->elements);
        array->elements = values;
        --array->count;
	}
}

unsigned long CTArrayIndexOfEntry(CTArray * restrict array, const char * restrict value)
{
    for (unsigned long i = 0; i < array->count; i++)
    {
        if (!strcmp(array->elements[i]->characters, value))
        {
            return i;
        }
    }
    return -1;
}