//
//  CTString.h
//  WatchBoxCGI
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"

typedef struct
{
    CTAllocator * alloc;
    unsigned long length;
    char * characters;
} CTString;

CTString * CTStringCreate(CTAllocator * restrict alloc, const char * restrict characters);
void CTStringAppendCharacters(CTString * restrict string, const char * restrict characters);
void CTStringPrependCharacters(CTString * restrict string, const char * restrict characters);
void CTStringSet(CTString * restrict string, const char * restrict characters);
void CTStringRemoveCharactersFromStart(CTString * restrict string, unsigned long count);
void CTStringRemoveCharactersFromEnd(CTString * restrict string, unsigned long count);
const char * CTStringStringBetween(CTString * restrict string, const char * restrict search1, const char * restrict search2);