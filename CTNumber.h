//
//  CTNumber.h
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"

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
    unsigned long ULong;
    long Long;
    long double Double;
};

typedef struct
{
    CTAllocator * alloc;
    union CTNumberValue value;
    unsigned long size;
    int type;
} CTNumber;

CTNumber * CTNumberCreateWithUnsignedInt(CTAllocator * restrict alloc, unsigned int integer);
CTNumber * CTNumberCreateWithInt(CTAllocator * restrict alloc, int integer);
CTNumber * CTNumberCreateWithUnsignedLong(CTAllocator * restrict alloc, unsigned long longInteger);
CTNumber * CTNumberCreateWithLong(CTAllocator * restrict alloc, long longInteger);
CTNumber * CTNumberCreateWithDouble(CTAllocator * restrict alloc, long double floatingPoint);

void CTNumberSetUnsignedIntValue(CTNumber * restrict number, unsigned int integer);
void CTNumberSetIntValue(CTNumber * restrict number, int integer);
void CTNumberSetUnsignedLongValue(CTNumber * restrict number, unsigned long longInteger);
void CTNumberSetLongValue(CTNumber * restrict number, long longInteger);
void CTNumberSetDoubleValue(CTNumber * restrict number, long double floatingPoint);