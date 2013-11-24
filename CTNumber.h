//
//  CTNumber.h
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include <stdint.h>

enum
{
    CTNumber_TYPE_UINT,
    CTNumber_TYPE_INT,
    CTNumber_TYPE_ULONG,
    CTNumber_TYPE_LONG,
    CTNumber_TYPE_DOUBLE,
};

union CTNumberValue
{
    unsigned int UInt;
    int Int;
    uint64_t ULong;
    int64_t Long;
    long double Double;
};

typedef struct
{
    CTAllocator * alloc;
    union CTNumberValue value;
    uint64_t size;
    int type;
} CTNumber;

typedef struct
{
    CTNumber * base;
    CTNumber * exponent;
} CTLargeNumber;

CTNumber * CTNumberCreateWithUnsignedInt(CTAllocator * restrict alloc, unsigned int integer);
CTNumber * CTNumberCreateWithInt(CTAllocator * restrict alloc, int integer);
CTNumber * CTNumberCreateWithUnsignedLong(CTAllocator * restrict alloc, uint64_t longInteger);
CTNumber * CTNumberCreateWithLong(CTAllocator * restrict alloc, int64_t longInteger);
CTNumber * CTNumberCreateWithDouble(CTAllocator * restrict alloc, long double floatingPoint);
void CTNumberRelease(CTNumber * number);

CTLargeNumber * CTLargeNumberCreate(CTAllocator * restrict alloc, CTNumber * base, CTNumber * exponent);

void CTNumberSetUnsignedIntValue(CTNumber * restrict number, unsigned int integer);
void CTNumberSetIntValue(CTNumber * restrict number, int integer);
void CTNumberSetUnsignedLongValue(CTNumber * restrict number, uint64_t longInteger);
void CTNumberSetLongValue(CTNumber * restrict number, int64_t longInteger);
void CTNumberSetDoubleValue(CTNumber * restrict number, long double floatingPoint);