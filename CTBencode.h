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
#include "CTArray.h"
#include "CTDictionary.h"
#include "CTObject.h"

CTObject * CTBencodeParse(CTAllocator * restrict alloc, const char * restrict bencoded, uint64_t * start, CTError ** error);
CTString * CTBencodeSerialise(CTAllocator * restrict alloc, CTObject * restrict bencoded, CTError ** error);