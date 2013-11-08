//
//  CTArray.h
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include "CTString.h"

typedef struct
{
    CTAllocator * alloc;
    uint64_t count;
    CTString ** elements;
} CTArray;

CTArray * CTArrayCreate(CTAllocator * restrict alloc);
void CTArrayAddEntry(CTArray * restrict array, const char * restrict value);
void CTArrayDeleteEntry(CTArray * restrict array, unsigned long index);
unsigned long CTArrayIndexOfEntry(CTArray * restrict array, const char * restrict value);