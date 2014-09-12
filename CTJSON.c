//
//  CTJSON.c
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTJSON.h"
#include "CTFunctions.h"
#include "CTString.h"
#include "CTObject.h"
#include "CTDictionary.h"
#include "CTArray.h"
#include "CTNumber.h"
#include "CTNull.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

CTObject * CTStringFromJSON(CTAllocator * alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error);
CTObject * CTObjectFromJSON(CTAllocator * alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error);
void CTJSONSerialiseRecursive(CTAllocator * alloc, CTString * JSON, void * obj, int type, CTJSONOptions options, CTError ** error);
CTObject * CTJSONParse2(CTAllocator * alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error);
CTObject * CTDictionaryFromJSON(CTAllocator * alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error);
CTObject * CTLiteralFromJSON(CTAllocator * alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error);
CTObject * CTNumberFromJSON(CTAllocator * alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error);
CTObject * CTArrayFromJSON(CTAllocator * alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error);


CTObject * CTJSONParse(CTAllocator * alloc, const char * restrict JSON, CTJSONOptions options, CTError ** error)
{
	uint64_t start = 0;
	CTAllocator * lalloc = CTAllocatorCreate();
	CTString * JSONString = CTStringCreate(lalloc, JSON);
	CTObject * retVal = CTJSONParse2(alloc, JSONString, &start, options, error);
	CTAllocatorRelease(lalloc);
	return retVal;
}

