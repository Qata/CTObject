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
#include <math.h>

CTJSONObject * CTJSONObjectFromJSONObject(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, CTError ** error);
CTString * CTStringFromJSON(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, CTError ** error);
CTObject * CTObjectFromJSON(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, int * valueType, CTError ** error);
CTString * CTJSONSerialiseRecursive(CTAllocator * alloc, void * obj, int type, CTError ** error);

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

CTJSONObject * CTJSONParse(CTAllocator * alloc, const char * JSON, CTError ** error)
{
    CTJSONObject * object = CTJSONObjectCreate(alloc);
    CTString * JSONString = CTStringCreate(alloc, JSON);
    char lastChar = 0;
    if (JSONString && JSONString->length)
    {
		int loop = 1;
		int index = 0;
		while (loop && index < JSONString->length)
		{
			loop = 0;
			switch (JSONString->characters[index++])
			{
				case ' ':
				case '\r':
				case '\n':
					loop = 1;
					break;
				case '{':
					lastChar = '{';
					break;
			}
		}
        if (lastChar == '{')
        {
            unsigned long end = 0;
            object = CTJSONObjectFromJSONObject(alloc, JSONString, strchr(JSONString->characters, '{') - JSONString->characters, &end, error);
        }
        else
        {
            *error = CTErrorCreate(alloc, "Valid JSON needs to be within the scope of a root JSON object.", 0);
        }
    }
	else
	{
		*error = CTErrorCreate(alloc, "The passed string can neither be NULL nor empty.", 0);
	}
    return error ? object : NULL;
}

CTJSONObject * CTJSONObjectFromJSONObject(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, CTError ** error)
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
                    CTString * string = CTStringFromJSON(alloc, JSON, start, &start, error);
                    while (start < JSON->length && JSON->characters[start] == ' ') ++start;
                    if (JSON->characters[start] == ':')
                    {
                        int type = 0;
                        CTJSONObjectAddKeyValuePair(object, string, CTObjectFromJSON(alloc, JSON, start + 1, &start, &type, error), type);
                        unsigned long startcopy;
                        for (startcopy = start; startcopy < JSON->length && JSON->characters[startcopy] != ','; startcopy++)
                        {
                            switch (JSON->characters[startcopy])
                            {
                                case ' ':
                                case '\r':
                                case '\n':
                                    break;
                                case '}':
                                    *end = startcopy + 1;
                                    return object;
                                    break;
                                default:
                                {
                                    char str[0x100];
                                    memset(str, 0, 0x100);
                                    sprintf(str, "Extraneous character '%c' in JSON at index %lu", JSON->characters[startcopy], startcopy);
                                    *error = CTErrorCreate(alloc, str, 0);
                                    *end = startcopy + 1;
                                    return object;
                                    break;
                                }
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
				case '\r':
				case '\n':
                    break;
                case '{':
                case '[':
                {
                    char str[0x100];
                    memset(str, 0, 0x100);
                    sprintf(str, "Extraneous character '%c' in JSON: %s. Neither arrays nor objects can be keys.", JSON->characters[start], &JSON->characters[start]);
                    *error = CTErrorCreate(alloc, str, 0);
                    return object;
                    break;
                }
                default:
                {
                    char str[0x100];
                    memset(str, 0, 0x100);
                    sprintf(str, "Extraneous character '%c' in JSON: %s.", JSON->characters[start], &JSON->characters[start]);
                    *error = CTErrorCreate(alloc, str, 0);
                    return object;
                    break;
                }
            }
        }
    }
    *end = start;
    return object;
}

CTJSONArray * CTJSONArrayFromJSON(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, CTError ** error)
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
				case '\r':
				case '\n':
                    break;
                case ']':
                    *end = start + 1;
                    return array;
                    break;
                default:
                    CTJSONArrayAddValueContainer(array, CTObjectFromJSON(alloc, JSON, start, &start, &type, error), type);
                    unsigned long startcopy;
                    for (startcopy = start; startcopy < JSON->length && JSON->characters[startcopy] != ','; startcopy++)
                    {
                        switch (JSON->characters[startcopy])
                        {
							case ' ':
							case '\r':
							case '\n':
                                break;
                            case ']':
                                *end = startcopy + 1;
                                return array;
                                break;
                            default:
                            {
                                char str[0x100];
                                memset(str, 0, 0x100);
                                sprintf(str, "Extraneous character '%c' in JSON at index %lu", JSON->characters[startcopy], startcopy);
                                *error = CTErrorCreate(alloc, str, 0);
                                *end = startcopy + 1;
                                return array;
                                break;
                            }
                        }
                    }
                    break;
            }
        }
        *end = start + 1;
    }
    return array;
}

