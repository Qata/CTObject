//
//  CTString.c
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTString.h"
#include "CTFunctions.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

CTString * CTStringCreate(CTAllocator * restrict alloc, const char * restrict characters)
{
    CTString * string = CTAllocatorAllocate(alloc, sizeof(CTString));
    string->alloc = alloc;
    string->characters = stringDuplicate(alloc, characters ? characters : "");
	if (characters)
	{
		CTStringSetLength(string, strlen(characters));
	}
    return string;
}

void CTStringRelease(CTString * string)
{
    CTAllocatorDeallocate(string->alloc, string->characters);
    CTAllocatorDeallocate(string->alloc, string);
}

const char * CTStringUTF8String(const CTString * restrict string)
{
	return string->characters;
}

uint64_t CTStringLength(const CTString * restrict string)
{
	return string->length;
}

void CTStringSetLength(CTString * restrict string, uint64_t length)
{
	string->length = length;
}

void CTStringPrependCharacters(CTString * restrict string, const char * restrict characters, int64_t limit)
{
	const uint64_t length = limit < 0 ? strlen(characters) : limit;
	string->characters = CTAllocatorReallocate(string->alloc, string->characters, string->length + length + 1);
	memmove(string->characters + length, string->characters, string->length + 1);
	memcpy(string->characters, characters, length);
	string->length += length;
}

void CTStringPrependCharacter(CTString * restrict string, char character)
{
	string->characters = CTAllocatorReallocate(string->alloc, string->characters, string->length + 2);
	memmove(string->characters + 1, string->characters, string->length + 1);
	string->characters[0] = character;
	++string->length;
}

void CTStringAppendCharacters(CTString * restrict string, const char * restrict characters, int64_t limit)
{
	const uint64_t length = limit < 0 ? strlen(characters) : limit;
	string->characters = CTAllocatorReallocate(string->alloc, string->characters, CTStringLength(string) + length + 1);
	memcpy(string->characters + string->length, characters, length);
	string->length += length;
	string->characters[string->length] = 0;
}

void CTStringAppendCharacter(CTString * restrict string, char character)
{
	string->characters = CTAllocatorReallocate(string->alloc, string->characters, string->length + 2);
	string->characters[string->length] = character;
	++string->length;
	string->characters[string->length] = 0;
}

void CTStringSet(CTString * restrict string, const char * restrict characters)
{
    CTAllocatorDeallocate(string->alloc, string->characters);
    string->characters = stringDuplicate(string->alloc, characters);
    CTStringSetLength(string, strlen(characters));
}

void CTStringRemoveCharactersFromStart(CTString * restrict string, unsigned long count)
{
    if (count < CTStringLength(string))
    {
		memmove(string->characters, string->characters + count, string->length - count + 1);
		CTAllocatorReallocate(string->alloc, string->characters, string->length - count + 1);
		string->length -= count;
    }
    else
    {
        string->characters = CTAllocatorReallocate(string->alloc, string->characters, 1);
		string->characters[0] = 0;
		string->length = 0;
    }
}

void CTStringRemoveCharactersFromEnd(CTString * restrict string, unsigned long count)
{
    if (count < CTStringLength(string))
    {
		CTAllocatorReallocate(string->alloc, string->characters, string->length - count + 1);
		string->characters[string->length - count] = 0;
		string->length -= count;
    }
    else
    {
        string->characters = CTAllocatorReallocate(string->alloc, string->characters, 1);
		string->characters[0] = 0;
		string->length = 0;
    }
}

void CTStringAppendString(CTString * restrict string1, CTString * restrict string2)
{
	string1->characters = CTAllocatorReallocate(string1->alloc, string1->characters, CTStringLength(string1) + CTStringLength(string2) + 1);
	strcat(string1->characters, CTStringUTF8String(string2));
	string1->characters[CTStringLength(string1) + CTStringLength(string2)] = 0;
	CTStringSetLength(string1, CTStringLength(string1) + CTStringLength(string2));
}

void CTStringToUpper(CTString * restrict string)
{
	for (uint64_t i = 0; i < CTStringLength(string); ++i)
	{
		string->characters[i] = toupper(string->characters[i]);
	}
}

void CTStringToLower(CTString * restrict string)
{
	for (uint64_t i = 0; i < CTStringLength(string); ++i)
	{
		string->characters[i] = tolower(string->characters[i]);
	}
}

const char * CTStringStringBetween(CTString * restrict string, const char * restrict search1, const char * restrict search2)
{
	uint64_t index = 0;
	char * ret1 = NULL, * ret2 = NULL;
	
	while ((!ret1 && !ret2) || ret1 >= ret2)
	{
		if (index + strlen(search1) < CTStringLength(string) && (ret1 = strstr(CTStringUTF8String(string) + index, search1)) && (ret2 = strstr(CTStringUTF8String(string) + index + strlen(search1), search2)))
		{
			if (ret1 < ret2)
			{
				char * retVal = CTAllocatorAllocate(string->alloc, ret2 - (ret1 + strlen(search1)));
				strncpy(retVal, ret1 + strlen(search1), ret2 - (ret1 + strlen(search1)));
				return retVal;
			}
			index = ret1 - CTStringUTF8String(string);
			if (index >= CTStringLength(string))
				return NULL;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

uint8_t CTStringContainsString(CTString * restrict string, const char * restrict search)
{
	return strstr(CTStringUTF8String(string), search) ? 1 : 0;
}

int8_t CTStringCompare(CTString * restrict string1, CTString * restrict string2)
{
	return strcmp(CTStringUTF8String(string1), CTStringUTF8String(string2));
}

int8_t CTStringCompare2(CTString * restrict string1, const char * restrict string2)
{
	return strcmp(CTStringUTF8String(string1), string2);
}

CTObject * CTObjectWithString(CTAllocator * alloc, CTString * restrict str)
{
	return CTObjectCreate(alloc, str, CTOBJECT_TYPE_STRING);
}