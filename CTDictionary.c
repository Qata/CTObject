//
//  Dictionary.c
//  CTObject
//
//  Created by Carlo Tortorella on 17/09/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "CTDictionary.h"
#include "CTFunctions.h"

struct CTDictionaryEntry * CTDictionaryCreateEntry(CTAllocator * restrict alloc)
{
    struct CTDictionaryEntry * retVal = CTAllocatorAllocate(alloc, sizeof(struct CTDictionaryEntry));
    retVal->key = NULL;
    retVal->value = NULL;
    return retVal;
}

CTDictionary * CTDictionaryCreate(CTAllocator * alloc)
{
    CTDictionary * dict = CTAllocatorAllocate(alloc, sizeof(CTDictionary));
    dict->count = 0;
    dict->elements = NULL;
    dict->alloc = alloc;
    return dict;
}

void CTDictionaryRelease(CTDictionary * dict)
{
	for (uint64_t i = 0; i < dict->count; i++)
    {
        CTStringRelease(dict->elements[i]->key);
        CTStringRelease(dict->elements[i]->value);
    }
	CTAllocatorDeallocate(dict->alloc, dict->elements);
	CTAllocatorDeallocate(dict->alloc, dict);
}

void CTDictionaryAddEntriesFromQueryString(CTDictionary * restrict dict, const char * restrict query)
{
    if (query)
    {
        CTAllocator * alloc = CTAllocatorCreate();
        
        char * queryCopy = stringDuplicate(alloc, query);
        
        char str[0x1000];
        char val[0x1000];
        
        char * pointer = strtok(queryCopy, "&");
        if (pointer)
        {
            while (pointer)
            {
                str[0] = 0;
                val[0] = 0;
                memset(str, 0, sizeof(str));
                memset(val, 0, sizeof(val));
                sscanf(pointer, "%[^=]=%s", str, val);
                CTDictionaryAddEntry(dict, str, val);
                pointer = strtok(NULL, "&");
            }
        }
        CTAllocatorRelease(alloc);
    }
}

void CTDictionaryAddEntry(CTDictionary * restrict dict, const char * restrict key, const char * restrict value)
{
    unsigned long index = dict->count++;
    
	assert((dict->elements = CTAllocatorReallocate(dict->alloc, dict->elements, sizeof(struct CTDictionaryEntry *) * dict->count)));
    dict->elements[index] = CTDictionaryCreateEntry(dict->alloc);
	dict->elements[index]->key = CTStringCreate(dict->alloc, key);
    dict->elements[index]->value = CTStringCreate(dict->alloc, value);
}

void CTDictionaryDeleteEntry(CTDictionary * restrict dict, const char * restrict key)
{
    if (dict->count)
	{
		int countOfKeys = 0;
		for (unsigned long i = 0; i < dict->count; i++)
		{
			if (!strcmp(dict->elements[i]->key->characters, key))
			{
				++countOfKeys;
			}
		}
		if (countOfKeys)
		{
			struct CTDictionaryEntry ** retVal = CTAllocatorAllocate(dict->alloc, sizeof(struct CTDictionaryEntry *) * dict->count - countOfKeys);
			for (unsigned long i = 0, count = 0; i < dict->count; i++)
			{
				if (strcmp(dict->elements[i]->key->characters, key))
				{
					retVal[count++] = dict->elements[i];
				}
                else
                {
                    CTAllocatorDeallocate(dict->alloc, dict->elements[i]);
                }
			}
            CTAllocatorDeallocate(dict->alloc, dict->elements);
			dict->elements = retVal;
            dict->count = dict->count - countOfKeys;
		}
	}
}

const CTString * CTDictionaryValueForKey(const CTDictionary * restrict dict, const char * restrict key)
{
    for (unsigned long i = 0; i < dict->count; i++)
    {
        if (!strcmp(dict->elements[i]->key->characters, key))
        {
            return dict->elements[i]->value;
        }
    }
    return NULL;
}

unsigned long CTDictionaryIndexOfEntry(const CTDictionary * restrict dict, const char * restrict key)
{
    for (unsigned long i = 0; i < dict->count; i++)
    {
        if (!strcmp(dict->elements[i]->key->characters, key))
        {
            return i;
        }
    }
    return -1;
}