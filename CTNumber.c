//
//  CTNumber.c
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTNumber.h"

CTNumberRef CTNumberCreate(CTAllocatorRef alloc)
{
	CTNumberRef number = CTAllocatorAllocate(alloc, sizeof(CTNumber));
	number->alloc = alloc;
	return number;
}

CTNumberRef CTNumberCreateWithUnsignedInt(CTAllocatorRef restrict alloc, uint32_t value)
{
	CTNumberRef number = CTNumberCreate(alloc);
    CTNumberSetUnsignedIntValue(number, value);
    return number;
}

CTNumberRef CTNumberCreateWithInt(CTAllocatorRef restrict alloc, int32_t value)
{
	CTNumberRef number = CTNumberCreate(alloc);
    CTNumberSetIntValue(number, value);
    return number;
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
		case CTNUMBER_TYPE_INT:
			return CTNumberCreateWithInt(alloc, number->value.Int);
		case CTNUMBER_TYPE_UINT:
			return CTNumberCreateWithUnsignedInt(alloc, number->value.UInt);
		case CTNUMBER_TYPE_LONG:
			return CTNumberCreateWithLong(alloc, number->value.Long);
		case CTNUMBER_TYPE_ULONG:
			return CTNumberCreateWithUnsignedLong(alloc, number->value.ULong);
		default:
			return CTNumberCreateWithDouble(alloc, number->value.Double);
	}
}

uint8_t CTNumberCompare(const CTNumberRef restrict number1, const CTNumberRef restrict number2)
{
	if (number1->type == number2->type)
	{
		switch (number1->type)
		{
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

uint8_t CTLargeNumberCompare(const CTLargeNumberRef restrict number1, const CTLargeNumberRef restrict number2)
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

CTNumberRef CTLargeNumberBase(const CTLargeNumberRef restrict number)
{
	return number->base;
}

CTNumberRef CTLargeNumberExponent(const CTLargeNumberRef restrict number)
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

void CTNumberSetUnsignedIntValue(CTNumberRef restrict number, uint32_t value)
{
    number->value.UInt = value;
    number->type = CTNUMBER_TYPE_UINT;
}

void CTNumberSetIntValue(CTNumberRef restrict number, int32_t value)
{
    number->value.Int = value;
    number->type = CTNUMBER_TYPE_INT;
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

uint32_t CTNumberUnsignedIntValue(const CTNumberRef restrict number)
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

int32_t CTNumberIntValue(const CTNumberRef restrict number)
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

uint64_t CTNumberUnsignedLongValue(const CTNumberRef restrict number)
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

int64_t CTNumberLongValue(const CTNumberRef restrict number)
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

long double CTNumberDoubleValue(const CTNumberRef restrict number)
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

CTNUMBER_TYPE CTNumberType(const CTNumberRef restrict number)
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