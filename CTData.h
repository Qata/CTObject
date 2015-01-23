//
//  CTData.h
//  CTObject
//
//  Created by Carlo Tortorella on 21/01/14.
//  Copyright (c) 2014 Carlo Tortorella. All rights reserved.
//

#pragma once
#include <stdint.h>
#include "CTAllocator.h"

typedef struct
{
    const void * bytes;
    uint64_t length;
} CTData, * CTDataRef;

CTDataRef CTDataCreate(CTAllocatorRef restrict alloc, const void * restrict bytes, uint64_t length);
const void * CTDataGetBytes(const CTDataRef restrict data);
uint8_t CTDataGetByteAtIndex(const CTDataRef restrict data, uint64_t index);
uint64_t CTDataGetLength(const CTDataRef restrict data);