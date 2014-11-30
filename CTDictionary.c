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

CTDictionary * CTDictionaryCopy(CTAllocator * restrict alloc, CTDictionary * dict)
{
	CTDictionary * new_dict = CTDictionaryCreate(alloc);
	for (uint64_t i = 0; i < dict->count; ++i)
	{
		CTDictionaryAddEntry(new_dict, dict->elements[i]->key->characters, CTObjectCopy(alloc, dict->elements[i]->value));
	}
	return new_dict;
}

void CTDictionaryRelease(CTDictionary * dict)
{
	for (uint64_t i = 0; i < dict->count; ++i)
    {
        CTStringRelease(dict->elements[i]->key);
		CTObjectRelease(dict->elements[i]->value);
		CTAllocatorDeallocate(dict->alloc, dict->elements[i]);
    }
	CTAllocatorDeallocate(dict->alloc, dict->elements);
	CTAllocatorDeallocate(dict->alloc, dict);
}

uint8_t CTDictionaryCompare(CTDictionary * dict1, CTDictionary * dict2)
{
	if (dict1->count == dict2->count)
	{
		for (uint64_t i = 0; i < dict1->count; ++i)
		{
			CTDictionaryEntry * entry1 = CTDictionaryEntryAtIndex(dict1, i);
			CTDictionaryEntry * entry2 = NULL;
			for (uint64_t j = 0; j < dict2->count; ++j)
			{
				CTDictionaryEntry * entryTemp = CTDictionaryEntryAtIndex(dict2, j);
				if (CTStringCompare(entry1->key, entryTemp->key) == 0 && CTObjectCompare(entry1->value, entryTemp->value))
				{
					entry2 = entryTemp;
					break;
				}
			}
			
			if (!entry2) return 0;
		}
		return 1;
	}
	return 0;
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
		for (unsigned long i = 0; i < dict->count; ++i)
		{
			if (!strcmp(CTStringUTF8String(dict->elements[i]->key), key))
			{
				++countOfKeys;
			}
		}
		if (countOfKeys)
		{
			CTDictionaryEntry ** retVal = CTAllocatorAllocate(dict->alloc, sizeof(CTDictionaryEntry *) * dict->count - countOfKeys);
			for (unsigned long i = 0, count = 0; i < dict->count; ++i)
			{
				if (strcmp(CTStringUTF8String(dict->elements[i]->key), key))
				{
					retVal[count++] = dict->elements[i];
				}
                else
                {
					CTStringRelease(dict->elements[i]->key);
					CTObjectRelease(dict->elements[i]->value);
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
	uint64_t key_hash = CTStringCharHash(key);
    for (unsigned long i = 0; i < dict->count; ++i)
    {
        if (CTStringHash(dict->elements[i]->key) == key_hash)
        {
            return dict->elements[i]->value;
        }
    }
    return NULL;
}

uint64_t CTDictionaryIndexOfEntry(const CTDictionary * restrict dict, const char * restrict key)
{
    for (unsigned long i = 0; i < dict->count; ++i)
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

CTObject * CTObjectWithDictionary(CTAllocator * alloc, CTDictionary * restrict dict)
{
	return CTObjectCreate(alloc, dict, CTOBJECT_TYPE_DICTIONARY);
}