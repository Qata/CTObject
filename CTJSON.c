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

CTDictionary * CTDictionaryFromJSONObject(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, CTError ** error);
CTString * CTStringFromJSON(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, CTError ** error);
CTObject * CTObjectFromJSON(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, CTError ** error);
void CTJSONSerialiseRecursive(CTAllocator * alloc, CTString * JSON, void * obj, int type, CTError ** error);

CTDictionary * CTJSONParse(CTAllocator * alloc, const char * JSON, CTError ** error)
{
    CTDictionary * object = CTDictionaryCreate(alloc);
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
            object = CTDictionaryFromJSONObject(alloc, JSONString, strchr(CTStringUTF8String(JSONString), '{') - CTStringUTF8String(JSONString), &end, error);
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

CTDictionary * CTDictionaryFromJSONObject(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, CTError ** error)
{
    CTDictionary * object = CTDictionaryCreate(alloc);
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
						CTDictionaryAddEntry2(object, string, CTObjectFromJSON(alloc, JSON, start + 1, &start, error));
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

CTArray * CTArrayFromJSON(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, CTError ** error)
{
    CTArray * array = CTArrayCreate(alloc);
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
					CTArrayAddEntry2(array, CTObjectFromJSON(alloc, JSON, start, &start, error));
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
                    CTStringAppendCharacters(string, character, CTSTRING_NO_LIMIT);
                    break;
                default:
                    character[0] = CTStringUTF8String(JSON)[start];
                    CTStringAppendCharacters(string, character, CTSTRING_NO_LIMIT);
                    break;
            }
        }
        *end = start + 1;
    }
    return string;
}

