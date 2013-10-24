//
//  CTJSON.c
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTJSON.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

CTJSONObject * CTJSONObjectCreate(CTAllocator * alloc)
{
    CTJSONObject * object = CTAllocatorAllocate(alloc, sizeof(CTJSONObject));
    object->count = 0;
    object->elements = NULL;
    return object;
}

CTJSONObjectKeyValuePair * CTJSONObjectCreateEntry(CTAllocator * alloc)
{
    CTJSONObjectKeyValuePair * retVal = CTAllocatorAllocate(alloc, sizeof(CTJSONObjectKeyValuePair));
    retVal->key = NULL;
    retVal->value = NULL;
    return retVal;
}

void CTJSONObjectAddKeyValuePair(CTJSONObject * object, CTString * key, CTObject * value)
{
    unsigned long index = object->count++;
    
	assert((object->elements = CTAllocatorReallocate(object->alloc, object->elements, sizeof(struct CTJSONObjectKeyValuePair *) * object->count)));
    object->elements[index] = CTJSONObjectCreateEntry(object->alloc);
	object->elements[index]->key = key;
    object->elements[index]->value = value;
}

CTJSONObject * CTJSONParse(CTAllocator * alloc, const char * JSON)
{
    CTJSONObject * object = CTJSONObjectCreate(alloc);
    if (JSON && strlen(JSON))
    {
        char lastChar[2];
        lastChar[1] = 0;
        switch (JSON[0]) {
            case ' ':
                sscanf(JSON, "%*[ ]%c", lastChar);
                break;
            case '{':
                sscanf(JSON, "%c", lastChar);
                break;
            default:
                return NULL;
                break;
        }
        puts(lastChar);
        return object;
    }
    return NULL;
}

CTJSONObject * CTJSONObjectFromJSONObject(CTAllocator * alloc, const char * JSON, unsigned long start)
{
    return CTJSONObjectCreate(alloc);
}