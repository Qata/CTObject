//
//  CTData.c
//  CTObject
//
//  Created by Carlo Tortorella on 21/01/14.
//  Copyright (c) 2014 Carlo Tortorella. All rights reserved.
//

#include "CTData.h"
#include <string.h>

CTDataRef CTDataCreate(CTAllocatorRef restrict alloc, const void * restrict bytes, uint64_t length)
{
    CTDataRef data = CTAllocatorAllocate(alloc, sizeof(CTData));
    void * copy = CTAllocatorAllocate(alloc, length);
    memcpy(copy, bytes, length);
    data->bytes = copy;
    data->length = length;
    return data;
}

const void * CTDataGetBytes(const CTData * restrict data)
{
    return data->bytes;
}

uint64_t CTDataGetLength(const CTData * restrict data)
{
    return data->length;
}

uint8_t CTDataGetByteAtIndex(const CTData * restrict data, uint64_t index)
{
    if (index < data->length)
    {
        return ((uint8_t *)data->bytes)[index];
    }
    return 0;
}