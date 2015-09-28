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
#include "CTNumber.h"

CTDictionaryEntryRef CTDictionaryCreateEntry(CTAllocatorRef restrict alloc)
{
    CTDictionaryEntryRef retVal = CTAllocatorAllocate(alloc, sizeof(CTDictionaryEntry));
    return retVal;
}

CTStringRef CTDictionaryEntryKey(const CTDictionaryEntry * restrict entry)
{
	return entry->key;
}

CTObjectRef CTDictionaryEntryValue(const CTDictionaryEntry * restrict entry)
{
	return entry->value;
}

CTDictionaryRef CTDictionaryCreate(CTAllocatorRef alloc)
{
    CTDictionaryRef dict = CTAllocatorAllocate(alloc, sizeof(CTDictionary));
    dict->alloc = alloc;
    return dict;
}

CTDictionaryRef CTDictionaryCreateWithKeysPairedWithValues(CTAllocatorRef restrict alloc, ...)
{
	CTDictionaryRef retVal = CTDictionaryCreate(alloc);
	va_list list;
	va_start(list, alloc);
	for (const char * key = va_arg(list, const char*); key != NULL; key = va_arg(list, const char*))
	{
		CTObjectRef value = va_arg(list, CTObject*);
		if (!value)
			break;
		CTDictionaryAddEntry(retVal, key, value);
	}
	va_end(list);
	return retVal;
}

CTDictionaryRef CTDictionaryCreateWithKeysPairedWithNumbers(CTAllocatorRef restrict alloc, ...)
{
	CTDictionaryRef retVal = CTDictionaryCreate(alloc);
	va_list list;
	va_start(list, alloc);
	for (const char * key = va_arg(list, const char*); key != NULL; key = va_arg(list, const char*))
	{
		CTNumberRef value = va_arg(list, CTNumberRef);
		if (!value)
			break;
		CTDictionaryAddEntry(retVal, key, CTObjectWithNumber(alloc, value));
	}
	va_end(list);
	return retVal;
}

CTDictionaryRef CTDictionaryCopy(CTAllocatorRef restrict alloc, CTDictionaryRef dict)
{
	CTDictionaryRef new_dict = CTDictionaryCreate(alloc);
	for (uint64_t i = 0; i < dict->count; ++i)
	{
		CTDictionaryAddEntry(new_dict, dict->elements[i]->key->characters, CTObjectCopy(alloc, dict->elements[i]->value));
	}
	return new_dict;
}

void CTDictionaryRelease(CTDictionaryRef dict)
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

uint8_t CTDictionaryCompare(CTDictionaryRef dict1, CTDictionaryRef dict2)
{
	if (dict1->count == dict2->count)
	{
		for (uint64_t i = 0; i < dict1->count; ++i)
		{
			CTDictionaryEntryRef entry1 = CTDictionaryEntryAtIndex(dict1, i);
			CTDictionaryEntryRef entry2 = NULL;
			for (uint64_t j = 0; j < dict2->count; ++j)
			{
				CTDictionaryEntryRef entryTemp = CTDictionaryEntryAtIndex(dict2, j);
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

void CTDictionaryAddEntriesFromQueryString(CTDictionaryRef restrict dict, const char * restrict query)
{
    if (query)
    {
        CTAllocatorRef alloc = CTAllocatorCreate();
        
        char * queryCopy = stringDuplicate(alloc, query);
        
        char str[0x1000];
        char val[0x1000];
        
        char * pointer = strtok(queryCopy, "&");
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
        CTAllocatorRelease(alloc);
    }
}

void CTDictionaryAddEntry(CTDictionaryRef restrict dict, const char * restrict key, CTObjectRef restrict value)
{
    CTDictionaryAddEntry2(dict, CTStringCreate(dict->alloc, key), value);
}

void CTDictionaryAddEntry2(CTDictionaryRef restrict dict, CTStringRef restrict key, CTObjectRef restrict value)
{
    uint64_t index = dict->count++;
	assert((dict->elements = CTAllocatorReallocate(dict->alloc, dict->elements, sizeof(CTDictionaryEntryRef) * dict->count)));
    dict->elements[index] = CTDictionaryCreateEntry(dict->alloc);
	dict->elements[index]->key = key;
    dict->elements[index]->value = value;
}

void CTDictionaryDeleteEntry(CTDictionaryRef restrict dict, const char * restrict key)
{
    if (dict->count)
	{
		int countOfKeys = 0;
		for (uint64_t i = 0; i < dict->count; ++i)
		{
			if (!strcmp(CTStringUTF8String(dict->elements[i]->key), key))
			{
				++countOfKeys;
			}
		}
		if (countOfKeys)
		{
			CTDictionaryEntryRef* retVal = CTAllocatorAllocate(dict->alloc, sizeof(CTDictionaryEntryRef) * dict->count - countOfKeys);
			for (uint64_t i = 0, count = 0; i < dict->count; ++i)
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

CTDictionaryEntryRef CTDictionaryEntryAtIndex(const CTDictionary * restrict dict, uint64_t index)
{
	if (index < dict->count)
	{
		return dict->elements[index];
	}
	return NULL;
}

CTObjectRef CTDictionaryObjectForKey(const CTDictionary * restrict dict, const char * restrict key)
{
	hash_t key_hash = CTStringCharHash(key);
    for (uint64_t i = 0; i < dict->count; ++i)
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
    for (uint64_t i = 0; i < dict->count; ++i)
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

CTObjectRef CTObjectWithDictionary(CTAllocatorRef alloc, CTDictionaryRef restrict dict)
{
	return CTObjectCreate(alloc, dict, CTOBJECT_TYPE_DICTIONARY);
}