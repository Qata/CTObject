//
//  CTNumber.c
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTNumber.h"

CTNumber * CTNumberCreate(CTAllocator * alloc)
{
	CTNumber * number = CTAllocatorAllocate(alloc, sizeof(CTNumber));
	number->alloc = alloc;
	return number;
}

CTNumber * CTNumberCreateWithUnsignedInt(CTAllocator * restrict alloc, uint32_t value)
{
	CTNumber * number = CTNumberCreate(alloc);
    CTNumberSetUnsignedIntValue(number, value);
    return number;
}

CTNumber * CTNumberCreateWithInt(CTAllocator * restrict alloc, int32_t value)
{
	CTNumber * number = CTNumberCreate(alloc);
    CTNumberSetIntValue(number, value);
    return number;
}

CTNumber * CTNumberCreateWithUnsignedLong(CTAllocator * restrict alloc, uint64_t value)
{
	CTNumber * number = CTNumberCreate(alloc);
    CTNumberSetUnsignedLongValue(number, value);
    return number;
}

CTNumber * CTNumberCreateWithLong(CTAllocator * restrict alloc, int64_t value)
{
	CTNumber * number = CTNumberCreate(alloc);
    CTNumberSetLongValue(number, value);
    return number;
}

CTNumber * CTNumberCreateWithDouble(CTAllocator * restrict alloc, long double value)
{
	CTNumber * number = CTNumberCreate(alloc);
    CTNumberSetDoubleValue(number, value);
    return number;
}

uint8_t CTNumberCompare(const CTNumber * restrict number1, const CTNumber * restrict number2)
{
	if (number1->type == number2->type)
	{
		switch (number1->type) {
			case CTNUMBER_TYPE_INT:
				return number1->value.Int == number2->value.Int;
			case CTNUMBER_TYPE_UINT:
				return number1->value.UInt == number2->value.UInt;
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

CTLargeNumber * CTLargeNumberCreate(CTAllocator * restrict alloc, CTNumber * base, CTNumber * exponent)
{
    CTLargeNumber * lnumber = CTAllocatorAllocate(alloc, sizeof(CTLargeNumber));
	lnumber->alloc = alloc;
    lnumber->base = base;
    lnumber->exponent = exponent;
    return lnumber;
}

CTNumber * CTLargeNumberBase(const CTLargeNumber * restrict number)
{
	return number->base;
}

CTNumber * CTLargeNumberExponent(const CTLargeNumber * restrict number)
{
	return number->exponent;
}

void CTNumberRelease(CTNumber * number)
{
    CTAllocatorDeallocate(number->alloc, number);
}

void CTNumberSetUnsignedIntValue(CTNumber * restrict number, uint32_t value)
{
    number->value.UInt = value;
    number->type = CTNUMBER_TYPE_UINT;
}

void CTNumberSetIntValue(CTNumber * restrict number, int32_t value)
{
    number->value.Int = value;
    number->type = CTNUMBER_TYPE_INT;
}

void CTNumberSetUnsignedLongValue(CTNumber * restrict number, uint64_t value)
{
    number->value.ULong = value;
    number->type = CTNUMBER_TYPE_ULONG;
}

void CTNumberSetLongValue(CTNumber * restrict number, int64_t value)
{
    number->value.Long = value;
    number->type = CTNUMBER_TYPE_LONG;
}

void CTNumberSetDoubleValue(CTNumber * restrict number, long double value)
{
    number->value.Double = value;
    number->type = CTNUMBER_TYPE_DOUBLE;
}

uint32_t CTNumberUnsignedIntValue(const CTNumber * restrict number)
{
    switch (number->type)
    {
        case CTNUMBER_TYPE_UINT:
            return number->value.UInt;
        case CTNUMBER_TYPE_INT:
            return (uint32_t)number->value.Int;
        case CTNUMBER_TYPE_LONG:
            return (uint32_t)number->value.Long;
        case CTNUMBER_TYPE_ULONG:
            return (uint32_t)number->value.ULong;
        case CTNUMBER_TYPE_DOUBLE:
            return (uint32_t)number->value.Double;
        default:
            return 0;
    }
}

int32_t CTNumberIntValue(const CTNumber * restrict number)
{
    switch (number->type)
    {
        case CTNUMBER_TYPE_INT:
            return number->value.Int;
        case CTNUMBER_TYPE_UINT:
            return (int32_t)number->value.UInt;
        case CTNUMBER_TYPE_LONG:
            return (int32_t)number->value.Long;
        case CTNUMBER_TYPE_ULONG:
            return (int32_t)number->value.ULong;
        case CTNUMBER_TYPE_DOUBLE:
            return (int32_t)number->value.Double;
        default:
            return 0;
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
        case CTNUMBER_TYPE_INT:
            return (uint64_t)number->value.Int;
        case CTNUMBER_TYPE_UINT:
            return (uint64_t)number->value.UInt;
        case CTNUMBER_TYPE_DOUBLE:
            return (uint64_t)number->value.Double;
        default:
            return 0;
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
        case CTNUMBER_TYPE_INT:
            return (int64_t)number->value.Int;
        case CTNUMBER_TYPE_UINT:
            return (int64_t)number->value.UInt;
        case CTNUMBER_TYPE_DOUBLE:
            return (int64_t)number->value.Double;
        default:
            return 0;
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
        case CTNUMBER_TYPE_INT:
            return (long double)number->value.Int;
        case CTNUMBER_TYPE_UINT:
            return (long double)number->value.UInt;
        default:
            return 0;
    }
}

CTNUMBER_TYPE CTNumberType(const CTNumber * restrict number)
{
	return number->type;
}

CTObject * CTObjectWithNumber(CTAllocator * alloc, CTNumber * restrict n)
{
	return CTObjectCreate(alloc, n, CTOBJECT_TYPE_NUMBER);
}

CTObject * CTObjectWithLargeNumber(CTAllocator * alloc, CTLargeNumber * restrict n)
{
	return CTObjectCreate(alloc, n, CTOBJECT_TYPE_LARGE_NUMBER);
}