CTObject * CTObjectFromJSON(CTAllocator * alloc, CTString * restrict JSON, unsigned long start, unsigned long * end, CTError ** error)
{
    CTObject * object = NULL;
    assert(end);
    int loop = 1;
    while (loop && start < CTStringLength(JSON))
    {
        loop = 0;
        switch (CTStringUTF8String(JSON)[start])
        {
            case '"':
                object = CTObjectCreate(alloc, CTStringFromJSON(alloc, JSON, start, &start, error), CTOBJECT_TYPE_STRING);
                break;
            case '{':
                object = CTObjectCreate(alloc, CTDictionaryFromJSONObject(alloc, JSON, start, &start, error), CTOBJECT_TYPE_DICTIONARY);
                break;
            case '[':
                object = CTObjectCreate(alloc, CTArrayFromJSON(alloc, JSON, start, &start, error), CTOBJECT_TYPE_ARRAY);
                break;
            case 't':
            case 'f':
            case 'n':
                if (CTStringLength(JSON) - start >= 4)
                {
                    if (!strncmp("true", CTStringUTF8String(JSON) + start, 4))
                    {
                        start += strlen("true");
                        object = CTObjectCreate(alloc, CTNumberCreateWithInt(alloc, 1), CTOBJECT_TYPE_NUMBER);
                    }
                    else if (!strncmp("null", CTStringUTF8String(JSON) + start, 4))
                    {
                        start += strlen("null");
                        object = CTObjectCreate(alloc, CTNullCreate(alloc), CTOBJECT_TYPE_NULL);
                    }
                    else if (CTStringLength(JSON) - start > 4 && !strncmp("false", CTStringUTF8String(JSON) + start, 5))
                    {
                        start += strlen("false");
                        object = CTObjectCreate(alloc, CTNumberCreateWithInt(alloc, 0), CTOBJECT_TYPE_NUMBER);
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
                        double Double = strtod(CTStringUTF8String(numberString), &pEnd);
                        if (pEnd && pEnd != CTStringUTF8String(JSON))
                        {
                            if (CTStringLength(exponentString))
                            {
                                if (exponent <= 15 && exponent >= -6)
                                {
                                    object = CTObjectCreate(alloc, CTNumberCreateWithDouble(alloc, Double * pow(10, exponent)), CTOBJECT_TYPE_NUMBER);
                                }
                                else
                                {
                                    object = CTObjectCreate(alloc, CTLargeNumberCreate(alloc, CTNumberCreateWithDouble(alloc, Double), CTNumberCreateWithLong(alloc, exponent)), CTOBJECT_TYPE_NUMBER);
                                }
                            }
                            else
                            {
                                object = CTObjectCreate(alloc, CTNumberCreateWithDouble(alloc, Double), CTOBJECT_TYPE_NUMBER);
                            }
                        }
                    }
                    else
                    {
                        int64_t Long = strtoll(CTStringUTF8String(numberString), &pEnd, 0);
                        if (CTStringLength(exponentString))
                        {
                            if (exponent <= 15 && exponent >= -6)
                            {
                                object = CTObjectCreate(alloc, CTNumberCreateWithLong(alloc, Long * pow(10, exponent)), CTOBJECT_TYPE_NUMBER);
                            }
                            else
                            {
                                object = CTObjectCreate(alloc, CTLargeNumberCreate(alloc, CTNumberCreateWithLong(alloc, Long), CTNumberCreateWithLong(alloc, exponent)), CTOBJECT_TYPE_NUMBER);
                            }
                        }
                        else
                        {
                            object = CTObjectCreate(alloc, CTNumberCreateWithLong(alloc, Long), CTOBJECT_TYPE_NUMBER);
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

CTString * CTJSONSerialise(CTAllocator * alloc, CTDictionary * JSON, CTError ** error)
{
    CTString * retVal = CTStringCreate(alloc, "");
    CTJSONSerialiseRecursive(alloc, retVal, JSON, CTOBJECT_TYPE_DICTIONARY, error);
    return retVal;
}

void CTJSONSerialiseRecursive(CTAllocator * alloc, CTString * JSON, void * obj, int type, CTError ** error)
{
    CTDictionary * object = obj;
    CTArray * array = obj;
    uint64_t count = 0;
    int valueType = 0;
    void * ptr = NULL;
    
    if (type == CTOBJECT_TYPE_DICTIONARY)
    {
        count = object->count;
        CTStringAppendCharacter(JSON, '{');
    }
    else if (type == CTOBJECT_TYPE_ARRAY)
    {
        count = array->count;
        CTStringAppendCharacter(JSON, '[');
    }
    for (unsigned long i = 0; i < count; i++)
    {
        if (type == CTOBJECT_TYPE_DICTIONARY)
        {
            CTString * key = object->elements[i]->key;
            CTStringAppendCharacter(JSON, '"');
            CTStringAppendCharacters(JSON, CTStringUTF8String(key), CTSTRING_NO_LIMIT);
            CTStringAppendCharacter(JSON, '"');
            CTStringAppendCharacter(JSON, ':');
        }
        if (type == CTOBJECT_TYPE_DICTIONARY)
        {
            ptr = object->elements[i]->value->ptr;
            valueType = object->elements[i]->value->type;
        }
        else if (type == CTOBJECT_TYPE_ARRAY)
        {
            ptr = array->elements[i]->ptr;
            valueType = array->elements[i]->type;
        }
        
        switch (valueType)
        {
            case CTOBJECT_TYPE_DICTIONARY:
            case CTOBJECT_TYPE_ARRAY:
            {
                CTJSONSerialiseRecursive(alloc, JSON, ptr, valueType, error);
                break;
            }
            case CTOBJECT_TYPE_STRING:
                CTStringAppendCharacter(JSON, '"');
                CTStringAppendCharacters(JSON, ((CTString *)ptr)->characters, CTSTRING_NO_LIMIT);
                CTStringAppendCharacter(JSON, '"');
                break;
            case CTOBJECT_TYPE_NUMBER:
            {
				char * str = NULL;
				if (((CTNumber *)ptr)->type == CTNUMBER_TYPE_DOUBLE)
				{
					str = CTAllocatorAllocate(alloc, (unsigned)((ceil(log10(((CTNumber *)ptr)->value.Double)) + 7) * sizeof(char)));
					sprintf(str, "%Lf", ((CTNumber *)ptr)->value.Double);
					CTStringAppendCharacters(JSON, str, CTSTRING_NO_LIMIT);
					CTAllocatorDeallocate(alloc, str);
				}
				else
				{
					str = CTAllocatorAllocate(alloc, (unsigned)((ceil(log10(((CTNumber *)ptr)->value.Long)) + 1) * sizeof(char)));
					sprintf(str, "%lli", (long long)((CTNumber *)ptr)->value.Long);
					CTStringAppendCharacters(JSON, str, CTSTRING_NO_LIMIT);
				}
                
                CTAllocatorDeallocate(alloc, str);
                break;
            }
            case CTOBJECT_TYPE_NULL:
                CTStringAppendCharacters(JSON, ((CTNull *)ptr)->value, CTSTRING_NO_LIMIT);
                break;
            case CTOBJECT_TYPE_LARGE_NUMBER:
            {
                char * str = CTAllocatorAllocate(alloc, (unsigned)((ceil(log10(((CTLargeNumber *)ptr)->base->value.Double)) + 7) * sizeof(char)) + (int)((ceil(log10(((CTLargeNumber *)ptr)->exponent->value.Long)) + 1) * sizeof(char)));
                sprintf(str, "%Lfe%lli", ((CTLargeNumber *)ptr)->base->value.Double, (long long)((CTLargeNumber *)ptr)->exponent->value.Long);
                CTStringAppendCharacters(JSON, str, CTSTRING_NO_LIMIT);
                CTAllocatorDeallocate(alloc, str);
                break;
            }
        }
        if (i < count - 1)
        {
            CTStringAppendCharacter(JSON, ',');
        }
    }
    
    if (type == CTOBJECT_TYPE_DICTIONARY)
    {
        CTStringAppendCharacter(JSON, '}');
    }
    else if (type == CTOBJECT_TYPE_ARRAY)
    {
        CTStringAppendCharacter(JSON, ']');
    }
}
