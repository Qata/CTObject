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
void CTJSONSerialiseRecursive(CTAllocator * alloc, CTString * JSON, void * obj, int type, CTError ** error);

CTJSONObject * CTJSONObjectCreate(CTAllocator * alloc)
{
    CTJSONObject * object = CTAllocatorAllocate(alloc, sizeof(CTJSONObject));
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
    CTJSONArray * array = CTAllocatorAllocate(alloc, sizeof(CTJSONArray));
    array->alloc = alloc;
    return array;
}

CTJSONArrayValueContainer * CTJSONArrayCreateEntry(CTAllocator * alloc)
{
    CTJSONArrayValueContainer * retVal = CTAllocatorAllocate(alloc, sizeof(CTJSONArrayValueContainer));
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
    if (JSONString && CTStringLength(JSONString))
    {
		int loop = 1;
		int index = 0;
		while (loop && index < CTStringLength(JSONString))
		{
			loop = 0;
			switch (CTStringUTF8String(JSONString)[index++])
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
            object = CTJSONObjectFromJSONObject(alloc, JSONString, strchr(CTStringUTF8String(JSONString), '{') - CTStringUTF8String(JSONString), &end, error);
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
    if (CTStringUTF8String(JSON)[start++] == '{')
    {
        for (; start < CTStringLength(JSON); start++)
        {
            switch (CTStringUTF8String(JSON)[start])
            {
                case '}':
                    *end = start + 1;
                    return object;
                    break;
                case '"':
                {
                    CTString * string = CTStringFromJSON(alloc, JSON, start, &start, error);
                    while (start < CTStringLength(JSON) && CTStringUTF8String(JSON)[start] == ' ') ++start;
                    if (CTStringUTF8String(JSON)[start] == ':')
                    {
                        int type = 0;
                        CTJSONObjectAddKeyValuePair(object, string, CTObjectFromJSON(alloc, JSON, start + 1, &start, &type, error), type);
                        unsigned long startcopy;
                        for (startcopy = start; startcopy < CTStringLength(JSON) && CTStringUTF8String(JSON)[startcopy] != ','; startcopy++)
                        {
                            switch (CTStringUTF8String(JSON)[startcopy])
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
                                    sprintf(str, "Extraneous character '%c' in JSON at index %lu", CTStringUTF8String(JSON)[startcopy], startcopy);
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
                default:
                {
                    char str[0x100];
                    memset(str, 0, 0x100);
                    sprintf(str, "Extraneous character '%c' in JSON: %s. Only strings can be used as keys.", CTStringUTF8String(JSON)[start], &CTStringUTF8String(JSON)[start]);
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
    if (CTStringUTF8String(JSON)[start++] == '[')
    {
        assert(end);
        for (; CTStringUTF8String(JSON)[start] != ']' && CTStringUTF8String(JSON)[start] != 0; start++)
        {
            switch (CTStringUTF8String(JSON)[start])
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
                    for (startcopy = start; startcopy < CTStringLength(JSON) && CTStringUTF8String(JSON)[startcopy] != ','; startcopy++)
                    {
                        switch (CTStringUTF8String(JSON)[startcopy])
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
                                sprintf(str, "Extraneous character '%c' in JSON at index %lu", CTStringUTF8String(JSON)[startcopy], startcopy);
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
    if (CTStringUTF8String(JSON)[start] == '"')
    {
        assert(end);
        CTString str;
        str.alloc = JSON->alloc;
        CTStringSet(&str, CTStringUTF8String(JSON) + start);
        str.length = CTStringLength(JSON) - start;
        if (!CTStringStringBetween(&str, "\"", "\""))
        {
            char str[0x100];
            memset(str, 0, 0x100);
            sprintf(str, "Non-terminated string in JSON at index %lu", start);
            *error = CTErrorCreate(alloc, str, 0);
            return string;
        }
        
        for (++start; CTStringUTF8String(JSON)[start] != '"' && CTStringUTF8String(JSON)[start] != 0; start++)
        {
            char character[3];
            memset(character, 0, sizeof(character));
            switch (CTStringUTF8String(JSON)[start])
            {
                case '\\':
                    character[0] = CTStringUTF8String(JSON)[start];
                    character[1] = CTStringUTF8String(JSON)[++start];
                    CTStringAppendCharacters(string, character, -1);
                    break;
                default:
                    character[0] = CTStringUTF8String(JSON)[start];
                    CTStringAppendCharacters(string, character, -1);
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
    while (loop && start < CTStringLength(JSON))
    {
        loop = 0;
        switch (CTStringUTF8String(JSON)[start])
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
                if (CTStringLength(JSON) - start >= 4)
                {
                    if (!strncmp("true", CTStringUTF8String(JSON) + start, 4))
                    {
                        start += strlen("true");
                        object = CTObjectCreate(alloc, CTNumberCreateWithInt(alloc, 1), sizeof(CTNumber));
                        *valueType = CTJSON_TYPE_BOOLEAN;
                    }
                    else if (!strncmp("null", CTStringUTF8String(JSON) + start, 4))
                    {
                        start += strlen("null");
                        object = CTObjectCreate(alloc, CTNullCreate(alloc), sizeof(CTNumber));
                        *valueType = CTJSON_TYPE_NULL;
                    }
                    else if (CTStringLength(JSON) - start > 4 && !strncmp("false", CTStringUTF8String(JSON) + start, 5))
                    {
                        start += strlen("false");
                        object = CTObjectCreate(alloc, CTNumberCreateWithInt(alloc, 0), sizeof(CTNumber));
                        *valueType = CTJSON_TYPE_BOOLEAN;
                    }
                }
                break;
                
            default:
                if (CTStringUTF8String(JSON)[start] == '-' || CTStringUTF8String(JSON)[start] == '+' || (CTStringUTF8String(JSON)[start] >= '0' && CTStringUTF8String(JSON)[start] <= '9'))
                {
                    CTAllocator * allocl = CTAllocatorCreate();
                    CTString * numberString = CTStringCreate(allocl, "");
                    CTString * exponentString = CTStringCreate(allocl, "");
                    char * pEnd;
                    
                    unsigned long startcpy = start;
                    
                    while (startcpy < CTStringLength(JSON) && ((CTStringUTF8String(JSON)[startcpy] >= '0' && CTStringUTF8String(JSON)[startcpy] <= '9') || CTStringUTF8String(JSON)[startcpy] == '.'))
                    {
                        CTStringAppendCharacter(numberString, CTStringUTF8String(JSON)[startcpy++]);
                    }
                    
                    if (startcpy < CTStringLength(JSON) && (CTStringUTF8String(JSON)[startcpy] == 'e' || CTStringUTF8String(JSON)[startcpy] == 'E'))
                    {
                        if (startcpy < CTStringLength(JSON) && (CTStringUTF8String(JSON)[++startcpy] == '-' || CTStringUTF8String(JSON)[startcpy] == '+'))
                        {
                            CTStringAppendCharacter(exponentString, CTStringUTF8String(JSON)[startcpy++]);
                        }
                        
                        while (startcpy < CTStringLength(JSON) && ((CTStringUTF8String(JSON)[startcpy] >= '0' && CTStringUTF8String(JSON)[startcpy] <= '9')))
                        {
                            CTStringAppendCharacter(exponentString, CTStringUTF8String(JSON)[startcpy++]);
                        }
                        
                        if (startcpy < CTStringLength(JSON) && CTStringUTF8String(JSON)[startcpy] == '.')
                        {
                            *error = CTErrorCreate(alloc, "E notation cannot be a floating point number", 0);
                            while (startcpy < CTStringLength(JSON) && CTStringUTF8String(JSON)[startcpy] != ',' && CTStringUTF8String(JSON)[startcpy] != ']' && CTStringUTF8String(JSON)[startcpy] != '}')
                            {
                                ++startcpy;
                                ++start;
                            }
                        }
                    }
                    
                    long exponent = strtol(CTStringUTF8String(exponentString), &pEnd, 0);
                    if (CTStringContainsString(numberString, "."))
                    {
                        *valueType = CTJSON_TYPE_DOUBLE;
                        double Double = strtod(CTStringUTF8String(numberString), &pEnd);
                        if (pEnd && pEnd != CTStringUTF8String(JSON))
                        {
                            if (CTStringLength(exponentString))
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
                        int64_t Long = strtoll(CTStringUTF8String(numberString), &pEnd, 0);
                        if (CTStringLength(exponentString))
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
                    if (CTStringLength(exponentString))
                    {
                        start += CTStringLength(numberString) + CTStringLength(exponentString) + 1;
                    }
                    else
                    {
                        start += CTStringLength(numberString);
                    }
                    CTAllocatorRelease(allocl);
                }
                else
                {
                    char str[0x100];
                    memset(str, 0, 0x100);
                    sprintf(str, "Incorrectly formatted JSON: '%c' at %lu", CTStringUTF8String(JSON)[start], start);
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
    CTString * retVal = CTStringCreate(alloc, "");
    CTJSONSerialiseRecursive(alloc, retVal, JSON, CTJSON_TYPE_OBJECT, error);
    return retVal;
}

void CTJSONSerialiseRecursive(CTAllocator * alloc, CTString * JSON, void * obj, int type, CTError ** error)
{
    CTJSONObject * object = (CTJSONObject *)obj;
    CTJSONArray * array = (CTJSONArray *)obj;
    uint64_t count = 0;
    int valueType = 0;
    void * ptr = NULL;
    
    if (type == CTJSON_TYPE_OBJECT)
    {
        count = object->count;
        CTStringAppendCharacter(JSON, '{');
    }
    else if (type == CTJSON_TYPE_ARRAY)
    {
        count = array->count;
        CTStringAppendCharacter(JSON, '[');
    }
    for (unsigned long i = 0; i < count; i++)
    {
        if (type == CTJSON_TYPE_OBJECT)
        {
            CTString * key = object->elements[i]->key;
            CTStringAppendCharacter(JSON, '"');
            CTStringAppendCharacters(JSON, CTStringUTF8String(key), -1);
            CTStringAppendCharacter(JSON, '"');
            CTStringAppendCharacter(JSON, ':');
        }
        if (type == CTJSON_TYPE_OBJECT)
        {
            ptr = object->elements[i]->value->ptr;
            valueType = object->elements[i]->valueType;
        }
        else if (type == CTJSON_TYPE_ARRAY)
        {
            ptr = array->elements[i]->value->ptr;
            valueType = array->elements[i]->valueType;
        }
        
        switch (valueType)
        {
            case CTJSON_TYPE_OBJECT:
            case CTJSON_TYPE_ARRAY:
            {
                CTJSONSerialiseRecursive(alloc, JSON, ptr, valueType, error);
                break;
            }
            case CTJSON_TYPE_STRING:
                CTStringAppendCharacter(JSON, '"');
                CTStringAppendCharacters(JSON, ((CTString *)ptr)->characters, -1);
                CTStringAppendCharacter(JSON, '"');
                break;
            case CTJSON_TYPE_BOOLEAN:
                CTStringAppendCharacters(JSON, ((CTNumber *)ptr)->value.UInt ? "true" : "false", -1);
                break;
            case CTJSON_TYPE_DOUBLE:
            {
                char * str = CTAllocatorAllocate(alloc, (unsigned)((ceil(log10(((CTNumber *)ptr)->value.Double)) + 7) * sizeof(char)));
                sprintf(str, "%Lf", ((CTNumber *)ptr)->value.Double);
                CTStringAppendCharacters(JSON, str, -1);
                CTAllocatorDeallocate(alloc, str);
                break;
            }
            case CTJSON_TYPE_LONG:
            {
                char * str = CTAllocatorAllocate(alloc, (unsigned)((ceil(log10(((CTNumber *)ptr)->value.Long)) + 1) * sizeof(char)));
                sprintf(str, "%lli", (long long)((CTNumber *)ptr)->value.Long);
                CTStringAppendCharacters(JSON, str, -1);
                CTAllocatorDeallocate(alloc, str);
                break;
            }
            case CTJSON_TYPE_NULL:
                CTStringAppendCharacters(JSON, ((CTNull *)ptr)->value, -1);
                break;
            case CTJSON_TYPE_LARGE_NUMBER:
            {
                char * str = CTAllocatorAllocate(alloc, (unsigned)((ceil(log10(((CTLargeNumber *)ptr)->base->value.Double)) + 7) * sizeof(char)) + (int)((ceil(log10(((CTLargeNumber *)ptr)->exponent->value.Long)) + 1) * sizeof(char)));
                sprintf(str, "%Lfe%lli", ((CTLargeNumber *)ptr)->base->value.Double, (long long)((CTLargeNumber *)ptr)->exponent->value.Long);
                CTStringAppendCharacters(JSON, str, -1);
                CTAllocatorDeallocate(alloc, str);
                break;
            }
        }
        if (i < count - 1)
        {
            CTStringAppendCharacter(JSON, ',');
        }
    }
    
    if (type == CTJSON_TYPE_OBJECT)
    {
        CTStringAppendCharacter(JSON, '}');
    }
    else if (type == CTJSON_TYPE_ARRAY)
    {
        CTStringAppendCharacter(JSON, ']');
    }
}
