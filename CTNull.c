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

CTNull global_null = {.value = "null"};

CTNullRef CTNullCreate()
{
    return &global_null;
}

const char * CTNullValue(CTNullRef null)
{
	return null->value;
}

CTObjectRef CTObjectWithNull(CTAllocatorRef alloc, CTNullRef restrict n)
{
	return CTObjectCreate(alloc, n, CTOBJECT_TYPE_NULL);
}