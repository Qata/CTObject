//
//  CTJSON.h
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include <stdint.h>

enum CTJSON_OPTIONS
{
	CTJSONOptionsSingleQuoteStrings = (1UL << 0)
};

enum CTJSON_ERROR_CODES
{
	CTJSON_UNEXPECTED_TOKEN = -100,
	CTJSON_STRUCTURE_OMISSION = -101
};

typedef uint64_t CTJSONOptions;

#include "CTAllocator.h"
#include "CTError.h"

CTObject * CTJSONParse(CTAllocator * alloc, const char * JSON, CTJSONOptions options, CTError ** error);
CTString * CTJSONSerialise(CTAllocator * alloc, CTObject * JSON, CTJSONOptions options, CTError ** error);