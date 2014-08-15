//
//  CTBencode.c
//  CTObject
//
//  Created by Carlo Tortorella on 11/12/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTBencode.h"
#include "CTNumber.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

CTString * CTBencodeExtractString(CTAllocator * alloc, CTString * bencodedString, uint64_t * start, CTError ** error)
{
    CTString * retVal = CTStringCreate(alloc, "");
    uint64_t length = strtoull(CTStringUTF8String(bencodedString) + *start, NULL, 10);
	
	*start += length > 0 ? ceil(log10(length + 1)) + 1 : 2;
    if (*start + length <= CTStringLength(bencodedString))
    {
        CTStringAppendCharacters(retVal, CTStringUTF8String(bencodedString) + *start, length);
    }
    *start += length;
    return retVal;
}

CTDictionary * CTBencodeExtractDictionary(CTAllocator * alloc, CTString * bencodedString, uint64_t * start, CTError ** error)
{
    ++(*start);
    CTDictionary * dict = CTDictionaryCreate(alloc);
    while (*start < CTStringLength(bencodedString) && CTStringUTF8String(bencodedString)[*start] != 'e' && !*error)
    {
        CTString * key = CTObjectValue(CTBencodeParse2(alloc, CTStringUTF8String(bencodedString), start, error));
        CTObject * value = CTBencodeParse2(alloc, CTStringUTF8String(bencodedString), start, error);
        
        if (key && value)
        {
            CTDictionaryAddEntry2(dict, key, value);
        }
        else
        {
			if (error)
			{
				*error = CTErrorCreate(alloc, "Mismatched count of keys and values", 0);
			}
        }
    }
    if(CTStringUTF8String(bencodedString)[*start] != 'e')
    {
		if (error)
		{
			*error = CTErrorCreate(alloc, "No terminating e found for dictionary", 0);
		}
    }
    ++(*start);
    return dict;
}

CTArray * CTBencodeExtractList(CTAllocator * alloc, CTString * bencodedString, uint64_t * start, CTError ** error)
{
    ++(*start);
    CTArray * list = CTArrayCreate(alloc);
    while (*start < CTStringLength(bencodedString) && CTStringUTF8String(bencodedString)[*start] != 'e' && !*error)
    {
        CTArrayAddEntry2(list, CTBencodeParse2(alloc, CTStringUTF8String(bencodedString), start, error));
    }
    if (CTStringUTF8String(bencodedString)[*start] != 'e')
    {
        *error = CTErrorCreate(alloc, "No terminating e found for list", 0);
    }
    ++(*start);
    return list;
}

CTNumber * CTBencodeExtractInteger(CTAllocator * alloc, CTString * bencodedString, uint64_t * start, CTError ** error)
{
    ++(*start);
    CTString * string = CTStringCreate(alloc, "");
    
    while ((CTStringUTF8String(bencodedString)[*start] >= '0' && CTStringUTF8String(bencodedString)[*start] <= '9' ) || CTStringUTF8String(bencodedString)[*start] == '.' || CTStringUTF8String(bencodedString)[*start] == '-' || CTStringUTF8String(bencodedString)[*start] == '+')
    {
        CTStringAppendCharacter(string, CTStringUTF8String(bencodedString)[*start]);
        ++(*start);
    }
    ++(*start);
    int64_t value = strtoll(CTStringUTF8String(string), NULL, 10);
    return CTNumberCreateWithLong(alloc, value);
}

CTObject * CTBencodeParse(CTAllocator * alloc, const char * bencoded, CTError ** error)
{
    uint64_t start = 0;
	return CTBencodeParse2(alloc, bencoded, &start, error);
}

