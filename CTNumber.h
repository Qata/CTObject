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
    CTNUMBER_TYPE_UINT,
    CTNUMBER_TYPE_INT,
    CTNUMBER_TYPE_ULONG,
    CTNUMBER_TYPE_LONG,
    CTNUMBER_TYPE_DOUBLE,
};

union CTNumberValue
{
    uint32_t UInt;
    int32_t Int;
    uint64_t ULong;
    int64_t Long;
    long double Double;
};

typedef struct
{
    CTAllocator * alloc;
    union CTNumberValue value;
    uint64_t size;
    uint8_t type;
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

void CTNumberSetUnsignedIntValue(CTNumber * restrict number, uint32_t integer);
void CTNumberSetIntValue(CTNumber * restrict number, int32_t integer);
void CTNumberSetUnsignedLongValue(CTNumber * restrict number, uint64_t longInteger);
void CTNumberSetLongValue(CTNumber * restrict number, int64_t longInteger);
void CTNumberSetDoubleValue(CTNumber * restrict number, long double floatingPoint);

uint32_t CTNumberGetUnsignedIntValue(CTNumber * restrict number);
int32_t CTNumberGetIntValue(CTNumber * restrict number);
uint64_t CTNumberGetUnsignedLongValue(CTNumber * restrict number);
int64_t CTNumberGetLongValue(CTNumber * restrict number);
long double CTNumberGetDoubleValue(CTNumber * restrict number);