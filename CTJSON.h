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

CTObjectRef CTJSONParse(CTAllocatorRef restrict alloc, const char * restrict JSON, CTJSONOptions options, CTErrorRef* error);
CTStringRef CTJSONSerialise(CTAllocatorRef restrict alloc, const CTObjectRef restrict JSON, CTJSONOptions options);