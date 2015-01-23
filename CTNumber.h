//
//  CTNumber.h
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include "CTObject.h"
#include <stdint.h>

typedef enum
{
    CTNUMBER_TYPE_UINT,
    CTNUMBER_TYPE_INT,
    CTNUMBER_TYPE_ULONG,
    CTNUMBER_TYPE_LONG,
    CTNUMBER_TYPE_DOUBLE
} CTNUMBER_TYPE, * CTNUMBER_TYPERef;

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
    CTAllocatorRef alloc;
    union CTNumberValue value;
    CTNUMBER_TYPE type;
} CTNumber, * CTNumberRef;

typedef struct
{
    CTAllocatorRef alloc;
    CTNumberRef base;
    CTNumberRef exponent;
} CTLargeNumber, * CTLargeNumberRef;

CTNumberRef CTNumberCreateWithUnsignedInt(CTAllocatorRef restrict alloc, unsigned int integer);
CTNumberRef CTNumberCreateWithInt(CTAllocatorRef restrict alloc, int integer);
CTNumberRef CTNumberCreateWithUnsignedLong(CTAllocatorRef restrict alloc, uint64_t longInteger);
CTNumberRef CTNumberCreateWithLong(CTAllocatorRef restrict alloc, int64_t longInteger);
CTNumberRef CTNumberCreateWithDouble(CTAllocatorRef restrict alloc, long double floatingPoint);

CTNumberRef CTNumberCopy(CTAllocatorRef restrict alloc, CTNumberRef number);

/**
 * Compare two CTNumber objects
 * @param array	A properly initialised CTNumber that was created with CTNumberCreate*.
 * @return		A value indicating equality, 0 = false, 1 = true.
 **/
uint8_t CTNumberCompare(const CTNumberRef restrict number1, const CTNumberRef restrict number2);
uint8_t CTLargeNumberCompare(const CTLargeNumberRef restrict number1, const CTLargeNumberRef restrict number2);

void CTNumberRelease(CTNumberRef number);

CTLargeNumberRef CTLargeNumberCreate(CTAllocatorRef restrict alloc, CTNumberRef base, CTNumberRef exponent);
CTLargeNumberRef CTLargeNumberCopy(CTAllocatorRef restrict alloc, CTLargeNumberRef number);
CTNumberRef CTLargeNumberBase(const CTLargeNumberRef restrict number);
CTNumberRef CTLargeNumberExponent(const CTLargeNumberRef restrict number);

void CTNumberSetUnsignedIntValue(CTNumberRef restrict number, uint32_t integer);
void CTNumberSetIntValue(CTNumberRef restrict number, int32_t integer);
void CTNumberSetUnsignedLongValue(CTNumberRef restrict number, uint64_t longInteger);
void CTNumberSetLongValue(CTNumberRef restrict number, int64_t longInteger);
void CTNumberSetDoubleValue(CTNumberRef restrict number, long double floatingPoint);

uint32_t CTNumberUnsignedIntValue(const CTNumberRef restrict number);
int32_t CTNumberIntValue(const CTNumberRef restrict number);
uint64_t CTNumberUnsignedLongValue(const CTNumberRef restrict number);
int64_t CTNumberLongValue(const CTNumberRef restrict number);
long double CTNumberDoubleValue(const CTNumberRef restrict number);
CTNUMBER_TYPE CTNumberType(const CTNumberRef restrict number);

/**
 * Return a CTObject encasing the CTNumber passed.
 * @param n	A properly initialised CTNumber that was created with CTNumberCreate*.
 * @return	The CTNumber wrapped in a CTObject. The result is identical to using CTObjectCreate.
 **/
CTObjectRef CTObjectWithNumber(CTAllocatorRef alloc, CTNumberRef restrict n);
CTObjectRef CTObjectWithLargeNumber(CTAllocatorRef alloc, CTLargeNumberRef restrict n);