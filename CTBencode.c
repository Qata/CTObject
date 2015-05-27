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

CTStringRef CTBencodeExtractString(CTAllocatorRef alloc, CTStringRef bencodedString, uint64_t * start)
{
    CTStringRef retVal = CTStringCreate(alloc, "");
    uint64_t length = strtoull(CTStringUTF8String(bencodedString) + *start, NULL, 10);
	
	*start += length > 0 ? ceil(log10(length + 1)) + 1 : 2;
    if (*start + length <= CTStringLength(bencodedString))
    {
        CTStringAppendCharacters(retVal, CTStringUTF8String(bencodedString) + *start, length);
    }
    *start += length;
    return retVal;
}

CTDictionaryRef CTBencodeExtractDictionary(CTAllocatorRef alloc, CTStringRef bencodedString, uint64_t * start, CTErrorRef * error)
{
    ++(*start);
    CTDictionaryRef dict = CTDictionaryCreate(alloc);
    while (*start < CTStringLength(bencodedString) && CTStringUTF8String(bencodedString)[*start] != 'e' && !*error)
    {
        CTStringRef key = CTObjectValue(CTBencodeParse2(alloc, CTStringUTF8String(bencodedString), start, error));
        CTObjectRef value = CTBencodeParse2(alloc, CTStringUTF8String(bencodedString), start, error);
        
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

CTArrayRef CTBencodeExtractList(CTAllocatorRef alloc, CTStringRef bencodedString, uint64_t * start, CTErrorRef * error)
{
    ++(*start);
    CTArrayRef list = CTArrayCreate(alloc);
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

CTNumberRef CTBencodeExtractInteger(CTAllocatorRef alloc, CTStringRef bencodedString, uint64_t * start)
{
    ++(*start);
    CTStringRef string = CTStringCreate(alloc, "");
    
    while ((CTStringUTF8String(bencodedString)[*start] >= '0' && CTStringUTF8String(bencodedString)[*start] <= '9' ) || CTStringUTF8String(bencodedString)[*start] == '.' || CTStringUTF8String(bencodedString)[*start] == '-' || CTStringUTF8String(bencodedString)[*start] == '+')
    {
        CTStringAppendCharacter(string, CTStringUTF8String(bencodedString)[*start]);
        ++(*start);
    }
    ++(*start);
    int64_t value = strtoll(CTStringUTF8String(string), NULL, 10);
    return CTNumberCreateWithLong(alloc, value);
}

CTObjectRef CTBencodeParse(CTAllocatorRef alloc, const char * bencoded, CTErrorRef * error)
{
    uint64_t start = 0;
	return CTBencodeParse2(alloc, bencoded, &start, error);
}

CTObjectRef CTBencodeParse2(CTAllocatorRef alloc, const char * bencoded, uint64_t * start, CTErrorRef * error)
{
	CTObjectRef retVal = NULL;
    CTAllocatorRef lalloc = CTAllocatorCreate();
    CTStringRef bencodedString = CTStringCreate(lalloc, bencoded);
    
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
                    retVal = CTObjectCreate(alloc, CTBencodeExtractInteger(alloc, bencodedString, start), CTOBJECT_TYPE_NUMBER);
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
                    retVal = CTObjectCreate(alloc, CTBencodeExtractString(alloc, bencodedString, start), CTOBJECT_TYPE_STRING);
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

CTStringRef CTBencodeSerialise(CTAllocatorRef restrict alloc, CTObjectRef restrict bencoded, CTErrorRef * error)
{
    CTStringRef retVal = CTStringCreate(alloc, "");
    CTAllocatorRef allocl = CTAllocatorCreate();
    switch (CTObjectType(bencoded))
    {
        case CTOBJECT_TYPE_DICTIONARY:
        {
            CTStringAppendCharacter(retVal, 'd');
            for (uint64_t i = 0; i < CTDictionaryCount(CTObjectValue(bencoded)); ++i)
            {
				CTDictionaryEntryRef pair = CTDictionaryEntryAtIndex(CTObjectValue(bencoded), i);
                if (CTStringLength(CTObjectValue(bencoded)))
                {
                    char buf[(int)ceil(log10(CTStringLength(CTDictionaryEntryKey(pair))) + 1)];
                    snprintf(buf, sizeof(buf), "%llu", (unsigned long long)CTStringLength(CTDictionaryEntryKey(pair)));
                    CTStringAppendCharacters(retVal, buf, CTSTRING_NO_LIMIT);
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
            for (uint64_t i = 0; i < ((CTArrayRef)CTObjectValue(bencoded))->count; ++i)
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
                char buf[(int)ceil(log10(CTNumberUnsignedLongValue(CTObjectValue(bencoded))) + 1)];
                snprintf(buf, sizeof(buf), "%llu", (unsigned long long)CTNumberUnsignedLongValue(CTObjectValue(bencoded)));
                CTStringAppendCharacters(retVal, buf, CTSTRING_NO_LIMIT);
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
                char buf[(unsigned)ceil(log10(CTStringLength(CTObjectValue(bencoded))) + 1)];
                snprintf(buf, sizeof(buf), "%llu", (unsigned long long)CTStringLength(CTObjectValue(bencoded)));
                CTStringAppendCharacters(retVal, buf, CTSTRING_NO_LIMIT);
            }
            else
            {
                CTStringAppendCharacter(retVal, '0');
            }
            CTStringAppendCharacter(retVal, ':');
            CTStringAppendString(retVal, CTObjectValue(bencoded));
            break;
        }
		default:
			break;
    }
	CTAllocatorRelease(allocl);
    return retVal;
}