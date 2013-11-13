//
//  CTString.h
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"

typedef struct
{
    CTAllocator * alloc;
    uint64_t length;
    char * characters;
} CTString;

CTString * CTStringCreate(CTAllocator * restrict alloc, const char * restrict characters);
void CTStringAppendCharacters(CTString * restrict string, const char * restrict characters);
void CTStringAppendCharacter(CTString * restrict string, char character);
void CTStringPrependCharacters(CTString * restrict string, const char * restrict characters);
void CTStringPrependCharacter(CTString * restrict string, char character);
void CTStringSet(CTString * restrict string, const char * restrict characters);
void CTStringRemoveCharactersFromStart(CTString * restrict string, unsigned long count);
void CTStringRemoveCharactersFromEnd(CTString * restrict string, unsigned long count);
const char * CTStringStringBetween(CTString * restrict string, const char * restrict search1, const char * restrict search2);
int CTStringContainsString(CTString * restrict string, const char * restrict search);