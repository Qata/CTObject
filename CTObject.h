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

enum
{
	CTOBJECT_NOT_AN_OBJECT = -1,
    CTOBJECT_TYPE_DICTIONARY,
    CTOBJECT_TYPE_ARRAY,
    CTOBJECT_TYPE_STRING,
    CTOBJECT_TYPE_NUMBER,
	CTOBJECT_TYPE_NULL,
	CTOBJECT_TYPE_LARGE_NUMBER
};

typedef struct
{
    CTAllocator * alloc;
    uint64_t size;
    void * ptr;
    int8_t type;
} CTObject;

CTObject * CTObjectCreate(CTAllocator * restrict alloc, void * ptr, int8_t type);
CTObject * CTObjectCreate2(CTAllocator * restrict alloc, void * ptr, int8_t type, unsigned long size);
void CTObjectRelease(CTObject * object);