CTObject * CTJSONParse2(CTAllocator * alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error)
{
	CTObject * retVal = CTObjectCreate(alloc, NULL, CTOBJECT_NOT_AN_OBJECT);
	if (CTStringLength(JSON))
	{
		for (;*start < CTStringLength(JSON); (*start)++)
		{
			switch (CTStringUTF8String(JSON)[*start])
			{
				case '{':
					return CTDictionaryFromJSON(alloc, JSON, start, options, error);
				case '[':
					return CTArrayFromJSON(alloc, JSON, start, options, error);
				case '"':
				case '\'':
					return CTStringFromJSON(alloc, JSON, start, options, error);
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
				case 'n':
				case 't':
				case 'f':
					return CTLiteralFromJSON(alloc, JSON, start, options, error);
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
		const char * err = "Empty JSON string";
		if (error)
		{
			*error = CTErrorCreate(alloc, err, 0);
		}
	}
	return retVal;
}

CTObject * CTDictionaryFromJSON(CTAllocator * alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error)
{
	CTDictionary * dictionary = CTDictionaryCreate(alloc);
	CTObject * key = NULL;
	CTObject * value = NULL;
	const char * err = "Formatting error in dictionary";
	++(*start);
	const char * JSONC = CTStringUTF8String(JSON);
    while (*start < CTStringLength(JSON) && JSONC[*start] != '}')
	{
		switch (JSONC[*start])
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
				return CTObjectCreate(alloc, dictionary, CTOBJECT_TYPE_DICTIONARY);
				break;
		}
		if (key && value && CTObjectType(key) == CTOBJECT_TYPE_STRING)
		{
			CTDictionaryAddEntry2(dictionary, CTObjectValue(key), value);
			CTAllocatorDeallocate(alloc, key);
			key = value = NULL;
		}
		if (JSONC[(*start)++] == '}')
			break;
	}
	
	if ((key || value) && !(key && value))
	{
		if (error)
		{
			*error = CTErrorCreate(alloc, err, 0);
		}
		return CTObjectCreate(alloc, dictionary, CTOBJECT_TYPE_DICTIONARY);
	}
	
	return CTObjectCreate(alloc, dictionary, CTOBJECT_TYPE_DICTIONARY);
}

CTObject * CTArrayFromJSON(CTAllocator * alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error)
{
    CTArray * array = CTArrayCreate(alloc);
	++(*start);
	const char * JSONC = CTStringUTF8String(JSON);
    while (*start < CTStringLength(JSON))
	{
		switch (JSONC[*start])
        {
			case ',':
            case ']':
				break;
			default:
				CTArrayAddEntry2(array, CTJSONParse2(alloc, JSON, start, options, error));
				break;
		}
		if (JSONC[(*start)++] == ']') break;
	}
	return CTObjectCreate(alloc, array, CTOBJECT_TYPE_ARRAY);
}

CTObject * CTStringFromJSON(CTAllocator * alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error)
{
    CTString * string = CTStringCreate(alloc, "");
	const char * JSONC = CTStringUTF8String(JSON);
	char character[3];
	for (++(*start); *start < CTStringLength(JSON) && JSONC[*start] != (options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"'); (*start)++)
	{
		character[2] = 0;
		switch (JSONC[*start])
		{
			case '\\':
				memcpy(character, JSONC + (*start)++, 2);
				CTStringAppendCharacters(string, character, 2);
				break;
			default:
				CTStringAppendCharacter(string, JSONC[*start]);
				break;
		}
	}
	if (JSONC[*start] != (options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"'))
	{
		char err[0x100];
		memset(err, 0, 0x100);
		sprintf(err, "Non-terminated string in JSON at index %llu", *start);
		if (error)
		{
			*error = CTErrorCreate(alloc, err, 0);
		}
		++(*start);
	}
	return CTObjectWithString(alloc, string);
}

CTObject * CTNumberFromJSON(CTAllocator * alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error)
{
	assert(alloc);
	CTObject * retVal = NULL;
	const char * err = NULL;
	CTAllocator * allocl = CTAllocatorCreate();
	CTString * numberString = CTStringCreate(allocl, "");
	CTString * exponentString = CTStringCreate(allocl, "");
	char * pEnd = NULL;
	
	const char * JSONC = CTStringUTF8String(JSON);
	
	if (*start < CTStringLength(JSON))
	{
		if (JSONC[*start] == '-' || JSONC[*start] == '+')
		{
			CTStringAppendCharacter(numberString, JSONC[(*start)++]);
		}
	}
	
	while (*start < CTStringLength(JSON) && (isdigit(JSONC[*start]) || JSONC[*start] == '.'))
	{
		if (JSONC[*start] == '.' && CTStringContainsString(numberString, "."))
		{
			err = "A number was found that contained more than one decimal point";
			if (error)
			{
				*error = CTErrorCreate(alloc, err, 0);
			}
			while (*start < CTStringLength(JSON) && JSONC[*start] != ',' && JSONC[*start] != ']' && JSONC[*start] != '}') ++(*start);
		}
		CTStringAppendCharacter(numberString, JSONC[(*start)++]);
		if (CTStringLength(numberString) > 1 && CTStringUTF8String(numberString)[0] == '0' && !CTStringContainsString(numberString, "."))
		{
			err = "A number was found that started with zero";
			if (error)
			{
				*error = CTErrorCreate(alloc, err, 0);
			}
			while (*start < CTStringLength(JSON) && JSONC[*start] != ',' && JSONC[*start] != ']' && JSONC[*start] != '}') ++(*start);
		}
	}
	
	if (*start < CTStringLength(JSON) && tolower(JSONC[*start]) == 'e')
	{
		++(*start);
		if (*start < CTStringLength(JSON) && JSONC[*start] == '-')
		{
			CTStringAppendCharacter(exponentString, JSONC[(*start)++]);
		}
		else if (*start < CTStringLength(JSON) && JSONC[*start] == '+')
		{
			++(*start);
		}
		
		while (*start < CTStringLength(JSON) && isdigit(JSONC[*start]))
		{
			CTStringAppendCharacter(exponentString, JSONC[(*start)++]);
		}
		
		if (*start < CTStringLength(JSON) && JSONC[*start] == '.')
		{
			err = "E notation cannot be a floating point number";
			if (error)
			{
				*error = CTErrorCreate(alloc, err, 0);
			}
			while (*start < CTStringLength(JSON) && JSONC[*start] != ',' && JSONC[*start] != ']' && JSONC[*start] != '}') ++(*start);
		}
	}
	
	long exponent = strtol(CTStringUTF8String(exponentString), &pEnd, 0);
	if (CTStringContainsString(numberString, "."))
	{
		double Double = strtod(CTStringUTF8String(numberString), &pEnd);
		if (pEnd && pEnd != JSONC)
		{
			if (CTStringLength(exponentString))
			{
				if (exponent >= -6 && exponent <= 15)
				{
					retVal = CTObjectWithNumber(alloc, CTNumberCreateWithDouble(alloc, Double * pow(10, exponent)));
				}
				else
				{
					retVal = CTObjectWithLargeNumber(alloc, CTLargeNumberCreate(alloc, CTNumberCreateWithDouble(alloc, Double), CTNumberCreateWithLong(alloc, exponent)));
				}
			}
			else
			{
				retVal = CTObjectWithNumber(alloc, CTNumberCreateWithDouble(alloc, Double));
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
				retVal = CTObjectWithNumber(alloc, CTNumberCreateWithLong(alloc, Long * pow(10, exponent)));
			}
			else
			{
				retVal = CTObjectWithLargeNumber(alloc, CTLargeNumberCreate(alloc, CTNumberCreateWithLong(alloc, Long), CTNumberCreateWithLong(alloc, exponent)));
			}
		}
		else
		{
			retVal = CTObjectWithNumber(alloc, CTNumberCreateWithLong(alloc, Long));
		}
	}
	CTAllocatorRelease(allocl);
	return retVal;
}

CTObject * CTLiteralFromJSON(CTAllocator * alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTError ** error)
{
	const char * err = "A JSON literal that was not true, false or null was found";
	unsigned size = 4;
	
	const char * JSONC = CTStringUTF8String(JSON);
	switch (JSONC[*start])
	{
		case 't':
			if (CTStringLength(JSON) - *start >= size && !strncmp(JSONC + *start, "true", size))
			{
				*start += size;
				return CTObjectCreate(alloc, CTNumberCreateWithUnsignedInt(alloc, 1), CTOBJECT_TYPE_NUMBER);
			}
		case 'f':
			size = 5;
			if (CTStringLength(JSON) - *start >= size && !strncmp(JSONC + *start, "false", size))
			{
				*start += size;
				return CTObjectCreate(alloc, CTNumberCreateWithUnsignedInt(alloc, 0), CTOBJECT_TYPE_NUMBER);
			}
		case 'n':
			if (CTStringLength(JSON) - *start >= size && !strncmp(JSONC + *start, "null", 4))
			{
				*start += size;
				return CTObjectCreate(alloc, CTNullCreate(alloc), CTOBJECT_TYPE_NULL);
			}
		default:
			if (error)
			{
				*error = CTErrorCreate(alloc, err, 0);
			}
			break;
	}
	return CTObjectCreate(alloc, NULL, CTOBJECT_NOT_AN_OBJECT);
}

CTString * CTJSONSerialise(CTAllocator * alloc, CTObject * JSON, CTJSONOptions options, CTError ** error)
{
    CTString * retVal = CTStringCreate(alloc, "");
    CTJSONSerialiseRecursive(alloc, retVal, CTObjectValue(JSON), CTObjectType(JSON), options, error);
    return retVal;
}

void CTJSONSerialiseRecursive(CTAllocator * alloc, CTString * JSON, void * obj, int type, CTJSONOptions options, CTError ** error)
{
    uint64_t count = 0;
    int valueType = 0;
    
    if (type == CTOBJECT_TYPE_DICTIONARY)
    {
        count = CTDictionaryCount(obj);
        CTStringAppendCharacter(JSON, '{');
    }
    else if (type == CTOBJECT_TYPE_ARRAY)
    {
        count = CTArrayCount(obj);
        CTStringAppendCharacter(JSON, '[');
    }
	
	switch (type)
	{
		case CTOBJECT_TYPE_STRING:
			CTStringAppendCharacter(JSON, options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"');
			CTStringAppendCharacters(JSON, CTStringUTF8String(obj), CTSTRING_NO_LIMIT);
			CTStringAppendCharacter(JSON, options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"');
			break;
		case CTOBJECT_TYPE_NUMBER:
		{
			if (CTNumberType(obj) == CTNUMBER_TYPE_DOUBLE)
			{
				char str[(unsigned)ceil(log10(CTNumberDoubleValue(obj)) + 0x10)];
				sprintf(str, "%Lf", CTNumberDoubleValue(obj));
				CTStringAppendCharacters(JSON, str, CTSTRING_NO_LIMIT);
			}
			else
			{
				char str[(unsigned)ceil(log10(CTNumberLongValue(obj)) + 1)];
				sprintf(str, "%lli", CTNumberLongValue(obj));
				CTStringAppendCharacters(JSON, str, CTSTRING_NO_LIMIT);
			}
			
			break;
		}
		case CTOBJECT_TYPE_NULL:
			CTStringAppendCharacters(JSON, CTNullValue(obj), CTSTRING_NO_LIMIT);
			break;
		case CTOBJECT_TYPE_LARGE_NUMBER:
		{
			char * str = CTAllocatorAllocate(alloc, (unsigned)(ceil(log10(CTNumberDoubleValue(CTLargeNumberBase(obj)))) + 7 + ceil(log10(CTNumberLongValue(CTLargeNumberBase(obj)) + 1))) + 1);
			switch (CTNumberType(CTLargeNumberBase(obj)))
			{
				case CTNUMBER_TYPE_DOUBLE:
					sprintf(str, "%Lfe%lli", CTNumberDoubleValue(CTLargeNumberBase(obj)), CTNumberLongValue(CTLargeNumberExponent(obj)));
					break;
				default:
					sprintf(str, "%llie%lli", CTNumberLongValue(CTLargeNumberBase(obj)), CTNumberLongValue(CTLargeNumberExponent(obj)));
					break;
			}
			CTStringAppendCharacters(JSON, str, CTSTRING_NO_LIMIT);
			CTAllocatorDeallocate(alloc, str);
			break;
		}
			
		default:
		{
			void * ptr = NULL;
			for (uint64_t i = 0; i < count; ++i)
			{
				switch (type)
				{
					case CTOBJECT_TYPE_DICTIONARY:
						ptr = CTObjectValue(CTDictionaryEntryValue(CTDictionaryEntryAtIndex(obj, i)));
						valueType = CTObjectType(CTDictionaryEntryValue(CTDictionaryEntryAtIndex(obj, i)));
						CTStringAppendCharacter(JSON, options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"');
						CTStringAppendCharacters(JSON, CTStringUTF8String(CTDictionaryEntryKey(CTDictionaryEntryAtIndex(obj, i))), CTSTRING_NO_LIMIT);
						CTStringAppendCharacter(JSON, options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"');
						CTStringAppendCharacter(JSON, ':');
						break;
					case CTOBJECT_TYPE_ARRAY:
						ptr = CTObjectValue(CTArrayObjectAtIndex(obj, i));
						valueType = CTObjectType(CTArrayObjectAtIndex(obj, i));
						break;
				}
				
				CTJSONSerialiseRecursive(alloc, JSON, ptr, valueType, options, error);
				
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
			break;
		}
	}
}