CTObject * CTBencodeParse2(CTAllocator * alloc, const char * bencoded, uint64_t * start, CTError ** error)
{
	CTObject * retVal = NULL;
    CTAllocator * lalloc = CTAllocatorCreate();
    CTString * bencodedString = CTStringCreate(lalloc, bencoded);
    
    if (CTStringLength(bencodedString))
    {
        if (*start < CTStringLength(bencodedString))
        {
            switch (bencoded[*start])
            {
                case 'd':
                    retVal = CTObjectCreate(alloc, CTBencodeExtractDictionary(alloc, bencodedString, start, error), CTOBJECT_TYPE_DICTIONARY);
                    break;
                case 'l':
                    retVal = CTObjectCreate(alloc, CTBencodeExtractList(alloc, bencodedString, start, error), CTOBJECT_TYPE_ARRAY);
                    break;
                case 'i':
                    retVal = CTObjectCreate(alloc, CTBencodeExtractInteger(alloc, bencodedString, start, error), CTOBJECT_TYPE_NUMBER);
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
                    retVal = CTObjectCreate(alloc, CTBencodeExtractString(alloc, bencodedString,start, error), CTOBJECT_TYPE_STRING);
                    break;
                default:
                    *error = CTErrorCreate(alloc, "Incorrect starting character", 0);
                    break;
            }
        }
    }
    else
    {
        *error = CTErrorCreate(alloc, "Empty string", 0);
    }
    
    CTAllocatorRelease(lalloc);
    return retVal;
}

CTString * CTBencodeSerialise(CTAllocator * restrict alloc, CTObject * restrict bencoded, CTError ** error)
{
    CTString * retVal = CTStringCreate(alloc, "");
    CTAllocator * allocl = CTAllocatorCreate();
    switch (CTObjectType(bencoded))
    {
        case CTOBJECT_TYPE_DICTIONARY:
        {
            CTStringAppendCharacter(retVal, 'd');
            for (uint64_t i = 0; i < CTDictionaryCount(CTObjectValue(bencoded)); ++i)
            {
				CTDictionaryEntry * pair = CTDictionaryEntryAtIndex(CTObjectValue(bencoded), i);
                if (CTStringLength(CTObjectValue(bencoded)))
                {
                    char buf[(int)floor(log(CTStringLength(CTDictionaryEntryKey(pair))) + 1)];
                    snprintf(buf, sizeof(buf), "%llu", (unsigned long long)CTStringLength(CTDictionaryEntryKey(pair)));
                    CTStringAppendCharacters(retVal, buf, sizeof(buf));
                    CTStringAppendCharacter(retVal, ':');
                    CTStringAppendString(retVal, CTDictionaryEntryKey(pair));
                }
                else
                {
                    CTStringAppendCharacters(retVal, "0:", CTSTRING_NO_LIMIT);
                }
                CTStringAppendString(retVal, CTBencodeSerialise(allocl, CTDictionaryEntryValue(pair), error));
            }
            CTStringAppendCharacter(retVal, 'e');
            break;
        }
            
        case CTOBJECT_TYPE_ARRAY:
        {
            CTStringAppendCharacter(retVal, 'l');
            for (uint64_t i = 0; i < ((CTArray *)CTObjectValue(bencoded))->count; ++i)
            {
                CTStringAppendString(retVal, CTBencodeSerialise(allocl, CTArrayObjectAtIndex(CTObjectValue(bencoded), i), error));
            }
            CTStringAppendCharacter(retVal, 'e');
            break;
        }
            
        case CTOBJECT_TYPE_NUMBER:
        {
            CTStringAppendCharacter(retVal, 'i');
            if (CTNumberUnsignedLongValue(CTObjectValue(bencoded)))
            {
                char buf[(int)floor(log(CTNumberUnsignedLongValue(CTObjectValue(bencoded))) + 1)];
                snprintf(buf, sizeof(buf), "%llu", (unsigned long long)CTNumberUnsignedLongValue(CTObjectValue(bencoded)));
                CTStringAppendCharacters(retVal, buf, sizeof(buf));
            }
            else
            {
                CTStringAppendCharacter(retVal, '0');
            }
            CTStringAppendCharacter(retVal, 'e');
            break;
        }
            
        case CTOBJECT_TYPE_STRING:
        {
            if (CTStringLength(CTObjectValue(bencoded)))
            {
                char buf[(int)floor(log(CTStringLength(CTObjectValue(bencoded))) + 1)];
                snprintf(buf, sizeof(buf), "%llu", (unsigned long long)CTStringLength(CTObjectValue(bencoded)));
                CTStringAppendCharacters(retVal, buf, sizeof(buf));
            }
            else
            {
                CTStringAppendCharacter(retVal, '0');
            }
            CTStringAppendCharacter(retVal, ':');
            CTStringAppendString(retVal, CTObjectValue(bencoded));
            break;
        }
    }
	CTAllocatorRelease(allocl);
    return retVal;
}