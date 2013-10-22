//
//  CTArray.h
//  WatchBoxCGI
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
    unsigned long count;
    CTString ** values;
} CTArray;

CTArray * CTArrayCreate(CTAllocator * alloc);
void CTArrayAddEntry(CTArray * array, const char * restrict value, unsigned long size);
void CTArrayDeleteEntry(CTArray * array, unsigned long index);
unsigned long CTArrayIndexOfEntry(CTArray * array, const char * restrict value);