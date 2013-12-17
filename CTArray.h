//
//  CTArray.h
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include "CTObject.h"

typedef struct
{
    CTAllocator * alloc;
    uint64_t count;
    CTObject ** elements;
} CTArray;

CTArray * CTArrayCreate(CTAllocator * restrict alloc);
void CTArrayRelease(CTArray * restrict array);
void CTArrayAddEntry(CTArray * restrict array, CTObject * restrict value);
void CTArrayDeleteEntry(CTArray * restrict array, uint64_t index);
void CTArrayEmpty(CTArray * restrict array);
CTObject * CTArrayObjectAtIndex(CTArray * restrict array, uint64_t index);
uint64_t CTArrayIndexOfEntry(CTArray * restrict array, CTObject * value);