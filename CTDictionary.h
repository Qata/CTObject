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
#include "CTObject.h"

typedef struct
{
	CTString * key;
	CTObject * value;
} CTDictionaryEntry;

typedef struct
{
    uint64_t count;
    CTDictionaryEntry ** elements;
    CTAllocator * alloc;
} CTDictionary;

CTDictionary * CTDictionaryCreate(CTAllocator * restrict alloc);
void CTDictionaryRelease(CTDictionary * dict);

void CTDictionaryAddEntriesFromQueryString(CTDictionary * restrict dict, const char * restrict query);

void CTDictionaryAddEntry(CTDictionary * restrict dict, const char * restrict key, CTObject * restrict value);
void CTDictionaryAddEntry2(CTDictionary * restrict dict, CTString * restrict key, CTObject * restrict value);
void CTDictionaryDeleteEntry(CTDictionary * restrict dict, const char * restrict key);
CTObject * CTDictionaryValueForKey(const CTDictionary * restrict dict, const char * restrict key);
unsigned long CTDictionaryIndexOfEntry(const CTDictionary * restrict dict, const char * restrict key);