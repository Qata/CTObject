//
//  CTBencode.h
//  CTObject
//
//  Created by Carlo Tortorella on 11/12/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include "CTError.h"
#include "CTObject.h"

enum
{
    CTBENCODE_TYPE_DICTIONARY,
    CTBENCODE_TYPE_LIST,
    CTBENCODE_TYPE_STRING,
    CTBENCODE_TYPE_INTEGER
};

typedef struct
{
    CTAllocator * alloc;
    void * value;
    int8_t valueType;
} CTBencodeValueContainer;

typedef struct
{
    CTAllocator * alloc;
    CTString * key;
    CTBencodeValueContainer * value;
} CTBencodeDictionaryKeyValuePair;

typedef struct
{
    CTAllocator * alloc;
    CTBencodeDictionaryKeyValuePair ** elements;
    uint64_t count;
} CTBencodeDictionary;

typedef struct
{
    CTAllocator * alloc;
    CTBencodeValueContainer ** elements;
    uint64_t count;
} CTBencodeList;

CTBencodeValueContainer * CTBencodeParse(CTAllocator * alloc, const char * bencoded, uint64_t * start, CTError ** error);