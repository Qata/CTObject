//
//  CTNumber.c
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTNumber.h"

CTNumber * CTNumberCreateWithUnsignedInt(CTAllocator * restrict alloc, uint32_t value)
{
    CTNumber * number = CTAllocatorAllocate(alloc, sizeof(CTNumber));
    CTNumberSetUnsignedIntValue(number, value);
    return number;
}

CTNumber * CTNumberCreateWithInt(CTAllocator * restrict alloc, int32_t value)
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

void CTNumberSetUnsignedIntValue(CTNumber * restrict number, uint32_t value)
{
    number->value.UInt = value;
    number->size = sizeof(value);
    number->type = CTNUMBER_TYPE_UINT;
}

void CTNumberSetIntValue(CTNumber * restrict number, int32_t value)
{
    number->value.Int = value;
    number->size = sizeof(value);
    number->type = CTNUMBER_TYPE_INT;
}

void CTNumberSetUnsignedLongValue(CTNumber * restrict number, uint64_t value)
{
    number->value.ULong = value;
    number->size = sizeof(value);
    number->type = CTNUMBER_TYPE_ULONG;
}

void CTNumberSetLongValue(CTNumber * restrict number, int64_t value)
{
    number->value.Long = value;
    number->size = sizeof(value);
    number->type = CTNUMBER_TYPE_LONG;
}

void CTNumberSetDoubleValue(CTNumber * restrict number, long double value)
{
    number->value.Double = value;
    number->size = sizeof(value);
    number->type = CTNUMBER_TYPE_DOUBLE;
}

uint32_t CTNumberGetUnsignedIntValue(CTNumber * restrict number)
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

int32_t CTNumberGetIntValue(CTNumber * restrict number)
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

uint64_t CTNumberGetUnsignedLongValue(CTNumber * restrict number)
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

int64_t CTNumberGetLongValue(CTNumber * restrict number)
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

long double CTNumberGetDoubleValue(CTNumber * restrict number)
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