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
#include <stdarg.h>
#include "CTDictionary.h"
#include "CTFunctions.h"

CTDictionaryEntry * CTDictionaryCreateEntry(CTAllocator * restrict alloc)
{
    CTDictionaryEntry * retVal = CTAllocatorAllocate(alloc, sizeof(CTDictionaryEntry));
    return retVal;
}

CTString * CTDictionaryEntryKey(const CTDictionaryEntry * restrict entry)
{
	return entry->key;
}

CTObject * CTDictionaryEntryValue(const CTDictionaryEntry * restrict entry)
{
	return entry->value;
}

CTDictionary * CTDictionaryCreate(CTAllocator * alloc)
{
    CTDictionary * dict = CTAllocatorAllocate(alloc, sizeof(CTDictionary));
    dict->alloc = alloc;
    return dict;
}

CTDictionary * CTDictionaryCreateWithKeysPairedWithValues(CTAllocator * restrict alloc, ...)
{
	CTDictionary * retVal = CTDictionaryCreate(alloc);
	va_list list;
	va_start(list, alloc);
	for (const char * key = va_arg(list, const char*); key != NULL; key = va_arg(list, const char*))
	{
		CTObject * value = va_arg(list, CTObject*);
		if (!value)
			break;
		CTDictionaryAddEntry(retVal, key, value);
	}
	va_end(list);
	return retVal;
}

void CTDictionaryRelease(CTDictionary * dict)
{
	for (uint64_t i = 0; i < dict->count; i++)
    {
        CTStringRelease(dict->elements[i]->key);
        CTObjectRelease(dict->elements[i]->value);
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
                CTDictionaryAddEntry(dict, str, CTObjectCreate(dict->alloc, CTStringCreate(dict->alloc, val), CTOBJECT_TYPE_STRING));
                pointer = strtok(NULL, "&");
            }
        }
        CTAllocatorRelease(alloc);
    }
}

void CTDictionaryAddEntry(CTDictionary * restrict dict, const char * restrict key, CTObject * restrict value)
{
    CTDictionaryAddEntry2(dict, CTStringCreate(dict->alloc, key), value);
}

void CTDictionaryAddEntry2(CTDictionary * restrict dict, CTString * restrict key, CTObject * restrict value)
{
    unsigned long index = dict->count++;
	assert((dict->elements = CTAllocatorReallocate(dict->alloc, dict->elements, sizeof(CTDictionaryEntry *) * dict->count)));
    dict->elements[index] = CTDictionaryCreateEntry(dict->alloc);
	dict->elements[index]->key = key;
    dict->elements[index]->value = value;
}

void CTDictionaryDeleteEntry(CTDictionary * restrict dict, const char * restrict key)
{
    if (dict->count)
	{
		int countOfKeys = 0;
		for (unsigned long i = 0; i < dict->count; i++)
		{
			if (!strcmp(CTStringUTF8String(dict->elements[i]->key), key))
			{
				++countOfKeys;
			}
		}
		if (countOfKeys)
		{
			CTDictionaryEntry ** retVal = CTAllocatorAllocate(dict->alloc, sizeof(CTDictionaryEntry *) * dict->count - countOfKeys);
			for (unsigned long i = 0, count = 0; i < dict->count; i++)
			{
				if (strcmp(CTStringUTF8String(dict->elements[i]->key), key))
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

CTDictionaryEntry * CTDictionaryEntryAtIndex(const CTDictionary * restrict dict, uint64_t index)
{
	if (index < dict->count)
	{
		return dict->elements[index];
	}
	return NULL;
}

CTObject * CTDictionaryValueForKey(const CTDictionary * restrict dict, const char * restrict key)
{
    for (unsigned long i = 0; i < dict->count; i++)
    {
        if (!strcmp(CTStringUTF8String(dict->elements[i]->key), key))
        {
            return dict->elements[i]->value;
        }
    }
    return NULL;
}

uint64_t CTDictionaryIndexOfEntry(const CTDictionary * restrict dict, const char * restrict key)
{
    for (unsigned long i = 0; i < dict->count; i++)
    {
        if (!strcmp(CTStringUTF8String(dict->elements[i]->key), key))
        {
            return i;
        }
    }
    return CT_NOT_FOUND;
}

uint64_t CTDictionaryCount(const CTDictionary * restrict dict)
{
	return dict->count;
}

CTObject * CTObjectWithDictionary(CTDictionary * restrict dict)
{
	return CTObjectCreate(dict->alloc, dict, CTOBJECT_TYPE_DICTIONARY);
}