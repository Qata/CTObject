//
//  CTJSON.c
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTJSON.h"
#include "CTFunctions.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

CTJSONObject * CTJSONObjectFromJSONObject(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end);
CTString * CTStringFromJSON(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end);
CTObject * CTObjectFromJSON(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, int * valueType);

CTJSONObject * CTJSONObjectCreate(CTAllocator * alloc)
{
    CTJSONObject * object = CTAllocatorAllocate(alloc, sizeof(CTJSONObject));
    object->count = 0;
    object->elements = NULL;
    object->alloc = alloc;
    return object;
}

CTJSONObjectKeyValuePair * CTJSONObjectCreateEntry(CTAllocator * alloc)
{
    CTJSONObjectKeyValuePair * retVal = CTAllocatorAllocate(alloc, sizeof(CTJSONObjectKeyValuePair));
    retVal->key = NULL;
    retVal->value = NULL;
    return retVal;
}

void CTJSONObjectAddKeyValuePair(CTJSONObject * object, CTString * key, CTObject * value, int valueType)
{
    unsigned long index = object->count++;
    
	assert((object->elements = CTAllocatorReallocate(object->alloc, object->elements, sizeof(struct CTJSONObjectKeyValuePair *) * object->count)));
    object->elements[index] = CTJSONObjectCreateEntry(object->alloc);
	object->elements[index]->key = key;
    object->elements[index]->value = value;
    object->elements[index]->valueType = valueType;
}

CTJSONArray * CTJSONArrayCreate(CTAllocator * alloc)
{
    CTJSONArray * array = CTAllocatorAllocate(alloc, sizeof(CTJSONObject));
    array->count = 0;
    array->elements = NULL;
    array->alloc = alloc;
    return array;
}

CTJSONArrayValueContainer * CTJSONArrayCreateEntry(CTAllocator * alloc)
{
    CTJSONArrayValueContainer * retVal = CTAllocatorAllocate(alloc, sizeof(CTJSONArrayValueContainer));
    retVal->value = NULL;
    return retVal;
}

void CTJSONArrayAddValueContainer(CTJSONArray * array, CTObject * value, int valueType)
{
    unsigned long index = array->count++;
    
	assert((array->elements = CTAllocatorReallocate(array->alloc, array->elements, sizeof(struct CTJSONArrayValueContainer *) * array->count)));
    array->elements[index] = CTJSONArrayCreateEntry(array->alloc);
    array->elements[index]->value = value;
    array->elements[index]->valueType = valueType;
}

CTJSONObject * CTJSONParse(CTAllocator * alloc, const char * JSON)
{
    CTJSONObject * object = CTJSONObjectCreate(alloc);
    CTString * JSONString = CTStringCreate(alloc, JSON);
    char lastChar = 0;
    if (JSONString && JSONString->length)
    {
        switch (JSONString->characters[0])
        {
            case ' ':
                sscanf(JSONString->characters, "%*[ ]%c", &lastChar);
                break;
            case '{':
                sscanf(JSONString->characters, "%c", &lastChar);
                break;
        }
        if (lastChar == '{')
        {
            unsigned long end = 0;
            object = CTJSONObjectFromJSONObject(alloc, JSONString, strchr(JSONString->characters, '{') - JSONString->characters, &end);
        }
    }
    return lastChar == '{' ? object : NULL;
}

CTJSONObject * CTJSONObjectFromJSONObject(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end)
{
    CTJSONObject * object = CTJSONObjectCreate(alloc);
    if (JSON->characters[start++] == '{')
    {
        for (; start < JSON->length; start++)
        {
            switch (JSON->characters[start])
            {
                case '}':
                    *end = start + 1;
                    return object;
                    break;
                case '"':
                {
                    CTString * string = CTStringFromJSON(alloc, JSON, start, &start);
                    if (JSON->characters[start] == ':')
                    {
                        int type = 0;
                        CTJSONObjectAddKeyValuePair(object, string, CTObjectFromJSON(alloc, JSON, start + 1, &start, &type), type);
                        unsigned long startcopy;
                        for (startcopy = start; startcopy < JSON->length && JSON->characters[startcopy] != ','; startcopy++)
                        {
                            switch (JSON->characters[startcopy])
                            {
                                case ' ':
                                    break;
                                case '}':
                                    *end = startcopy + 1;
                                    return object;
                                    break;
                                default:
                                    printf("Incorrectly formatted JSON: %s, parsing anyway. You monster.\n", &JSON->characters[startcopy]);
                                    break;
                            }
                        }
                    }
                    else
                    {
                        CTAllocatorDeallocate(alloc, string);
                    }
                    break;
                }
                case ' ':
                    break;
                default:
                    printf("Extraneous '%c' in JSON: '%s'\n", JSON->characters[start], &JSON->characters[start]);
                    return object;
                    break;
            }
        }
    }
    *end = start;
    return object;
}

