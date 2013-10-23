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
#include "CTArray.h"

CTArray * CTArrayCreate(CTAllocator * restrict alloc)
{
    CTArray * array = CTAllocatorAllocate(alloc, sizeof(CTArray));
    array->count = 0;
    array->values = NULL;
    array->alloc = alloc;
    
    return array;
}

void CTArrayAddEntry(CTArray * restrict array, const char * restrict value)
{
    unsigned long index = array->count++;
    
    if (!(array->values = CTAllocatorReallocate(array->alloc, array->values, sizeof(CTArray *) * array->count)))
    {
        exit(1);
    }
    
    array->values[index] = CTStringCreate(array->alloc, value);
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
                values[count++] = array->values[i];
            }
            else
            {
                CTAllocatorDeallocate(array->alloc, array->values[i]);
            }
        }
        CTAllocatorDeallocate(array->alloc, array->values);
        array->values = values;
        --array->count;
	}
}

unsigned long CTArrayIndexOfEntry(CTArray * restrict array, const char * restrict value)
{
    for (unsigned long i = 0; i < array->count; i++)
    {
        if (!strcmp(array->values[i]->characters, value))
        {
            return i;
        }
    }
    return -1;
}