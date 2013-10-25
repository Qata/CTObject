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

CTString * CTStringCreate(CTAllocator * restrict alloc, const char * restrict characters)
{
    CTString * string = CTAllocatorAllocate(alloc, sizeof(CTString));
    string->alloc = alloc;
    string->characters = stringDuplicate(alloc, characters);
    string->length = strlen(characters);
    return string;
}

void CTStringPrependCharacters(CTString * restrict string, const char * restrict characters)
{
    char * result = CTAllocatorAllocate(string->alloc, string->length + strlen(characters) + 1);
    strcat(result, characters);
    strcat(result, string->characters);
    result[string->length + strlen(characters)] = 0;
    CTAllocatorDeallocate(string->alloc, string->characters);
    string->characters = result;
    string->length = string->length + strlen(characters);
}

void CTStringPrependCharacter(CTString * restrict string, char character)
{
	char * result = CTAllocatorAllocate(string->alloc, string->length + 1 + 1);
    strcat(result, &character);
    strcat(result, string->characters);
    result[string->length + 1] = 0;
    CTAllocatorDeallocate(string->alloc, string->characters);
    string->characters = result;
    string->length = string->length + 1;
}

void CTStringAppendCharacters(CTString * restrict string, const char * restrict characters)
{
    char * result = CTAllocatorAllocate(string->alloc, string->length + strlen(characters) + 1);
    strcat(result, string->characters);
    strcat(result, characters);
    result[string->length + strlen(characters)] = 0;
    CTAllocatorDeallocate(string->alloc, string->characters);
    string->characters = result;
    string->length = string->length + strlen(characters);
}

void CTStringAppendCharacter(CTString * restrict string, char character)
{
    char * result = CTAllocatorAllocate(string->alloc, string->length + 1 + 1);
    strcat(result, string->characters);
    strcat(result, &character);
    result[string->length + 1] = 0;
    CTAllocatorDeallocate(string->alloc, string->characters);
    string->characters = result;
    string->length = string->length + 1;
}

void CTStringSet(CTString * restrict string, const char * restrict characters)
{
    CTAllocatorDeallocate(string->alloc, string->characters);
    string->characters = stringDuplicate(string->alloc, characters);
    string->length = strlen(characters);
}

void CTStringRemoveCharactersFromStart(CTString * restrict string, unsigned long count)
{
    if (count < string->length)
    {
        char * new = CTAllocatorAllocate(string->alloc, string->length - count + 1);
        memcpy(new, string->characters + count, string->length - count);
        CTAllocatorDeallocate(string->alloc, string->characters);
        string->characters = new;
        string->length = string->length - count + 1;
    }
    else
    {
        string->characters = "";
        string->length = 0;
    }
}

void CTStringRemoveCharactersFromEnd(CTString * restrict string, unsigned long count)
{
    if (count < string->length)
    {
        char * new = CTAllocatorAllocate(string->alloc, string->length - count + 1);
        memcpy(new, string->characters, string->length - count);
        CTAllocatorDeallocate(string->alloc, string->characters);
        string->characters = new;
        string->length = string->length - count + 1;
    }
    else
    {
        string->characters = "";
        string->length = 0;
    }
}

const char * CTStringStringBetween(CTString * restrict string, const char * restrict search1, const char * restrict search2)
{
	char * ret1, * ret2;
	if ((ret1 = strstr(string->characters, search1)) && (ret2 = strstr(string->characters, search2)) && ret1 < ret2)
	{
		char * retVal = CTAllocatorAllocate(string->alloc, ret2 - (ret1 + strlen(search1)));
		strncpy(retVal, ret1 + strlen(search1), ret2 - (ret1 + strlen(search1)));
		retVal[ret2 - ret1 + 1] = 0;
		return retVal;
	}
	return NULL;
}