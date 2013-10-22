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

CTString * CTStringCreate(CTAllocator * alloc, const char * restrict characters);
void CTStringAppendCharacters(CTString * string, const char * restrict characters);
void CTStringPrependCharacters(CTString * string, const char * restrict characters);
void CTStringSet(CTString * string, const char * restrict characters);
void CTStringRemoveCharactersFromStart(CTString * string, unsigned long count);
void CTStringRemoveCharactersFromEnd(CTString * string, unsigned long count);
const char * CTStringStringBetween(CTString * string, const char * restrict search1, const char * restrict search2);