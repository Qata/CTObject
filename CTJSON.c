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
#include <ctype.h>

CTObject * CTStringFromJSON(CTAllocator * alloc, const char * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error);
CTObject * CTObjectFromJSON(CTAllocator * alloc, const char * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error);
void CTJSONSerialiseRecursive(CTAllocator * alloc, CTString * JSON, void * obj, int type, CTJSONOptions options, CTError ** error);
CTObject * CTJSONParse2(CTAllocator * alloc, const char * JSON, uint64_t * start, CTJSONOptions options, CTError ** error);
CTObject * CTDictionaryFromJSON(CTAllocator * alloc, const char * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error);
CTObject * CTLiteralFromJSON(CTAllocator * alloc, const char * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error);
CTObject * CTNumberFromJSON(CTAllocator * alloc, const char * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error);
CTObject * CTArrayFromJSON(CTAllocator * alloc, const char * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error);


CTObject * CTJSONParse(CTAllocator * alloc, const char * JSON, CTJSONOptions options, CTError ** error)
{
	uint64_t start = 0;
	return CTJSONParse2(alloc, JSON, &start, options, error);
}

CTObject * CTJSONParse2(CTAllocator * alloc, const char * JSON, uint64_t * start, CTJSONOptions options, CTError ** error)
{
	CTObject * retVal = CTObjectCreate(alloc, NULL, CTOBJECT_NOT_AN_OBJECT);
	if (strlen(JSON))
	{
		for (;*start < strlen(JSON); (*start)++)
		{
			switch (JSON[*start])
			{
				case '{':
					return CTDictionaryFromJSON(alloc, JSON, start, options, error);
					break;
				case '[':
					return CTArrayFromJSON(alloc, JSON, start, options, error);
					break;
				case '"':
				case '\'':
					return CTStringFromJSON(alloc, JSON, start, options, error);
					break;
				case '-':
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					return CTNumberFromJSON(alloc, JSON, start, options, error);
					break;
				case 'n':
				case 't':
				case 'f':
					return CTLiteralFromJSON(alloc, JSON, start, options, error);
					break;
				case ' ':
				case '\r':
				case '\n':
				case '\t':
					break;
			}
		}
	}
	else
	{
		const char * err = "Empty JSON string\n";
		if (error)
		{
			*error = CTErrorCreate(alloc, err, 0);
		}
		fputs(err, stderr);
	}
	return retVal;
}

CTObject * CTDictionaryFromJSON(CTAllocator * alloc, const char * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error)
{
	CTDictionary * dictionary = CTDictionaryCreate(alloc);
	CTObject * key = NULL;
	CTObject * value = NULL;
	const char * err = "Formatting error in dictionary\n";
	++*start;
    while (*start < strlen(JSON) && JSON[*start] != '}')
	{
		switch (JSON[*start])
		{
			case '"':
			case '\'':
				key = CTJSONParse2(alloc, JSON, start, options, error);
				break;
			case ':':
				++(*start);
				value = CTJSONParse2(alloc, JSON, start, options, error);
				break;
			case ',':
			case ' ':
			case '\r':
			case '\n':
			case '\t':
				break;
			default:
				if (error)
				{
					*error = CTErrorCreate(alloc, err, 0);
				}
				fputs(err, stderr);
				return CTObjectCreate(alloc, dictionary, CTOBJECT_TYPE_DICTIONARY);
				break;
		}
		if (key && value && key->type == CTOBJECT_TYPE_STRING)
		{
			CTDictionaryAddEntry2(dictionary, key->ptr, value);
			key = value = NULL;
		}
		++(*start);
		if (JSON[*start - 1] == '}')
			break;
	}
	
	if ((key || value) && !(key && value))
	{
		if (error)
		{
			*error = CTErrorCreate(alloc, err, 0);
		}
		fputs(err, stderr);
		return CTObjectCreate(alloc, dictionary, CTOBJECT_TYPE_DICTIONARY);
	}
	
	return CTObjectCreate(alloc, dictionary, CTOBJECT_TYPE_DICTIONARY);
}

