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

CTString * CTStringCreate(CTAllocator * restrict alloc, const char * restrict characters)
{
    CTString * string = CTAllocatorAllocate(alloc, sizeof(CTString));
    string->alloc = alloc;
    string->characters = stringDuplicate(alloc, characters);
    CTStringSetLength(string, strlen(characters));
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
    char * result = CTAllocatorAllocate(string->alloc, CTStringLength(string) + strlen(characters) + 1);
    strcat(result, characters);
    strcat(result, CTStringUTF8String(string));
    result[CTStringLength(string) + strlen(characters)] = 0;
    CTAllocatorDeallocate(string->alloc, string->characters);
    string->characters = result;
    CTStringSetLength(string, CTStringLength(string) + strlen(characters));
}

void CTStringPrependCharacter(CTString * restrict string, char character)
{
	char * result = CTAllocatorAllocate(string->alloc, CTStringLength(string) + 1 + 1);
    strcat(result, &character);
    strcat(result, CTStringUTF8String(string));
    result[CTStringLength(string) + 1] = 0;
    CTAllocatorDeallocate(string->alloc, string->characters);
    string->characters = result;
    CTStringSetLength(string, CTStringLength(string) + 1);
}

void CTStringAppendCharacters(CTString * restrict string, const char * restrict characters, int64_t limit)
{
	char * result = NULL;
	if (limit < 0)
	{
		result = CTAllocatorAllocate(string->alloc, CTStringLength(string) + strlen(characters) + 1);
		strcat(result, CTStringUTF8String(string));
		strcat(result, characters);
		result[CTStringLength(string) + strlen(characters)] = 0;
		CTAllocatorDeallocate(string->alloc, string->characters);
		string->characters = result;
		CTStringSetLength(string, CTStringLength(string) + strlen(characters));
	}
	else
	{
		result = CTAllocatorAllocate(string->alloc, CTStringLength(string) + limit + 1);
		strcat(result, CTStringUTF8String(string));
		strncat(result, characters, limit);
		result[CTStringLength(string) + limit] = 0;
		CTAllocatorDeallocate(string->alloc, string->characters);
		string->characters = result;
		CTStringSetLength(string, CTStringLength(string) + limit);
	}
}

void CTStringAppendCharacter(CTString * restrict string, char character)
{
    char * result = CTAllocatorAllocate(string->alloc, CTStringLength(string) + 1 + 1);
    strcat(result, CTStringUTF8String(string));
	strncat(result, &character, 1);
    CTStringSetLength(string, CTStringLength(string) + 1);
    result[CTStringLength(string)] = 0;
    CTAllocatorDeallocate(string->alloc, string->characters);
    string->characters = result;
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
        char * new = CTAllocatorAllocate(string->alloc, CTStringLength(string) - count + 1);
        memcpy(new, CTStringUTF8String(string) + count, CTStringLength(string) - count);
        CTAllocatorDeallocate(string->alloc, string->characters);
        string->characters = new;
        CTStringSetLength(string, CTStringLength(string) - count + 1);
    }
    else
    {
        string->characters = "";
        CTStringSetLength(string, 0);
    }
}

void CTStringRemoveCharactersFromEnd(CTString * restrict string, unsigned long count)
{
    if (count < CTStringLength(string))
    {
        char * new = CTAllocatorAllocate(string->alloc, CTStringLength(string) - count + 1);
        memcpy(new, CTStringUTF8String(string), CTStringLength(string) - count);
        CTAllocatorDeallocate(string->alloc, string->characters);
        string->characters = new;
        CTStringSetLength(string, CTStringLength(string) - count + 1);
    }
    else
    {
        string->characters = "";
        CTStringSetLength(string, 0);
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
				retVal[ret2 - ret1 + 1] = 0;
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

int CTStringContainsString(CTString * restrict string, const char * restrict search)
{
	return (int)strstr(CTStringUTF8String(string), search);
}