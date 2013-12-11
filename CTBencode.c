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

CTBencodeDictionary * CTBencodeDictionaryCreate(CTAllocator * alloc)
{
    CTBencodeDictionary * object = CTAllocatorAllocate(alloc, sizeof(CTBencodeDictionary));
    object->count = 0;
    object->elements = NULL;
    object->alloc = alloc;
    return object;
}

CTBencodeDictionaryKeyValuePair * CTBencodeDictionaryCreateEntry(CTAllocator * alloc)
{
    CTBencodeDictionaryKeyValuePair * retVal = CTAllocatorAllocate(alloc, sizeof(CTBencodeDictionaryKeyValuePair));
    retVal->key = NULL;
    retVal->value = NULL;
    return retVal;
}

void CTBencodeDictionaryAddKeyValuePair(CTBencodeDictionary * object, CTString * key, CTBencodeValueContainer * value)
{
    unsigned long index = object->count++;
    
	assert((object->elements = CTAllocatorReallocate(object->alloc, object->elements, sizeof(struct CTBencodeDictionaryKeyValuePair *) * object->count)));
    object->elements[index] = CTBencodeDictionaryCreateEntry(object->alloc);
	object->elements[index]->key = key;
    object->elements[index]->value = value;
}

CTBencodeList * CTBencodeListCreate(CTAllocator * alloc)
{
    CTBencodeList * array = CTAllocatorAllocate(alloc, sizeof(CTBencodeList));
    array->count = 0;
    array->elements = NULL;
    array->alloc = alloc;
    return array;
}

void CTBencodeListAddValueContainer(CTBencodeList * array, CTBencodeValueContainer * value)
{
    unsigned long index = array->count++;
    
	assert((array->elements = CTAllocatorReallocate(array->alloc, array->elements, sizeof(struct CTBencodeValueContainer *) * array->count)));
    array->elements[index] = value;
}

CTBencodeValueContainer * CTBencodeValueContainerCreate(CTAllocator * alloc, void * value, int valueType)
{
    CTBencodeValueContainer * container = CTAllocatorAllocate(alloc, sizeof(CTBencodeValueContainer));
    container->alloc = alloc;
    container->value = value;
    container->valueType = valueType;
    
    return container;
}

CTString * CTBencodeExtractString(CTAllocator * alloc, CTString * bencodedString, uint64_t * start, CTError ** error)
{
    CTString * retVal = CTStringCreate(alloc, "");
    char * p = NULL;
    uint64_t length = strtoull(CTStringUTF8String(bencodedString) + *start, &p, 10);
    
    *start += p - CTStringUTF8String(bencodedString) - *start + 1;
    if (*start + length <= CTStringLength(bencodedString))
    {
        CTStringAppendCharacters(retVal, CTStringUTF8String(bencodedString) + *start, length);
    }
    *start += length;
    return retVal;
}

CTBencodeDictionary * CTBencodeExtractDictionary(CTAllocator * alloc, CTString * bencodedString, uint64_t * start, CTError ** error)
{
    ++*start;
    CTBencodeDictionary * dict = CTBencodeDictionaryCreate(alloc);
    while (*start < CTStringLength(bencodedString) && CTStringUTF8String(bencodedString)[*start] != 'e' && !*error)
    {
        CTString * string = CTBencodeParse(alloc, CTStringUTF8String(bencodedString), start, error)->value;
        CTBencodeValueContainer * value = CTBencodeParse(alloc, CTStringUTF8String(bencodedString), start, error);
        
        if (string && value)
        {
            CTBencodeDictionaryAddKeyValuePair(dict, string, value);
        }
        else
        {
            *error = CTErrorCreate(alloc, "Mismatched count of keys and values", 0);
        }
    }
    if(CTStringUTF8String(bencodedString)[*start] != 'e')
    {
        *error = CTErrorCreate(alloc, "No terminating e found for dictionary", 0);
    }
    ++*start;
    return dict;
}

CTBencodeList * CTBencodeExtractList(CTAllocator * alloc, CTString * bencodedString, uint64_t * start, CTError ** error)
{
    ++*start;
    CTBencodeList * list = CTBencodeListCreate(alloc);
    while (CTStringUTF8String(bencodedString)[*start] != 'e' && !*error)
    {
        CTBencodeListAddValueContainer(list, CTBencodeParse(alloc, CTStringUTF8String(bencodedString), start, error));
    }
    ++*start;
    return list;
}

CTNumber * CTBencodeExtractInteger(CTAllocator * alloc, CTString * bencodedString, uint64_t * start, CTError ** error)
{
    ++*start;
    char * p = NULL;
    double value = strtod(CTStringUTF8String(bencodedString) + *start, &p);
    *start += p - CTStringUTF8String(bencodedString) - *start + 1;
    return CTNumberCreateWithDouble(alloc, value);
}

CTBencodeValueContainer * CTBencodeParse(CTAllocator * alloc, const char * bencoded, uint64_t * start, CTError ** error)
{
    CTBencodeValueContainer * retVal = NULL;
    CTAllocator * lalloc = CTAllocatorCreate();
    CTString * bencodedString = CTStringCreate(lalloc, bencoded);
    
    if (CTStringLength(bencodedString))
    {
        if (*start < CTStringLength(bencodedString))
        {
            switch (bencoded[*start])
            {
                case 'd':
                    retVal = CTBencodeValueContainerCreate(alloc, CTBencodeExtractDictionary(alloc, bencodedString, start, error), CTBENCODE_TYPE_DICTIONARY);
                    break;
                case 'l':
                    retVal = CTBencodeValueContainerCreate(alloc, CTBencodeExtractList(alloc, bencodedString, start, error), CTBENCODE_TYPE_LIST);
                    break;
                case 'i':
                    retVal = CTBencodeValueContainerCreate(alloc, CTBencodeExtractInteger(alloc, bencodedString, start, error), CTBENCODE_TYPE_INTEGER);
                    break;
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
                    retVal = CTBencodeValueContainerCreate(alloc, CTBencodeExtractString(alloc, bencodedString,start, error), CTBENCODE_TYPE_STRING);
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