//
//  CTNumber.c
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTNumber.h"
#include <math.h>

CTNumberRef CTNumberCreate(CTAllocatorRef alloc)
{
	CTNumberRef number = CTAllocatorAllocate(alloc, sizeof(CTNumber));
	number->alloc = alloc;
	return number;
}

CTNumberRef CTNumberCreateWithUnsignedInt(CTAllocatorRef restrict alloc, uint32_t value)
{
    return CTNumberCreateWithUnsignedLong(alloc, value);
}

CTNumberRef CTNumberCreateWithInt(CTAllocatorRef restrict alloc, int32_t value)
{
	return CTNumberCreateWithLong(alloc, value);
}

CTNumberRef CTNumberCreateWithUnsignedLong(CTAllocatorRef restrict alloc, uint64_t value)
{
	CTNumberRef number = CTNumberCreate(alloc);
    CTNumberSetUnsignedLongValue(number, value);
    return number;
}

CTNumberRef CTNumberCreateWithLong(CTAllocatorRef restrict alloc, int64_t value)
{
	CTNumberRef number = CTNumberCreate(alloc);
    CTNumberSetLongValue(number, value);
    return number;
}

CTNumberRef CTNumberCreateWithDouble(CTAllocatorRef restrict alloc, long double value)
{
	CTNumberRef number = CTNumberCreate(alloc);
    CTNumberSetDoubleValue(number, value);
    return number;
}

CTNumberRef CTNumberCopy(CTAllocatorRef restrict alloc, CTNumberRef number)
{
	switch (number->type)
	{
		case CTNUMBER_TYPE_LONG:
			return CTNumberCreateWithLong(alloc, number->value.Long);
		case CTNUMBER_TYPE_ULONG:
			return CTNumberCreateWithUnsignedLong(alloc, number->value.ULong);
		default:
			return CTNumberCreateWithDouble(alloc, number->value.Double);
	}
}

uint8_t CTNumberCompare(const CTNumber * restrict number1, const CTNumber * restrict number2)
{
	if (number1->type == number2->type)
	{
		switch (number1->type)
		{
			case CTNUMBER_TYPE_LONG:
				return number1->value.Long == number2->value.Long;
			case CTNUMBER_TYPE_ULONG:
				return number1->value.ULong == number2->value.ULong;
			case CTNUMBER_TYPE_DOUBLE:
				return number1->value.Double == number2->value.Double;
		}
	}
	return 0;
}

uint8_t CTLargeNumberCompare(const CTLargeNumber * restrict number1, const CTLargeNumber * restrict number2)
{
	return CTNumberCompare(number1->base, number2->base) && CTNumberCompare(number1->exponent, number2->exponent);
}

CTLargeNumberRef CTLargeNumberCreate(CTAllocatorRef restrict alloc, CTNumberRef base, CTNumberRef exponent)
{
    CTLargeNumberRef lnumber = CTAllocatorAllocate(alloc, sizeof(CTLargeNumber));
	lnumber->alloc = alloc;
    lnumber->base = base;
    lnumber->exponent = exponent;
    return lnumber;
}

CTLargeNumberRef CTLargeNumberCopy(CTAllocatorRef restrict alloc, CTLargeNumberRef number)
{
	return CTLargeNumberCreate(alloc, CTNumberCopy(alloc, number->base), CTNumberCopy(alloc, number->exponent));
}

CTNumberRef CTLargeNumberBase(const CTLargeNumber * restrict number)
{
	return number->base;
}

CTNumberRef CTLargeNumberExponent(const CTLargeNumber * restrict number)
{
	return number->exponent;
}

void CTNumberRelease(CTNumberRef number)
{
    CTAllocatorDeallocate(number->alloc, number);
}

void CTLargeNumberRelease(CTLargeNumberRef lnumber)
{
	CTNumberRelease(lnumber->base);
	CTNumberRelease(lnumber->exponent);
	CTAllocatorDeallocate(lnumber->alloc, lnumber);
}

void CTNumberSetUnsignedLongValue(CTNumberRef restrict number, uint64_t value)
{
    number->value.ULong = value;
    number->type = CTNUMBER_TYPE_ULONG;
}

void CTNumberSetLongValue(CTNumberRef restrict number, int64_t value)
{
    number->value.Long = value;
    number->type = CTNUMBER_TYPE_LONG;
}

void CTNumberSetDoubleValue(CTNumberRef restrict number, long double value)
{
    number->value.Double = value;
    number->type = CTNUMBER_TYPE_DOUBLE;
}

uint32_t CTNumberUnsignedIntValue(const CTNumber * restrict number)
{
    switch (number->type)
	{
		case CTNUMBER_TYPE_LONG:
			return (uint32_t)number->value.Long;
        case CTNUMBER_TYPE_ULONG:
            return (uint32_t)number->value.ULong;
        case CTNUMBER_TYPE_DOUBLE:
            return (uint32_t)number->value.Double;
        default:
            return UINT_MAX;
    }
}

int32_t CTNumberIntValue(const CTNumber * restrict number)
{
    switch (number->type)
    {
        case CTNUMBER_TYPE_LONG:
            return (int32_t)number->value.Long;
        case CTNUMBER_TYPE_ULONG:
            return (int32_t)number->value.ULong;
        case CTNUMBER_TYPE_DOUBLE:
            return (int32_t)number->value.Double;
        default:
            return INT_MAX;
    }
}

uint64_t CTNumberUnsignedLongValue(const CTNumber * restrict number)
{
    switch (number->type)
    {
        case CTNUMBER_TYPE_ULONG:
            return number->value.ULong;
        case CTNUMBER_TYPE_LONG:
            return (uint64_t)number->value.Long;
        case CTNUMBER_TYPE_DOUBLE:
            return (uint64_t)number->value.Double;
        default:
            return ULONG_MAX;
    }
}

int64_t CTNumberLongValue(const CTNumber * restrict number)
{
    switch (number->type)
    {
        case CTNUMBER_TYPE_LONG:
            return number->value.Long;
        case CTNUMBER_TYPE_ULONG:
            return (int64_t)number->value.ULong;
        case CTNUMBER_TYPE_DOUBLE:
            return (int64_t)number->value.Double;
        default:
            return LONG_MAX;
    }
}

long double CTNumberDoubleValue(const CTNumber * restrict number)
{
    switch (number->type)
    {
        case CTNUMBER_TYPE_DOUBLE:
            return number->value.Double;
        case CTNUMBER_TYPE_LONG:
            return (long double)number->value.Long;
        case CTNUMBER_TYPE_ULONG:
            return (long double)number->value.ULong;
        default:
            return INFINITY;
    }
}

CTNUMBER_TYPE CTNumberType(const CTNumber * restrict number)
{
	return number->type;
}

CTObjectRef CTObjectWithNumber(CTAllocatorRef alloc, CTNumberRef restrict n)
{
	return CTObjectCreate(alloc, n, CTOBJECT_TYPE_NUMBER);
}

CTObjectRef CTObjectWithLargeNumber(CTAllocatorRef alloc, CTLargeNumberRef restrict n)
{
	return CTObjectCreate(alloc, n, CTOBJECT_TYPE_LARGE_NUMBER);
}