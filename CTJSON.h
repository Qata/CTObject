//
//  CTJSON.h
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include <stdint.h>

enum
{
	CTJSONOptionsSingleQuoteStrings = (1UL << 0)
};

typedef uint64_t CTJSONOptions;

#include "CTAllocator.h"
#include "CTError.h"

CTObject * CTJSONParse(CTAllocator * alloc, const char * JSON, CTJSONOptions options, CTError ** error);
CTString * CTJSONSerialise(CTAllocator * alloc, CTObject * JSON, CTJSONOptions options, CTError ** error);