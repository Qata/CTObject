//
//  CTArray.h
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"

#define CTObjectNonNilAndType(OBJECT, TYPE) \
	(OBJECT != NULL && CTObjectType(OBJECT) == TYPE)


typedef enum
{
    CTOBJECT_NOT_AN_OBJECT = -1,
    CTOBJECT_TYPE_DICTIONARY,
    CTOBJECT_TYPE_ARRAY,
    CTOBJECT_TYPE_STRING,
    CTOBJECT_TYPE_NUMBER,
    CTOBJECT_TYPE_NULL,
    CTOBJECT_TYPE_LARGE_NUMBER
} CTOBJECT_TYPE;

typedef struct
{
    CTAllocatorRef alloc;
    uint64_t size;
    void * ptr;
    CTOBJECT_TYPE type;
} CTObject, * CTObjectRef;

inline void * CTObjectValueIfNonNilAndType(CTObjectRef object, CTOBJECT_TYPE type);
CTObjectRef CTObjectCreate(CTAllocatorRef restrict alloc, void * ptr, CTOBJECT_TYPE type);
CTObjectRef CTObjectCopy(CTAllocatorRef restrict alloc, const CTObjectRef restrict object);
void * CTObjectValue(const CTObjectRef restrict object);
CTOBJECT_TYPE CTObjectType(const CTObjectRef restrict object);
uint64_t CTObjectSize(const CTObjectRef restrict object);
/**
 * Compare two CTObject objects
 * @param array	A properly initialised CTObject that was created with CTObjectCreate* or CTObjectWith*.
 * @return		A value indicating equality, 0 = false, 1 = true.
 **/
uint8_t CTObjectCompare(const CTObjectRef restrict object1, const CTObjectRef restrict object2);
void CTObjectRelease(CTObjectRef object);