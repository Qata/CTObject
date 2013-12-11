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
    CTAllocator * alloc;
	CTString * error;
	int code;
} CTError;

CTError * CTErrorCreate(CTAllocator * alloc, const char * restrict error, int code);
void CTErrorRelease(CTError * restrict error);
const CTString * CTErrorGetError(CTError * restrict error);