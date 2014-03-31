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
#include "CTDictionary.h"
#include "CTArray.h"
#include "CTNumber.h"
#include "CTNull.h"
#include "CTError.h"

CTDictionary * CTJSONParse(CTAllocator * alloc, const char * JSON, CTError ** error);
CTString * CTJSONSerialise(CTAllocator * alloc, CTDictionary * JSON, CTError ** error);