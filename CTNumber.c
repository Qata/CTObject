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

CTNumber * CTNumberCreateWithUnsignedLong(CTAllocator * restrict alloc, unsigned long value)
{
    CTNumber * number = CTAllocatorAllocate(alloc, sizeof(CTNumber));
    CTNumberSetUnsignedLongValue(number, value);
    return number;
}

CTNumber * CTNumberCreateWithLong(CTAllocator * restrict alloc, long value)
{
    CTNumber * number = CTAllocatorAllocate(alloc, sizeof(CTNumber));
    CTNumberSetUnsignedLongValue(number, value);
    return number;
}

CTNumber * CTNumberCreateWithDouble(CTAllocator * restrict alloc, long double value)
{
    CTNumber * number = CTAllocatorAllocate(alloc, sizeof(CTNumber));
    CTNumberSetDoubleValue(number, value);
    return number;
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

void CTNumberSetUnsignedLongValue(CTNumber * restrict number, unsigned long value)
{
    number->value.ULong = value;
    number->size = sizeof(value);
    number->type = CTNumber_TYPE_ULONG;
}

void CTNumberSetLongValue(CTNumber * restrict number, long value)
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