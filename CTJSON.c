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

CTObjectRef CTStringFromJSON(CTAllocatorRef alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTErrorRef * error);
CTObjectRef CTObjectFromJSON(CTAllocatorRef alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTErrorRef * error);
void CTJSONSerialiseRecursive(CTAllocatorRef alloc, CTStringRef JSON, void * obj, CTOBJECT_TYPE type, CTJSONOptions options);
CTObjectRef CTJSONParse2(CTAllocatorRef alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTErrorRef * error);
CTObjectRef CTDictionaryFromJSON(CTAllocatorRef alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTErrorRef * error);
CTObjectRef CTLiteralFromJSON(CTAllocatorRef alloc, const CTString * restrict JSON, uint64_t * start, CTErrorRef * error);
CTObjectRef CTNumberFromJSON(CTAllocatorRef alloc, const CTString * restrict JSON, uint64_t * start, CTErrorRef * error);
CTObjectRef CTArrayFromJSON(CTAllocatorRef alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTErrorRef * error);


CTObjectRef CTJSONParse(CTAllocatorRef restrict alloc, const char * restrict JSON, CTJSONOptions options, CTErrorRef * error)
{
	uint64_t start = 0;
	CTAllocatorRef lalloc = CTAllocatorCreate();
	CTStringRef JSONString = CTStringCreate(lalloc, JSON);
	CTObjectRef retVal = CTJSONParse2(alloc, JSONString, &start, options, error);
	CTAllocatorRelease(lalloc);
	return retVal;
}

CTObjectRef CTJSONParse2(CTAllocatorRef alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTErrorRef * error)
{
	CTObjectRef retVal = CTObjectCreate(alloc, NULL, CTOBJECT_NOT_AN_OBJECT);
	if (CTStringLength(JSON))
	{
		for (;*start < CTStringLength(JSON); (*start)++)
		{
			if (isgraph(CTStringUTF8String(JSON)[*start]))
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
						return CTNumberFromJSON(alloc, JSON, start, error);
					case 'n':
					case 't':
					case 'f':
						return CTLiteralFromJSON(alloc, JSON, start, error);
				}
			}
		}
	}
	else
	{
		const char * err = "Empty JSON string";
		if (error)
		{
			*error = CTErrorCreate(alloc, err, CTJSON_PARSE_ERROR);
		}
	}
	return retVal;
}

CTObjectRef CTDictionaryFromJSON(CTAllocatorRef alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTErrorRef * error)
{
	CTDictionaryRef dictionary = CTDictionaryCreate(alloc);
	CTObjectRef key = NULL;
	CTObjectRef value = NULL;
	const char * err = "Formatting error in dictionary";
	++(*start);
	const char * JSONC = CTStringUTF8String(JSON);
	
	if (*start < CTStringLength(JSON))
	{
		do
		{
			if (JSONC[*start] != '}')
			{
				if (isgraph(JSONC[*start]))
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
							break;
						default:
							if (error)
							{
								*error = CTErrorCreate(alloc, err, CTJSON_PARSE_ERROR);
							}
							return CTObjectCreate(alloc, dictionary, CTOBJECT_TYPE_DICTIONARY);
					}
					if (CTObjectNonNilAndType(key, CTOBJECT_TYPE_STRING) && value)
					{
						CTDictionaryAddEntry(dictionary, CTStringUTF8String(CTObjectValue(key)), value);
						CTObjectRelease(key);
						key = value = NULL;
					}
				}
			}
		} while (*start < CTStringLength(JSON) && JSONC[(*start)++] != '}');
	}
	
	if ((key || value) && !(key && value))
	{
		if (error)
		{
			*error = CTErrorCreate(alloc, err, CTJSON_PARSE_ERROR);
		}
	}
	return CTObjectCreate(alloc, dictionary, CTOBJECT_TYPE_DICTIONARY);
}

