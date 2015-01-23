//
//  CTString.h
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include "CTObject.h"

#define CTSTRING_NO_LIMIT -1

typedef uint64_t hash_t;

typedef struct
{
    CTAllocator * alloc;
    uint64_t length;
    char * characters;
	uint64_t hash;
	uint8_t modified;
} CTString;

CTString * CTStringCreate(CTAllocator * restrict alloc, const char * restrict characters);
CTString * CTStringCopy(CTAllocator * restrict alloc, CTString * string);
void CTStringRelease(CTString * string);

hash_t CTStringCharHash(const char * restrict string);

const char * CTStringUTF8String(const CTString * restrict string);
uint64_t CTStringLength(const CTString * restrict string);
hash_t CTStringHash(CTString * restrict string);
void CTStringSetLength(CTString * restrict string, uint64_t length);
void CTStringAppendCharacters(CTString * restrict string, const char * restrict characters, int64_t limit);
void CTStringAppendCharacter(CTString * restrict string, char character);
void CTStringPrependCharacters(CTString * restrict string, const char * restrict characters, int64_t limit);
void CTStringPrependCharacter(CTString * restrict string, char character);
void CTStringSet(CTString * restrict string, const char * restrict characters);
void CTStringRemoveCharactersFromStart(CTString * restrict string, unsigned long count);
void CTStringRemoveCharactersFromEnd(CTString * restrict string, unsigned long count);

void CTStringAppendString(CTString * restrict string1, CTString * restrict string2);
void CTStringToUpper(CTString * restrict string);
void CTStringToLower(CTString * restrict string);

const char * CTStringStringBetween(CTString * restrict string, const char * restrict search1, const char * restrict search2);
uint8_t CTStringContainsString(CTString * restrict string, const char * restrict search);
int8_t CTStringCompare(CTString * restrict string1, CTString * restrict string2);
int8_t CTStringCompare2(CTString * restrict string1, const char * restrict string2);
uint8_t CTStringIsEqual2(CTString * restrict string1, const char * restrict string2);

/**
 * Return a CTObject encasing the CTString passed.
 * @param str	A properly initialised CTString that was created with CTStringCreate.
 * @return		The CTString wrapped in a CTObject. The result is identical to using CTObjectCreate.
 **/
CTObject * CTObjectWithString(CTAllocator * alloc, CTString * restrict str);