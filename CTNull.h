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
    char * value;
} CTNull, * CTNullRef;

CTNullRef CTNullCreate(void);
const char * CTNullValue(CTNullRef null);
void CTNullRelease(CTNullRef null);
CTObjectRef CTObjectWithNull(CTAllocatorRef alloc, CTNullRef restrict n);