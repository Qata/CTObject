//
//  CTArray.c
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CTObject.h"
#include "CTDictionary.h"
#include "CTArray.h"
#include "CTNumber.h"
#include "CTString.h"
#include "CTNull.h"

CTObject * CTObjectCreate(CTAllocator * restrict alloc, void * ptr, int8_t type)
{
    CTObject * object = CTAllocatorAllocate(alloc, sizeof(CTObject));
    object->alloc = alloc;
    object->ptr = ptr;
    object->type = type;
    return object;
}

CTObject * CTObjectCreate2(CTAllocator * restrict alloc, void * ptr, int8_t type, unsigned long size)
{
    CTObject * object = CTObjectCreate(alloc, ptr, type);
    object->size = size;
    return object;
}

void CTObjectRelease(CTObject * object)
{
    switch (object->type)
    {
        case CTOBJECT_TYPE_DICTIONARY:
            CTDictionaryRelease(object->ptr);
            break;
            
        case CTOBJECT_TYPE_ARRAY:
            CTArrayRelease(object->ptr);
            break;
            
        case CTOBJECT_TYPE_NUMBER:
            CTNumberRelease(object->ptr);
            break;
            
        case CTOBJECT_TYPE_STRING:
            CTStringRelease(object->ptr);
            break;
			
		case CTOBJECT_TYPE_NULL:
			CTNullRelease(object->ptr);
			break;
    }
    
    CTAllocatorDeallocate(object->alloc, object);
}