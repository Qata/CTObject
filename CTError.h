//
//  CTError.h
//  CTObject
//
//  Created by Carlo Tortorella on 11/11/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//
#pragma once
#include "CTAllocator.h"
#include "CTString.h"

typedef struct
{
    CTAllocatorRef alloc;
	CTStringRef error;
	int code;
} CTError, * CTErrorRef;

CTErrorRef CTErrorCreate(CTAllocatorRef alloc, const char * restrict error, int code);
void CTErrorRelease(CTErrorRef restrict error);
const CTStringRef CTErrorGetErrorString(CTErrorRef restrict error);