CTString * CTStringFromJSON(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, CTError ** error)
{
    CTString * string = CTStringCreate(alloc, "");
    if (JSON->characters[start] == '"')
    {
        assert(end);
        for (++start; JSON->characters[start] != '"' && JSON->characters[start] != 0; start++)
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

CTObject * CTObjectFromJSON(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, int * valueType, CTError ** error)
{
    CTObject * object = NULL;
    assert(end && valueType);
    int loop = 1;
    while (loop && start < JSON->length)
    {
        loop = 0;
        switch (JSON->characters[start])
        {
            case '"':
                object = CTObjectCreate(alloc, CTStringFromJSON(alloc, JSON, start, &start, error), sizeof(CTString));
                *valueType = CTJSON_TYPE_STRING;
                break;
            case '{':
                object = CTObjectCreate(alloc, CTJSONObjectFromJSONObject(alloc, JSON, start, &start, error), sizeof(CTJSONObject));
                *valueType = CTJSON_TYPE_OBJECT;
                break;
            case '[':
                object = CTObjectCreate(alloc, CTJSONArrayFromJSON(alloc, JSON, start, &start, error), sizeof(CTJSONArray));
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
                if (JSON->characters[start] == '-' || JSON->characters[start] == '+' || (JSON->characters[start] >= '0' && JSON->characters[start] <= '9'))
                {
                    CTAllocator * allocl = CTAllocatorCreate();
                    CTString * numberString = CTStringCreate(allocl, "");
                    CTString * exponentString = CTStringCreate(allocl, "");
                    char * pEnd;
                    
                    unsigned long startcpy = start;
                    
                    while (startcpy < JSON->length && ((JSON->characters[startcpy] >= '0' && JSON->characters[startcpy] <= '9') || JSON->characters[startcpy] == '.'))
                    {
                        CTStringAppendCharacter(numberString, JSON->characters[startcpy++]);
                    }
                    if (startcpy < JSON->length && (JSON->characters[startcpy] == 'e' || JSON->characters[startcpy] == 'E'))
                    {
                        if (startcpy < JSON->length && (JSON->characters[++startcpy] == '-' || JSON->characters[startcpy] == '+'))
                        {
                            CTStringAppendCharacter(exponentString, JSON->characters[startcpy++]);
                        }
                        
                        while (startcpy < JSON->length && ((JSON->characters[startcpy] >= '0' && JSON->characters[startcpy] <= '9')))
                        {
                            CTStringAppendCharacter(exponentString, JSON->characters[startcpy++]);
                        }
                        
                        if (startcpy < JSON->length && JSON->characters[startcpy] == '.')
                        {
                            *error = CTErrorCreate(alloc, "E notation cannot be a floating point number", 0);
                            while (startcpy < JSON->length && JSON->characters[startcpy] != ',' && JSON->characters[startcpy] != ']' && JSON->characters[startcpy] != '}')
                            {
                                ++startcpy;
                                ++start;
                            }
                        }
                    }
                    
                    long exponent = strtol(exponentString->characters, &pEnd, 0);
                    if (CTStringContainsString(numberString, "."))
                    {
                        *valueType = CTJSON_TYPE_DOUBLE;
                        double Double = strtod(numberString->characters, &pEnd);
                        if (pEnd && pEnd != JSON->characters)
                        {
                            if (exponentString->length)
                            {
                                if (exponent <= 15 && exponent >= -6)
                                {
                                    object = CTObjectCreate(alloc, CTNumberCreateWithDouble(alloc, Double * powl(10, exponent)), sizeof(CTNumber));
                                }
                                else
                                {
                                    *valueType = CTJSON_TYPE_LARGE_NUMBER;
                                    object = CTObjectCreate(alloc, CTLargeNumberCreate(alloc, CTNumberCreateWithDouble(alloc, Double), CTNumberCreateWithLong(alloc, exponent)), sizeof(CTLargeNumber));
                                }
                            }
                            else
                            {
                                object = CTObjectCreate(alloc, CTNumberCreateWithDouble(alloc, Double), sizeof(CTNumber));
                            }
                        }
                    }
                    else
                    {
                        *valueType = CTJSON_TYPE_LONG;
                        int64_t Long = strtoll(numberString->characters, &pEnd, 0);
                        if (exponentString->length)
                        {
                            if (exponent <= 15 && exponent >= -6)
                            {
                                object = CTObjectCreate(alloc, CTNumberCreateWithLong(alloc, Long * powl(10, exponent)), sizeof(CTNumber));
                            }
                            else
                            {
                                *valueType = CTJSON_TYPE_LARGE_NUMBER;
                                object = CTObjectCreate(alloc, CTLargeNumberCreate(alloc, CTNumberCreateWithLong(alloc, Long), CTNumberCreateWithLong(alloc, exponent)), sizeof(CTLargeNumber));
                            }
                        }
                        else
                        {
                            object = CTObjectCreate(alloc, CTNumberCreateWithLong(alloc, Long), sizeof(CTNumber));
                        }
                    }
                    if (exponentString->length)
                        start += numberString->length + exponentString->length + 1;
                    else
                        start += numberString->length;
                    CTAllocatorRelease(allocl);
                }
                else
                {
                    char str[0x100];
                    memset(str, 0, 0x100);
                    sprintf(str, "Incorrectly formatted JSON: '%c' at %lu", JSON->characters[start], start);
                    *error = CTErrorCreate(alloc, str, 0);
                }
                break;

			case ' ':
			case '\r':
			case '\n':
                loop = 1;
                ++start;
                break;
            case 0:
                break;
        }
    }
    *end = start;
    return object;
}

CTString * CTJSONSerialise(CTAllocator * alloc, CTJSONObject * JSON, CTError ** error)
{
    return CTJSONSerialiseRecursive(alloc, JSON, CTJSON_TYPE_OBJECT, error);
}

CTString * CTJSONSerialiseRecursive(CTAllocator * alloc, void * obj, int type, CTError ** error)
{
    CTString * returnString = CTStringCreate(alloc, "");
    switch (type)
    {
        case CTJSON_TYPE_OBJECT:
        {
            CTStringAppendCharacter(returnString, '{');
            CTJSONObject * object = (CTJSONObject *)obj;
            for (unsigned long i = 0; i < object->count; i++)
            {
                CTString * key = object->elements[i]->key;
                void * ptr = object->elements[i]->value->ptr;
                
                CTStringAppendCharacter(returnString, '"');
                CTStringAppendCharacters(returnString, key->characters);
                CTStringAppendCharacter(returnString, '"');
                CTStringAppendCharacter(returnString, ':');
                
                switch (object->elements[i]->valueType)
                {
                    case CTJSON_TYPE_OBJECT:
                    case CTJSON_TYPE_ARRAY:
                    {
                        CTAllocator * allocl = CTAllocatorCreate();
                        CTStringAppendCharacters(returnString, CTJSONSerialiseRecursive(allocl, ptr, object->elements[i]->valueType, error)->characters);
                        CTAllocatorRelease(allocl);
                        break;
                    }
                    case CTJSON_TYPE_STRING:
                        CTStringAppendCharacter(returnString, '"');
                        CTStringAppendCharacters(returnString, ((CTString *)ptr)->characters);
                        CTStringAppendCharacter(returnString, '"');
                        break;
                    case CTJSON_TYPE_BOOLEAN:
                        CTStringAppendCharacters(returnString, ((CTNumber *)ptr)->value.UInt ? "true" : "false");
                        break;
                    case CTJSON_TYPE_DOUBLE:
                    {
                        char * str = CTAllocatorAllocate(alloc, (unsigned)((ceil(log10(((CTNumber *)ptr)->value.Double)) + 7) * sizeof(char)));
                        sprintf(str, "%Lf", ((CTNumber *)ptr)->value.Double);
                        CTStringAppendCharacters(returnString, str);
                        CTAllocatorDeallocate(alloc, str);
                        break;
                    }
                    case CTJSON_TYPE_LONG:
                    {
                        char * str = CTAllocatorAllocate(alloc, (unsigned)((ceil(log10(((CTNumber *)ptr)->value.Long)) + 1) * sizeof(char)));
                        sprintf(str, "%lli", ((CTNumber *)ptr)->value.Long);
                        CTStringAppendCharacters(returnString, str);
                        CTAllocatorDeallocate(alloc, str);
                        break;
                    }
                    case CTJSON_TYPE_NULL:
                        CTStringAppendCharacters(returnString, ((CTNull *)ptr)->value);
                        break;
                    case CTJSON_TYPE_LARGE_NUMBER:
                    {
                        char * str = CTAllocatorAllocate(alloc, (unsigned)((ceil(log10(((CTLargeNumber *)ptr)->base->value.Double)) + 7) * sizeof(char)) + (int)((ceil(log10(((CTLargeNumber *)ptr)->exponent->value.Long)) + 1) * sizeof(char)));
                        sprintf(str, "%Lfe%lli", ((CTLargeNumber *)ptr)->base->value.Double, ((CTLargeNumber *)ptr)->exponent->value.Long);
                        CTStringAppendCharacters(returnString, str);
                        CTAllocatorDeallocate(alloc, str);
                        break;
                    }
                }
                if (i < object->count - 1)
                {
                    CTStringAppendCharacter(returnString, ',');
                }
            }
            CTStringAppendCharacter(returnString, '}');
            break;
        }
            
        case CTJSON_TYPE_ARRAY:
        {
            CTStringAppendCharacter(returnString, '[');
            CTJSONArray * array = (CTJSONArray *)obj;
            for (unsigned long i = 0; i < array->count; i++)
            {
                void * ptr = array->elements[i]->value->ptr;
                switch (array->elements[i]->valueType)
                {
                    case CTJSON_TYPE_OBJECT:
                    {
                        CTAllocator * allocl = CTAllocatorCreate();
                        CTStringAppendCharacters(returnString, CTJSONSerialiseRecursive(allocl, ptr, array->elements[i]->valueType, error)->characters);
                        CTAllocatorRelease(allocl);
                        break;
                    }
                    case CTJSON_TYPE_STRING:
                        CTStringAppendCharacter(returnString, '"');
                        CTStringAppendCharacters(returnString, ((CTString *)ptr)->characters);
                        CTStringAppendCharacter(returnString, '"');
                        break;
                    case CTJSON_TYPE_BOOLEAN:
                        CTStringAppendCharacters(returnString, ((CTNumber *)ptr)->value.UInt ? "true" : "false");
                        break;
                    case CTJSON_TYPE_DOUBLE:
                    {
                        char * str = CTAllocatorAllocate(alloc, (unsigned)((ceil(log10(((CTNumber *)ptr)->value.Double)) + 7) * sizeof(char)));
                        sprintf(str, "%Lf", ((CTNumber *)ptr)->value.Double);
                        CTStringAppendCharacters(returnString, str);
                        CTAllocatorDeallocate(alloc, str);
                        break;
                    }
                    case CTJSON_TYPE_LONG:
                    {
                        char * str = CTAllocatorAllocate(alloc, (unsigned)((ceil(log10(((CTNumber *)ptr)->value.Long)) + 1) * sizeof(char)));
                        sprintf(str, "%lli", ((CTNumber *)ptr)->value.Long);
                        CTStringAppendCharacters(returnString, str);
                        CTAllocatorDeallocate(alloc, str);
                        break;
                    }
                    case CTJSON_TYPE_NULL:
                        CTStringAppendCharacters(returnString, ((CTNull *)ptr)->value);
                        break;
                    case CTJSON_TYPE_LARGE_NUMBER:
                    {
                        char * str = CTAllocatorAllocate(alloc, (unsigned)((ceil(log10(((CTLargeNumber *)ptr)->base->value.Double)) + 7) * sizeof(char)) + (int)((ceil(log10(((CTLargeNumber *)ptr)->exponent->value.Long)) + 1) * sizeof(char)));
                        sprintf(str, "%Lfe%lli", ((CTLargeNumber *)ptr)->base->value.Double, ((CTLargeNumber *)ptr)->exponent->value.Long);
                        CTStringAppendCharacters(returnString, str);
                        CTAllocatorDeallocate(alloc, str);
                        break;
                    }
                }
            }
            CTStringAppendCharacter(returnString, ']');
            break;
        }
    }
    return returnString;
}
