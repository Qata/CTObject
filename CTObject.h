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
    uint64_t size;
    void * ptr;
} CTObject;

CTObject * CTObjectCreate(CTAllocator * restrict alloc, void * ptr, unsigned long size);