//
//  Dictionary.h
//  Lefty
//
//  Created by Carlo Tortorella on 17/09/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//
#pragma once
#include "CTAllocator.h"

typedef struct
{
	char * key;
	char * value;
} CTDictionaryEntry;

typedef struct
{
    long count;
    CTDictionaryEntry ** elements;
    CTAllocator * alloc;
} CTDictionary;

CTDictionary * CTDictionaryCreate(CTAllocator * alloc);

void CTDictionaryAddEntriesFromQueryString(CTDictionary * dict, const char * restrict query);

void CTDictionaryAddEntry(CTDictionary * dict, const char * restrict key, const char * restrict value);
void CTDictionaryDeleteEntry(CTDictionary * dict, const char * restrict key);
const char * CTDictionaryValueForKey(const CTDictionary * restrict dict, const char * restrict key);
unsigned long CTDictionaryIndexOfEntry(const CTDictionary * restrict dict, const char * restrict key);