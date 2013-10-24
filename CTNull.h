//
//  CTNumber.h
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"

typedef struct
{
    CTAllocator * alloc;
    void * value;
} CTNull;

CTNull * CTNullCreate(CTAllocator * restrict alloc);