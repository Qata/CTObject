//
//  CTNumber.c
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#ifndef NULL
#define NULL ((void *) 0)
#endif
#include "CTNull.h"

CTNull * CTNullCreate(CTAllocator * restrict alloc)
{
    CTNull * null = CTAllocatorAllocate(alloc, sizeof(CTNull));
    null->value = "null";
    null->alloc = alloc;
    return null;
}

const char * CTNullValue(CTNull * null)
{
	return null->value;
}

void CTNullRelease(CTNull * null)
{
    CTAllocatorDeallocate(null->alloc, null);
}

CTObject * CTObjectWithNull(CTAllocator * alloc, CTNull * restrict n)
{
	return CTObjectCreate(alloc, n, CTOBJECT_TYPE_NULL);
}