CTJSONArray * CTJSONArrayFromJSON(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end)
{
    CTJSONArray * array = CTJSONArrayCreate(alloc);
    int type;
    if (JSON->characters[start++] == '[')
    {
        assert(end);
        for (; JSON->characters[start] != ']' && JSON->characters[start] != 0; start++)
        {
            switch (JSON->characters[start])
            {
                case ' ':
                    break;
                case ']':
                    *end = start + 1;
                    return array;
                    break;
                default:
                    CTJSONArrayAddValueContainer(array, CTObjectFromJSON(alloc, JSON, start, &start, &type), type);
                    unsigned long startcopy;
                    for (startcopy = start; startcopy < JSON->length && JSON->characters[startcopy] != ','; startcopy++)
                    {
                        switch (JSON->characters[startcopy])
                        {
                            case ' ':
                                break;
                            case ']':
                                *end = startcopy + 1;
                                return array;
                                break;
                            default:
                                printf("Incorrectly formatted JSON: %s, parsing anyway. You monster.\n", &JSON->characters[startcopy]);
                                break;
                        }
                    }
                    break;
            }
        }
        *end = start + 1;
    }
    return array;
}

CTString * CTStringFromJSON(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end)
{
    CTString * string = CTStringCreate(alloc, "");
    if (JSON->characters[start++] == '"')
    {
        assert(end);
        for (; JSON->characters[start] != '"' && JSON->characters[start] != 0; start++)
        {
            char character[3];
            memset(character, 0, sizeof(character));
            switch (JSON->characters[start])
            {
                case '\\':
                    character[0] = JSON->characters[start];
                    character[1] = JSON->characters[++start];
                    CTStringAppendCharacters(string, character);
                    break;
                default:
                    character[0] = JSON->characters[start];
                    CTStringAppendCharacters(string, character);
                    break;
            }
        }
        *end = start + 1;
    }
    return string;
}

CTObject * CTObjectFromJSON(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, int * valueType)
{
    CTObject * object = NULL;
    assert(end && valueType);
    int loop = 1;
    while (loop)
    {
        loop = 0;
        switch (JSON->characters[start])
        {
            case '"':
                object = CTObjectCreate(alloc, CTStringFromJSON(alloc, JSON, start, &start), sizeof(CTString));
                *valueType = CTJSON_TYPE_STRING;
                break;
            case '{':
                object = CTObjectCreate(alloc, CTJSONObjectFromJSONObject(alloc, JSON, start, &start), sizeof(CTJSONObject));
                *valueType = CTJSON_TYPE_OBJECT;
                break;
            case '[':
                object = CTObjectCreate(alloc, CTJSONArrayFromJSON(alloc, JSON, start, &start), sizeof(CTJSONArray));
                *valueType = CTJSON_TYPE_ARRAY;
                break;
            case 't':
            case 'f':
            case 'n':
                if (JSON->length - start >= 4)
                {
                    if (!strncmp("true", JSON->characters + start, 4))
                    {
                        start += strlen("true");
                        object = CTObjectCreate(alloc, CTNumberCreateWithInt(alloc, 1), sizeof(CTNumber));
                        *valueType = CTJSON_TYPE_BOOLEAN;
                    }
                    else if (!strncmp("null", JSON->characters + start, 4))
                    {
                        start += strlen("null");
                        object = CTObjectCreate(alloc, CTNullCreate(alloc), sizeof(CTNumber));
                        *valueType = CTJSON_TYPE_NULL;
                    }
                    else if (JSON->length - start > 4 && !strncmp("false", JSON->characters + start, 5))
                    {
                        start += strlen("false");
                        object = CTObjectCreate(alloc, CTNumberCreateWithInt(alloc, 0), sizeof(CTNumber));
                        *valueType = CTJSON_TYPE_BOOLEAN;
                    }
                }
                break;
                
            default:
                if (JSON->characters[start] == '-' || (JSON->characters[start] >= '0' && JSON->characters[start] <= '9'))
                {
                    CTAllocator * allocl = CTAllocatorCreate();
                    CTString * numberString = CTStringCreate(allocl, "");
                    char * pEnd;
                    
                    unsigned long startcpy = start;
                    
                    while (startcpy < JSON->length && ((JSON->characters[startcpy] >= '0' && JSON->characters[startcpy] <= '9') || JSON->characters[startcpy] == '.'))
                    {
                        CTStringAppendCharacter(numberString, JSON->characters[startcpy++]);
                    }
                    
                    double Double = strtod(numberString->characters, &pEnd);
                    if (pEnd != JSON->characters && pEnd)
                    {
                        object = CTObjectCreate(alloc, CTNumberCreateWithDouble(alloc, Double), sizeof(CTNumber));
                    }
                    else
                    {
                        object = CTObjectCreate(alloc, CTNumberCreateWithLong(alloc, strtol(numberString->characters, &pEnd, 0)), sizeof(CTNumber));
                    }
                    *valueType = CTJSON_TYPE_NUMBER;
                    start += numberString->length;
                    CTAllocatorRelease(allocl);
                }
                else
                {
                    puts("Incorrectly formatted JSON literal");
                }
                break;
                
            case ' ':
                loop = 1;
                break;
            case 0:
                break;
        }
    }
    *end = start;
    return object;
}