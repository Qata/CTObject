//
//  CTJSON.h
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include "CTString.h"
#include "CTObject.h"
#include "CTNumber.h"
#include "CTNull.h"

enum
{
    CTJSON_TYPE_OBJECT,
    CTJSON_TYPE_ARRAY,
    CTJSON_TYPE_STRING,
    CTJSON_TYPE_INTEGER,
    CTJSON_TYPE_BOOLEAN,
    CTJSON_TYPE_NULL
};

typedef struct
{
    CTAllocator * alloc;
    CTString * key;
    CTObject * value;
    int valueType;
} CTJSONObjectKeyValuePair;

typedef struct
{
    CTAllocator * alloc;
    CTJSONObjectKeyValuePair ** elements;
    unsigned long count;
} CTJSONObject;

typedef struct
{
    CTAllocator * alloc;
    CTObject ** elements;
    unsigned long count;
} CTJSONArray;

CTJSONObject * CTJSONParse(CTAllocator * alloc, const char * JSON);