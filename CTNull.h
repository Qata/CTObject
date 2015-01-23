//
//  CTNumber.h
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include "CTObject.h"

typedef struct
{
    CTAllocatorRef alloc;
    char * value;
} CTNull, * CTNullRef;

CTNullRef CTNullCreate(CTAllocatorRef restrict alloc);
const char * CTNullValue(CTNullRef null);
void CTNullRelease(CTNullRef null);
CTObjectRef CTObjectWithNull(CTAllocatorRef alloc, CTNullRef restrict n);