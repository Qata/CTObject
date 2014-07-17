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
    CTAllocator * alloc;
    char * value;
} CTNull;

CTNull * CTNullCreate(CTAllocator * restrict alloc);
const char * CTNullValue(CTNull * null);
void CTNullRelease(CTNull * null);
CTObject * CTObjectWithNull(CTNull * restrict n);