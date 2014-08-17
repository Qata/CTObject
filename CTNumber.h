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
} CTNUMBER_TYPE;

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
    CTNUMBER_TYPE type;
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

/**
 * Compare two CTNumber objects
 * @param array	A properly initialised CTNumber that was created with CTNumberCreate*.
 * @return		A value indicating equality, 0 = false, 1 = true.
 **/
uint8_t CTNumberCompare(const CTNumber * restrict number1, const CTNumber * restrict number2);
uint8_t CTLargeNumberCompare(const CTLargeNumber * restrict number1, const CTLargeNumber * restrict number2);

void CTNumberRelease(CTNumber * number);

CTLargeNumber * CTLargeNumberCreate(CTAllocator * restrict alloc, CTNumber * base, CTNumber * exponent);
CTNumber * CTLargeNumberBase(const CTLargeNumber * restrict number);
CTNumber * CTLargeNumberExponent(const CTLargeNumber * restrict number);

void CTNumberSetUnsignedIntValue(CTNumber * restrict number, uint32_t integer);
void CTNumberSetIntValue(CTNumber * restrict number, int32_t integer);
void CTNumberSetUnsignedLongValue(CTNumber * restrict number, uint64_t longInteger);
void CTNumberSetLongValue(CTNumber * restrict number, int64_t longInteger);
void CTNumberSetDoubleValue(CTNumber * restrict number, long double floatingPoint);

uint32_t CTNumberUnsignedIntValue(const CTNumber * restrict number);
int32_t CTNumberIntValue(const CTNumber * restrict number);
uint64_t CTNumberUnsignedLongValue(const CTNumber * restrict number);
int64_t CTNumberLongValue(const CTNumber * restrict number);
long double CTNumberDoubleValue(const CTNumber * restrict number);
CTNUMBER_TYPE CTNumberType(const CTNumber * restrict number);

/**
 * Return a CTObject encasing the CTNumber passed.
 * @param n	A properly initialised CTNumber that was created with CTNumberCreate*.
 * @return	The CTNumber wrapped in a CTObject. The result is identical to using CTObjectCreate.
 **/
CTObject * CTObjectWithNumber(CTNumber * restrict n);
CTObject * CTObjectWithLargeNumber(CTLargeNumber * restrict n);