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
    CTAllocator * alloc;
    uint64_t count;
    CTDictionaryEntry ** elements;
} CTDictionary;

CTDictionary * CTDictionaryCreate(CTAllocator * restrict alloc);
CTDictionary * CTDictionaryCreateWithKeysPairedWithValues(CTAllocator * restrict alloc, ...);
void CTDictionaryRelease(CTDictionary * dict);

void CTDictionaryAddEntriesFromQueryString(CTDictionary * restrict dict, const char * restrict query);

void CTDictionaryAddEntry(CTDictionary * restrict dict, const char * restrict key, CTObject * restrict value);
void CTDictionaryAddEntry2(CTDictionary * restrict dict, CTString * restrict key, CTObject * restrict value);
void CTDictionaryDeleteEntry(CTDictionary * restrict dict, const char * restrict key);
CTObject * CTDictionaryValueForKey(const CTDictionary * restrict dict, const char * restrict key);
uint64_t CTDictionaryIndexOfEntry(const CTDictionary * restrict dict, const char * restrict key);

/**
 * Return a CTObject encasing the CTDictionary passed.
 * @param dict	A properly initialised CTDictionary that was created with CTDictionaryCreate*.
 * @return		The CTDictionary wrapped in a CTObject. The result is identical to using CTObjectCreate.
 **/
CTObject * CTObjectWithDictionary(CTDictionary * restrict dict);