CTObject * CTArrayFromJSON(CTAllocator * alloc, const char * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error)
{
    CTArray * array = CTArrayCreate(alloc);
	(*start)++;
    while (*start < strlen(JSON))
	{
		switch (JSON[*start])
        {
			case ',':
            case ']':
				break;
			default:
				CTArrayAddEntry2(array, CTJSONParse2(alloc, JSON, start, options, error));
				break;
		}
		++(*start);
		if (JSON[*start - 1] == ']')
			break;
	}
	return CTObjectCreate(alloc, array, CTOBJECT_TYPE_ARRAY);
}

CTObject * CTStringFromJSON(CTAllocator * alloc, const char * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error)
{
    CTString * string = CTStringCreate(alloc, "");
	
	char character[3];
	for (++(*start); *start < strlen(JSON) && JSON[*start] != (options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"'); (*start)++)
	{
		character[2] = 0;
		switch (JSON[*start])
		{
			case '\\':
				character[0] = JSON[*start];
				character[1] = JSON[++(*start)];
				CTStringAppendCharacters(string, character, 2);
				break;
			default:
				CTStringAppendCharacter(string, JSON[*start]);
				break;
		}
	}
	if (JSON[*start] != (options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"'))
	{
		char err[0x100];
		memset(err, 0, 0x100);
		sprintf(err, "Non-terminated string in JSON at index %llu", *start);
		if (error)
		{
			*error = CTErrorCreate(alloc, err, 0);
		}
		fputs(err, stderr);
		++(*start);
	}
	return CTObjectCreate(alloc, string, CTOBJECT_TYPE_STRING);
}

CTObject * CTNumberFromJSON(CTAllocator * alloc, const char * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error)
{
	CTObject * retVal = NULL;
	const char * err = NULL;
	CTAllocator * allocl = CTAllocatorCreate();
	CTString * numberString = CTStringCreate(allocl, "");
	CTString * exponentString = CTStringCreate(allocl, "");
	char * pEnd;
	
	while (*start < strlen(JSON) && (isdigit(JSON[*start]) || JSON[*start] == '.'))
	{
		CTStringAppendCharacter(numberString, JSON[(*start)++]);
	}
	
	if (*start < strlen(JSON) && (JSON[*start] == 'e' || JSON[*start] == 'E'))
	{
		++*start;
		if (*start < strlen(JSON) && JSON[*start] == '-')
		{
			CTStringAppendCharacter(exponentString, JSON[(*start)++]);
		}
		
		while (*start < strlen(JSON) && isdigit(JSON[*start]))
		{
			CTStringAppendCharacter(exponentString, JSON[(*start)++]);
		}
		
		if (*start < strlen(JSON) && JSON[*start] == '.')
		{
			err = "E notation cannot be a floating point number\n";
			if (error)
			{
				*error = CTErrorCreate(alloc, err, 0);
			}
			fputs(err, stderr);
			while (*start < strlen(JSON) && JSON[*start] != ',' && JSON[*start] != ']' && JSON[*start] != '}') ++(*start);
		}
	}
	
	long exponent = strtol(CTStringUTF8String(exponentString), &pEnd, 0);
	if (CTStringContainsString(numberString, "."))
	{
		double Double = strtod(CTStringUTF8String(numberString), &pEnd);
		if (pEnd && pEnd != JSON)
		{
			if (CTStringLength(exponentString))
			{
				if (exponent >= -6 && exponent <= 15)
				{
					retVal = CTObjectCreate(alloc, CTNumberCreateWithDouble(alloc, Double * pow(10, exponent)), CTOBJECT_TYPE_NUMBER);
				}
				else
				{
					retVal = CTObjectCreate(alloc, CTLargeNumberCreate(alloc, CTNumberCreateWithDouble(alloc, Double), CTNumberCreateWithLong(alloc, exponent)), CTOBJECT_TYPE_NUMBER);
				}
			}
			else
			{
				retVal = CTObjectCreate(alloc, CTNumberCreateWithDouble(alloc, Double), CTOBJECT_TYPE_NUMBER);
			}
		}
	}
	else
	{
		int64_t Long = strtoll(CTStringUTF8String(numberString), &pEnd, 0);
		if (CTStringLength(exponentString))
		{
			if (exponent >= -6 && exponent <= 15)
			{
				retVal = CTObjectCreate(alloc, CTNumberCreateWithLong(alloc, Long * pow(10, exponent)), CTOBJECT_TYPE_NUMBER);
			}
			else
			{
				retVal = CTObjectCreate(alloc, CTLargeNumberCreate(alloc, CTNumberCreateWithLong(alloc, Long), CTNumberCreateWithLong(alloc, exponent)), CTOBJECT_TYPE_LARGE_NUMBER);
			}
		}
		else
		{
			retVal = CTObjectCreate(alloc, CTNumberCreateWithLong(alloc, Long), CTOBJECT_TYPE_NUMBER);
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
	return retVal ? retVal : CTObjectCreate(alloc, 0, CTOBJECT_NOT_AN_OBJECT);
}

CTObject * CTLiteralFromJSON(CTAllocator * alloc, const char * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error)
{
	const char * err = "A JSON literal that was not true, false or null was found\n";
	unsigned size = 4;
	
	switch (JSON[*start])
	{
		case 't':
			if (strlen(JSON + *start) >= size && !strncmp(JSON + *start, "true", size))
			{
				*start += size;
				return CTObjectCreate(alloc, CTNumberCreateWithUnsignedInt(alloc, 1), CTOBJECT_TYPE_NUMBER);
			}
		case 'f':
			size = 5;
			if (strlen(JSON + *start) >= size && !strncmp(JSON + *start, "false", size))
			{
				*start += size;
				return CTObjectCreate(alloc, CTNumberCreateWithUnsignedInt(alloc, 0), CTOBJECT_TYPE_NUMBER);
			}
		case 'n':
			if (strlen(JSON + *start) >= 4 && !strncmp(JSON + *start, "null", 4))
			{
				*start += size;
				return CTObjectCreate(alloc, CTNullCreate(alloc), CTOBJECT_TYPE_NULL);
			}
		default:
			if (error)
			{
				*error = CTErrorCreate(alloc, err, 0);
			}
			fputs(err, stderr);
			break;
	}
	return CTObjectCreate(alloc, NULL, CTOBJECT_NOT_AN_OBJECT);
}

CTString * CTJSONSerialise(CTAllocator * alloc, CTObject * JSON, CTJSONOptions options, CTError ** error)
{
    CTString * retVal = CTStringCreate(alloc, "");
    CTJSONSerialiseRecursive(alloc, retVal, JSON->ptr, JSON->type, options, error);
    return retVal;
}

void CTJSONSerialiseRecursive(CTAllocator * alloc, CTString * JSON, void * obj, int type, CTJSONOptions options, CTError ** error)
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
            CTStringAppendCharacter(JSON, options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"');
            CTStringAppendCharacters(JSON, CTStringUTF8String(key), CTSTRING_NO_LIMIT);
            CTStringAppendCharacter(JSON, options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"');
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
                CTJSONSerialiseRecursive(alloc, JSON, ptr, valueType, options, error);
                break;
            }
            case CTOBJECT_TYPE_STRING:
                CTStringAppendCharacter(JSON, options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"');
                CTStringAppendCharacters(JSON, ((CTString *)ptr)->characters, CTSTRING_NO_LIMIT);
                CTStringAppendCharacter(JSON, options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"');
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
        if (i < (count - 1))
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
