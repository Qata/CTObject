//
//  CTNumber.c
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTNumber.h"

CTNumber * CTNumberCreateWithUnsignedInt(CTAllocator * restrict alloc, unsigned int value)
{
    CTNumber * number = CTAllocatorAllocate(alloc, sizeof(CTNumber));
    CTNumberSetUnsignedIntValue(number, value);
    return number;
}

CTNumber * CTNumberCreateWithInt(CTAllocator * restrict alloc, int value)
{
    CTNumber * number = CTAllocatorAllocate(alloc, sizeof(CTNumber));
    CTNumberSetIntValue(number, value);
    return number;
}

CTNumber * CTNumberCreateWithUnsignedLong(CTAllocator * restrict alloc, uint64_t value)
{
    CTNumber * number = CTAllocatorAllocate(alloc, sizeof(CTNumber));
    CTNumberSetUnsignedLongValue(number, value);
    return number;
}

CTNumber * CTNumberCreateWithLong(CTAllocator * restrict alloc, int64_t value)
{
    CTNumber * number = CTAllocatorAllocate(alloc, sizeof(CTNumber));
    CTNumberSetLongValue(number, value);
    return number;
}

CTNumber * CTNumberCreateWithDouble(CTAllocator * restrict alloc, long double value)
{
    CTNumber * number = CTAllocatorAllocate(alloc, sizeof(CTNumber));
    CTNumberSetDoubleValue(number, value);
    return number;
}

CTLargeNumber * CTLargeNumberCreate(CTAllocator * restrict alloc, CTNumber * base, CTNumber * exponent)
{
    CTLargeNumber * lnumber = CTAllocatorAllocate(alloc, sizeof(CTLargeNumber));
    lnumber->base = base;
    lnumber->exponent = exponent;
    return lnumber;
}

void CTNumberRelease(CTNumber * number)
{
    CTAllocatorDeallocate(number->alloc, number);
}

void CTNumberSetUnsignedIntValue(CTNumber * restrict number, unsigned int value)
{
    number->value.UInt = value;
    number->size = sizeof(value);
    number->type = CTNumber_TYPE_UINT;
}

void CTNumberSetIntValue(CTNumber * restrict number, int value)
{
    number->value.Int = value;
    number->size = sizeof(value);
    number->type = CTNumber_TYPE_INT;
}

void CTNumberSetUnsignedLongValue(CTNumber * restrict number, uint64_t value)
{
    number->value.ULong = value;
    number->size = sizeof(value);
    number->type = CTNumber_TYPE_ULONG;
}

void CTNumberSetLongValue(CTNumber * restrict number, int64_t value)
{
    number->value.Long = value;
    number->size = sizeof(value);
    number->type = CTNumber_TYPE_LONG;
}

void CTNumberSetDoubleValue(CTNumber * restrict number, long double value)
{
    number->value.Double = value;
    number->size = sizeof(value);
    number->type = CTNumber_TYPE_DOUBLE;
}