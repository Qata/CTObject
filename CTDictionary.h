//
//  Dictionary.h
//  CTObject
//
//  Created by Carlo Tortorella on 17/09/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//
#pragma once
#include "CTAllocator.h"
#include "CTString.h"

struct CTDictionaryEntry
{
	CTString * key;
	CTString * value;
};

typedef struct
{
    long count;
    struct CTDictionaryEntry ** elements;
    CTAllocator * alloc;
} CTDictionary;

CTDictionary * CTDictionaryCreate(CTAllocator * restrict alloc);

void CTDictionaryAddEntriesFromQueryString(CTDictionary * restrict dict, const char * restrict query);

void CTDictionaryAddEntry(CTDictionary * restrict dict, const char * restrict key, const char * restrict value);
void CTDictionaryDeleteEntry(CTDictionary * restrict dict, const char * restrict key);
const CTString * CTDictionaryValueForKey(const CTDictionary * restrict dict, const char * restrict key);
unsigned long CTDictionaryIndexOfEntry(const CTDictionary * restrict dict, const char * restrict key);