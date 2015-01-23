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

CTNullRef CTNullCreate(CTAllocatorRef restrict alloc)
{
    CTNullRef null = CTAllocatorAllocate(alloc, sizeof(CTNull));
    null->value = "null";
    null->alloc = alloc;
    return null;
}

const char * CTNullValue(CTNullRef null)
{
	return null->value;
}

void CTNullRelease(CTNullRef null)
{
    CTAllocatorDeallocate(null->alloc, null);
}

CTObjectRef CTObjectWithNull(CTAllocatorRef alloc, CTNullRef restrict n)
{
	return CTObjectCreate(alloc, n, CTOBJECT_TYPE_NULL);
}