CTObjectRef CTArrayFromJSON(CTAllocatorRef alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTErrorRef * error)
{
    CTArrayRef array = CTArrayCreate(alloc);
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

CTObjectRef CTStringFromJSON(CTAllocatorRef alloc, const CTString * restrict JSON, uint64_t * start, CTJSONOptions options, CTErrorRef * error)
{
    CTStringRef string = CTStringCreate(alloc, "");
	const char * JSONC = CTStringUTF8String(JSON);
	char character[3];
	uint64_t string_start_for_errors = *start;
	for (++(*start); *start < CTStringLength(JSON) && JSONC[*start] != (options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"'); ++(*start))
	{
		character[2] = 0;
		switch (JSONC[*start])
		{
			case '\\':
				if (*start + 1 < CTStringLength(JSON))
				{
					++(*start);
					switch (JSONC[*start])
					{
						case '/':
						case '\\':
						case '"':
							CTStringAppendCharacter(string, JSONC[*start]);
							break;
						case 'b':
							CTStringAppendCharacter(string, '\b');
							break;
						case 'f':
							CTStringAppendCharacter(string, '\f');
							break;
						case 'n':
							CTStringAppendCharacter(string, '\n');
							break;
						case 'r':
							CTStringAppendCharacter(string, '\r');
							break;
						case 't':
							CTStringAppendCharacter(string, '\t');
							break;
						case 'u':
							if (*start + 4 < CTStringLength(JSON))
							{
								const char * unicode = &JSONC[*start + 1];
								char just_hex[5];
								strncpy(just_hex, unicode, 4);
								uint32_t unicode_as_uint = (uint32_t)strtoul(just_hex, NULL, 0x10);
								uint8_t count = 0;
								if (unicode_as_uint < 0x80)
								{
									count = 1;
								}
								else if (unicode_as_uint < 0x800)
								{
									count = 2;
								}
								else if (unicode_as_uint < 0x10000)
								{
									count = 3;
								}
								
								if (count != 0)
								{
									char result[4] = {0};
									for (uint8_t i = count - 1; i > 0; --i)
									{
										result[i] = (char) 0x80 | (unicode_as_uint & 0x3F);
										unicode_as_uint >>= 6;
									}
									
									for (uint8_t i = 0; i < count; ++i)
									{
										unicode_as_uint |= 1 << (7 - i);
									}
									
									result[0] = unicode_as_uint;
									CTStringAppendCharacters(string, result, count);
								}
								*start += 4;
							}
							break;
							
						default:
							if (error)
							{
								char err[0x100];
								memset(err, 0, 0x100);
								sprintf(err, "Invalid escaped character at index %llu", string_start_for_errors);
								*error = CTErrorCreate(alloc, err, CTJSON_PARSE_ERROR);
							}
							break;
					}
				}
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
		sprintf(err, "Non-terminated string in JSON at index %llu", string_start_for_errors);
		if (error)
		{
			*error = CTErrorCreate(alloc, err, CTJSON_PARSE_ERROR);
		}
		++(*start);
	}
	return CTObjectWithString(alloc, string);
}

CTObjectRef CTNumberFromJSON(CTAllocatorRef alloc, const CTString * restrict JSON, uint64_t * start, CTErrorRef * error)
{
	assert(alloc);
	CTObjectRef retVal = NULL;
	const char * err = NULL;
	CTAllocatorRef allocl = CTAllocatorCreate();
	CTStringRef numberString = CTStringCreate(allocl, "");
	CTStringRef exponentString = CTStringCreate(allocl, "");
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
				*error = CTErrorCreate(alloc, err, CTJSON_PARSE_ERROR);
			}
			while (*start < CTStringLength(JSON) && JSONC[*start] != ',' && JSONC[*start] != ']' && JSONC[*start] != '}') ++(*start);
		}
		CTStringAppendCharacter(numberString, JSONC[(*start)++]);
		if (CTStringLength(numberString) > 1 && CTStringUTF8String(numberString)[0] == '0' && !CTStringContainsString(numberString, "."))
		{
			err = "A number was found that started with zero";
			if (error)
			{
				*error = CTErrorCreate(alloc, err, CTJSON_PARSE_ERROR);
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
				*error = CTErrorCreate(alloc, err, CTJSON_PARSE_ERROR);
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
				if (exponent <= 15)
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
			if (exponent <= 15)
			{
				retVal = CTObjectWithNumber(alloc, CTNumberCreateWithDouble(alloc, Long * pow(10, exponent)));
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

CTObjectRef CTLiteralFromJSON(CTAllocatorRef alloc, const CTString * restrict JSON, uint64_t * start, CTErrorRef * error)
{
	const char * err = "A JSON literal that was not true, false or null was found";
	uint8_t size = 4;
	
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
				return CTObjectCreate(alloc, CTNullCreate(), CTOBJECT_TYPE_NULL);
			}
		default:
			if (error)
			{
				*error = CTErrorCreate(alloc, err, CTJSON_PARSE_ERROR);
			}
			break;
	}
	return CTObjectCreate(alloc, NULL, CTOBJECT_NOT_AN_OBJECT);
}

CTStringRef CTJSONSerialise(CTAllocatorRef alloc, const CTObject * restrict JSON, CTJSONOptions options)
{
    CTStringRef retVal = CTStringCreate(alloc, "");
    CTJSONSerialiseRecursive(alloc, retVal, CTObjectValue(JSON), CTObjectType(JSON), options);
    return retVal;
}

void CTJSONSerialiseRecursive(CTAllocatorRef alloc, CTStringRef JSON, void * obj, CTOBJECT_TYPE type, CTJSONOptions options)
{
    uint64_t count = 0;
    
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
		{
			CTAllocatorRef local_alloc = CTAllocatorCreate();
			CTStringAppendCharacter(JSON, options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"');
			CTStringAppendCharacters(JSON, CTStringUTF8String(CTStringReplaceCharacterWithCharacters(local_alloc, obj, ^const char *(const char character) {
				if (options & CTJSONOptionsSingleQuoteStrings)
				{
					return character == '\'' ? "\\'" : NULL;
				}
				return character == '"' ? "\\\"" : NULL;
			})), CTSTRING_NO_LIMIT);
			CTStringAppendCharacter(JSON, options & CTJSONOptionsSingleQuoteStrings ? '\'' : '"');
			CTAllocatorRelease(local_alloc);
			break;
		}
		case CTOBJECT_TYPE_NUMBER:
		{
			double intpart;
			if (CTNumberType(obj) == CTNUMBER_TYPE_DOUBLE && modf(CTNumberDoubleValue(obj), &intpart))
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
			char str[(unsigned)(ceil(log10(CTNumberDoubleValue(CTLargeNumberBase(obj)))) + 7 + ceil(log10(CTNumberLongValue(CTLargeNumberBase(obj)) + 1))) + 1];
			if (CTNumberType(CTLargeNumberBase(obj)) == CTNUMBER_TYPE_DOUBLE)
			{
				sprintf(str, "%Lfe%lli", CTNumberDoubleValue(CTLargeNumberBase(obj)), CTNumberLongValue(CTLargeNumberExponent(obj)));
			}
			else
			{
				sprintf(str, "%llie%lli", CTNumberLongValue(CTLargeNumberBase(obj)), CTNumberLongValue(CTLargeNumberExponent(obj)));
			}
			CTStringAppendCharacters(JSON, str, CTSTRING_NO_LIMIT);
			break;
		}
			
		default:
		{
			void * ptr = NULL;
			CTOBJECT_TYPE valueType = 0;
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
					default:
						break;
				}
				
				CTJSONSerialiseRecursive(alloc, JSON, ptr, valueType, options);
				
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
size_t fileSize(const char * restrict file)
{
	long fsize = 0;
	if (access(file, F_OK | R_OK) != -1)
	{
		FILE * f = fopen(file, "r");
		if (f)
		{
			fseek(f, 0, SEEK_END);
			fsize = ftell(f);
			fclose(f);
		}
	}
	return fsize;
}

size_t readFile(const char * restrict file, char * buffer, size_t buflen)
{
	size_t bytesRead = 0;
	if (access(file, F_OK | R_OK) != -1)
	{
		FILE * f = fopen(file, "r");
		if (f)
		{
			bytesRead = fread(buffer, 1, buflen, f);
			fclose(f);
		}
	}
	return bytesRead;
}

CTObject * loadJSONWithAllocatorFromPath(CTAllocator * alloc, const char * restrict path)
{
	CTObject * ret_val = NULL;
	size_t file_size = fileSize(path);
	if (file_size > 0)
	{
		char buffer[file_size + 1];
		if (readFile(path, buffer, file_size) > 0)
		{
			CTError * error = NULL;
			CTObject * JSON_object = CTJSONParse(alloc, buffer, 0, &error);
			if (!error)
			{
				ret_val = JSON_object;
			}
		}
	}
	return ret_val;
}

void saveJSONToPath(const CTString * restrict content, const char * restrict path)
{
	FILE * f = fopen(path, "w");
	if (f)
	{
		fputs(CTStringUTF8String(content), f);
		fclose(f);
	}
}

void saveCTObjectAsJSONToPath(const CTObject * restrict content, const char * restrict path)
{
	CTAllocator * alloc = CTAllocatorCreate();
	CTString * JSON = CTJSONSerialise(alloc, content, 0);
	saveJSONToPath(JSON, path);
	CTAllocatorRelease(alloc);
}

void renameJSONFile(const char * restrict path1, const char * restrict path2)
{
	rename(path1, path2);
}
