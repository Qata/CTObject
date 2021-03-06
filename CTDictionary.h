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
#include "CTArray.h"

typedef struct
{
	CTStringRef key;
	CTObjectRef value;
} CTDictionaryEntry;

typedef struct
{
    CTAllocatorRef alloc;
    uint64_t count;
    CTDictionaryEntry ** elements;
} CTDictionary, * CTDictionaryRef;

CTDictionaryRef CTDictionaryCreate(CTAllocatorRef restrict alloc);
CTDictionaryRef CTDictionaryCreateWithKeysPairedWithValues(CTAllocatorRef restrict alloc, ...);
CTDictionaryRef CTDictionaryCreateWithKeysPairedWithNumbers(CTAllocatorRef restrict alloc, ...);
CTDictionaryRef CTDictionaryCreateWithKeysPairedWithStrings(CTAllocatorRef restrict alloc, ...);
CTDictionaryRef CTDictionaryCopy(CTAllocatorRef restrict alloc, CTDictionaryRef dict);
void CTDictionaryRelease(CTDictionaryRef dict);

/**
 * Compare two CTDictionary objects
 * @param array	A properly initialised CTDictionary that was created with CTDictionaryCreate*.
 * @return		A value indicating equality, 0 = false, 1 = true.
 **/
uint8_t CTDictionaryCompare(CTDictionaryRef dict1, CTDictionaryRef dict2);

CTDictionaryEntry * CTDictionaryEntryAtIndex(const CTDictionary * restrict dict, uint64_t index);

CTStringRef CTDictionaryEntryKey(const CTDictionaryEntry * restrict entry);
CTObjectRef CTDictionaryEntryValue(const CTDictionaryEntry * restrict entry);

void CTDictionaryAddEntriesFromQueryString(CTDictionaryRef restrict dict, const char * restrict query);

void CTDictionaryAddEntry(CTDictionaryRef restrict dict, const char * restrict key, CTObjectRef restrict value);
void CTDictionaryAddEntry2(CTDictionaryRef restrict dict, CTStringRef restrict key, CTObjectRef restrict value);
void CTDictionaryDeleteEntry(CTDictionaryRef restrict dict, const char * restrict key);
CTObjectRef CTDictionaryObjectForKey(const CTDictionary * restrict dict, const char * restrict key);
uint64_t CTDictionaryIndexOfEntry(const CTDictionary * restrict dict, const char * restrict key);
uint64_t CTDictionaryCount(const CTDictionary * restrict dict);

void CTDictionaryMergeMutate(CTDictionary * restrict dest, const CTDictionary * restrict src);

CTDictionary * CTDictionaryMerge(CTAllocator * alloc, const CTDictionary * restrict src1, const CTDictionary * restrict src2);
CTObject * CTDictionaryReduce(CTObject * start, const CTDictionary * dictionary, CTObject * (^redFn)(CTObject * accumulator, const CTDictionaryEntry * entry));
CTArray * CTDictionaryMap(CTAllocator * alloc, const CTDictionary * dictionary, CTObject * (^mapFn)(const CTDictionaryEntry * entry));

/**
 * Return a CTObject encasing the CTDictionary passed.
 * @param dict	A properly initialised CTDictionary that was created with CTDictionaryCreate*.
 * @return		The CTDictionary wrapped in a CTObject. The result is identical to using CTObjectCreate.
 **/
CTObjectRef CTObjectWithDictionary(CTAllocatorRef alloc, CTDictionaryRef restrict dict);