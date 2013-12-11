//
//  main.c
//  CTObject Tests
//
//  Created by Carlo Tortorella on 23/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTPrelude.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

void recurseJSON(void * obj, int type, int indentation)
{
    for (int i = 0; i < indentation; i++)
        printf("\t");
    switch (type)
    {
        case CTJSON_TYPE_OBJECT:
        {
            printf("{\n");
            CTJSONObject * object = (CTJSONObject *)obj;
            for (unsigned long i = 0; i < object->count; i++)
            {
                CTString * key = object->elements[i]->key;
                void * ptr = object->elements[i]->value->ptr;
                for (int i = 0; i < indentation + 1; i++)
                    printf("\t");
                switch (object->elements[i]->valueType)
                {
                    case CTJSON_TYPE_OBJECT:
                        printf("'%s' = \n", CTStringUTF8String(key));
                        recurseJSON(ptr, CTJSON_TYPE_OBJECT, indentation + 1);
                        break;
                    case CTJSON_TYPE_ARRAY:
                        printf("'%s' = \n", CTStringUTF8String(key));
                        recurseJSON(ptr, CTJSON_TYPE_ARRAY, indentation + 1);
                        break;
                    case CTJSON_TYPE_STRING:
                        printf("'%s' = '%s'\n", CTStringUTF8String(key), ((CTString *)ptr)->characters);
                        break;
                    case CTJSON_TYPE_BOOLEAN:
                        printf("'%s' = '%u'\n", CTStringUTF8String(key), ((CTNumber *)ptr)->value.UInt);
                        break;
                    case CTJSON_TYPE_DOUBLE:
                        printf("'%s' = '%Lf'\n", CTStringUTF8String(key), ((CTNumber *)ptr)->value.Double);
                        break;
                    case CTJSON_TYPE_LONG:
                        printf("'%s' = '%lli'\n", CTStringUTF8String(key), (long long)((CTNumber *)ptr)->value.Long);
                        break;
                    case CTJSON_TYPE_NULL:
                        printf("'%s' = %s\n", CTStringUTF8String(key), ((CTNull *)ptr)->value);
                        break;
                    case CTJSON_TYPE_LARGE_NUMBER:
                        printf("'%s' = %Lfe%lli\n", CTStringUTF8String(key), ((CTLargeNumber *)ptr)->base->value.Double, (long long)((CTLargeNumber *)ptr)->exponent->value.Long);
                        break;
                }
            }
            for (int i = 0; i < indentation; i++)
                printf("\t");
            printf("}\n");
            break;
        }
            
        case CTJSON_TYPE_ARRAY:
        {
            printf ("[\n");
            CTJSONArray * array = (CTJSONArray *)obj;
            for (unsigned long i = 0; i < array->count; i++)
            {
                if (array->elements[i]->valueType != CTJSON_TYPE_OBJECT && array->elements[i]->valueType != CTJSON_TYPE_ARRAY)
                    for (int i = 0; i < indentation + 1; i++)
                        printf("\t");
                void * ptr = array->elements[i]->value->ptr;
                switch (array->elements[i]->valueType)
                {
                    case CTJSON_TYPE_OBJECT:
                        recurseJSON(ptr, CTJSON_TYPE_OBJECT, indentation + 1);
                        break;
                    case CTJSON_TYPE_ARRAY:
                        recurseJSON(ptr, CTJSON_TYPE_ARRAY, indentation + 1);
                        break;
                    case CTJSON_TYPE_STRING:
                        printf("'%s'\n", ((CTString *)ptr)->characters);
                        break;
                    case CTJSON_TYPE_BOOLEAN:
                        printf("'%u'\n", ((CTNumber *)ptr)->value.UInt);
                        break;
                    case CTJSON_TYPE_DOUBLE:
                        printf("'%Lf'\n", ((CTNumber *)ptr)->value.Double);
                        break;
                    case CTJSON_TYPE_LONG:
                        printf("'%lli'\n", (long long)((CTNumber *)ptr)->value.Long);
                        break;
                    case CTJSON_TYPE_NULL:
                        printf("'%s'\n", ((CTNull *)ptr)->value);
                        break;
                }
            }
            for (int i = 0; i < indentation; i++)
                printf("\t");
            printf("]\n");
            break;
        }
    }
}

void recurseBencode(CTBencodeValueContainer * obj, int indentation)
{
    for (int i = 0; i < indentation; i++)
        printf("\t");
    switch (obj->valueType)
    {
        case CTBENCODE_TYPE_DICTIONARY:
        {
            printf("{\n");
            CTBencodeDictionary * object = obj->value;
            for (unsigned long i = 0; i < object->count; i++)
            {
                CTString * key = object->elements[i]->key;
                void * ptr = object->elements[i]->value->value;
                for (int i = 0; i < indentation + 1; i++)
                    printf("\t");
                switch (object->elements[i]->value->valueType)
                {
                    case CTBENCODE_TYPE_DICTIONARY:
                    case CTBENCODE_TYPE_LIST:
                        printf("'%s' = \n", CTStringUTF8String(key));
                        recurseBencode(object->elements[i]->value, indentation + 1);
                        break;
                    case CTBENCODE_TYPE_STRING:
                        printf("'%s' = '%s'\n", CTStringUTF8String(key), ((CTString *)ptr)->characters);
                        break;
                    case CTBENCODE_TYPE_INTEGER:
                        printf("'%s' = '%i'\n", CTStringUTF8String(key), ((CTNumber *)ptr)->value.Int);
                        break;
                }
            }
            for (int i = 0; i < indentation; i++)
                printf("\t");
            printf("}\n");
            break;
        }
            
        case CTBENCODE_TYPE_LIST:
        {
            printf ("[\n");
            CTBencodeList * array = obj->value;
            for (unsigned long i = 0; i < array->count; i++)
            {
                if (array->elements[i]->valueType != CTBENCODE_TYPE_DICTIONARY && array->elements[i]->valueType != CTBENCODE_TYPE_LIST)
                    for (int i = 0; i < indentation + 1; i++)
                        printf("\t");
                void * ptr = array->elements[i]->value;
                switch (array->elements[i]->valueType)
                {
                    case CTBENCODE_TYPE_DICTIONARY:
                    case CTBENCODE_TYPE_LIST:
                        recurseBencode(array->elements[i], indentation + 1);
                        break;
                    case CTBENCODE_TYPE_STRING:
                        printf("'%s'\n", ((CTString *)ptr)->characters);
                        break;
                    case CTBENCODE_TYPE_INTEGER:
                        printf("'%i'\n", ((CTNumber *)ptr)->value.Int);
                        break;
                }
            }
            for (int i = 0; i < indentation; i++)
                printf("\t");
            printf("]\n");
            break;
        }
        case CTBENCODE_TYPE_STRING:
        {
            printf("'%s'\n", CTStringUTF8String(obj->value));
            break;
        }
            
        case CTBENCODE_TYPE_INTEGER:
        {
            printf("'%lli'\n", CTNumberGetLongValue(obj->value));
            break;
        }
    }
}

int main(int argc, const char * argv[])
{
    clock_t t = clock();
#pragma mark - CTAllocator Test Begin
    CTAllocator * allocator = CTAllocatorCreate();
    CTAllocatorRelease(allocator);
    allocator = CTAllocatorCreate();
    
    char ** testStrings = CTAllocatorAllocate(allocator, sizeof(void *) * 0x10);
    char ** testStrings2 = CTAllocatorAllocate(allocator, sizeof(void *) * 0x10);
    
    for (int i = 0; i < 0x10; i++)
    {
        char string[0x10];
        char string2[0x10];
        memset(string, 0, 0x10);
        sprintf(string, "Test%i", i);
        sprintf(string2, "Test%i", i + 0x10);
        testStrings[i] = stringDuplicate(allocator, string);
        testStrings2[i] = stringDuplicate(allocator, string);
    }
    
#pragma mark - CTArray Test Begin
    CTArray * array = CTArrayCreate(allocator);
    
    for (int i = 0; i < 0x10; i++)
    {
        CTArrayAddEntry(array, testStrings[i]);
    }
    
    assert(array->count == 0x10);
    assert(CTArrayIndexOfEntry(array, "not found in array") == -1);
    
    for (int i = 0; i < 0x10; i++)
    {
        assert(CTArrayIndexOfEntry(array, testStrings[i]) == i);
        assert(strcmp(array->elements[i]->characters, testStrings[i]) == 0);
    }
    
    for (int i = 0xF; i >= 0x0; i--)
    {
        CTArrayDeleteEntry(array, i);
    }
    
    assert(array->count == 0x0);
    
#pragma mark - CTDictionary Test Begin
    CTDictionary * dict = CTDictionaryCreate(allocator);
    
    for (int i = 0; i < 0x10; i++)
    {
        CTDictionaryAddEntry(dict, testStrings[i], testStrings2[i]);
    }
    
    assert(dict->count == 0x10);
    assert(CTDictionaryIndexOfEntry(dict, "not found in dictionary") == -1);
    assert(CTDictionaryValueForKey(dict, "not found in dictionary") == NULL);
    
    for (int i = 0; i < 0x10; i++)
    {
        assert(CTDictionaryIndexOfEntry(dict, testStrings[i]) == i);
        assert(strcmp(CTDictionaryValueForKey(dict, testStrings[i])->characters, testStrings2[i]) == 0);
    }
    
    for (int i = 0; i < 0x10; i++)
    {
        CTDictionaryDeleteEntry(dict, testStrings[i]);
    }
    
    assert(dict->count == 0x0);
    
#pragma mark - CTString Test Begin
    char * stringTest = "Test of string";
    char * prepend = "Prepended Characters. ";
    char * append = ". Appended Characters";
    
    CTString * string = CTStringCreate(allocator, stringTest);
    CTStringAppendCharacters(string, append, -1);
    CTStringPrependCharacters(string, prepend, -1);
    assert(strcmp(CTStringUTF8String(string), "Prepended Characters. Test of string. Appended Characters") == 0);
    assert(strcmp(CTStringStringBetween(string, prepend, append), "Test of string") == 0);
    assert(CTStringStringBetween(string, append, prepend) == NULL);
    assert(CTStringStringBetween(string, "not found", "in string") == NULL);
    CTStringRemoveCharactersFromEnd(string, strlen(append));
    assert(strcmp(CTStringUTF8String(string), "Prepended Characters. Test of string") == 0);
    CTStringRemoveCharactersFromStart(string, strlen(prepend));
    assert(strcmp(CTStringUTF8String(string), stringTest) == 0);
    
#pragma mark - CTNumber Test Begin
    CTNumber * number = CTNumberCreateWithLong(allocator, 0xFF);
    assert((number->value.Int & number->value.UInt & number->value.ULong & number->value.Long) == 0xFF);
    CTNumberSetDoubleValue(number, 255.5);
    assert(number->value.Double == 255.5);
    
#pragma mark - CTJSON Test Begin
    
    CTAllocatorRelease(allocator);
    allocator = CTAllocatorCreate();
    
    array = CTArrayCreate(allocator);
	CTArrayAddEntry(array, "{}");
	CTArrayAddEntry(array, "{ \"v\":\"1\"}");
	CTArrayAddEntry(array, "{ \"v\":\"1\"\r\n}");
	CTArrayAddEntry(array, "{ \"v\":1}");
	CTArrayAddEntry(array, "{ \"v\":\"ab'c\"}");
	CTArrayAddEntry(array, "{ \"PI\":3.141E-10}");
	CTArrayAddEntry(array, "{ \"PI\":3.1413e-10}");
	CTArrayAddEntry(array, "{ \"v\":12345123456789}");
	CTArrayAddEntry(array, "{ \"v\":123456789123456789123456789}");
	CTArrayAddEntry(array, "{ \"v\":[ 1,2,3,4]}");
	CTArrayAddEntry(array, "{ \"v\":[ \"1\",\"2\",\"3\",\"4\"]}");
	CTArrayAddEntry(array, "{ \"v\":[ { \n}, { },[]]}");
	CTArrayAddEntry(array, "{ \"v\":\"\u03bc\u00bf\"}");
	CTArrayAddEntry(array, "{ \"v\":\"\u00B1\u00B6\"}");
	CTArrayAddEntry(array, "{ \"a\":\"hp://foo\"}");
	CTArrayAddEntry(array, "{ \"a\":null}");
	CTArrayAddEntry(array, "{ \"a\":true}");
	CTArrayAddEntry(array, "{ \"a\":false}");
	CTArrayAddEntry(array, "{ \"a\" : true }");
	CTArrayAddEntry(array, "{ \"v\":1.797693134E308}");
    
    CTError * error = NULL;
    CTJSONObject * object = NULL;
	for (int i = 0; i < array->count; i++)
	{
		object = CTJSONParse(allocator, array->elements[i]->characters, &error);
		assert(!error);
        //Comment this line if you don't want to see the output and/or do not want the memory leak associated with it.
        //recurseJSON(object, CTJSON_TYPE_OBJECT, 0);
	}
    CTArrayEmpty(array);
    
	CTArrayAddEntry(array, "");
	CTArrayAddEntry(array, "{'X':'s");
	CTArrayAddEntry(array, "{{\"k\":\"v\"}\"e\"}");
	CTArrayAddEntry(array, "[]");
	CTArrayAddEntry(array, "{\"k\":[]\"}");
	CTArrayAddEntry(array, "{\"k\":[\"\", \"]\"}");
	for (int i = 0; i < array->count; i++)
	{
		error = NULL;
		CTJSONParse(CTAllocatorGetDefault(), CTStringUTF8String(CTArrayObjectAtIndex(array, i)), &error);
		assert(error);
        printf("%s\n", CTStringUTF8String(CTErrorGetError(error)));
        CTErrorRelease(error);
	}
#pragma mark - CTBencode Test Begin
    
    CTArrayEmpty(array);
    CTArrayAddEntry(array, "de");
    CTArrayAddEntry(array, "li7483ee");
	CTArrayAddEntry(array, "d4:yololllleeeleli720eeli-230eld4:hulli-233eeeeee");
	CTArrayAddEntry(array, "d4:yoloi3ee");
	CTArrayAddEntry(array, "i-3240.0e");
    CTArrayAddEntry(array, "l0:e");
	for (int i = 0; i < array->count; i++)
	{
        uint64_t start = 0;
		error = NULL;
        CTBencodeParse(allocator, CTStringUTF8String(CTArrayObjectAtIndex(array, i)), &start, &error);
        //recurseBencode(CTBencodeParse(allocator, CTStringUTF8String(CTArrayObjectAtIndex(array, i)), &start, &error), 0);
		assert(!error);
	}
    
    CTArrayEmpty(array);
    CTArrayAddEntry(array, "");
    CTArrayAddEntry(array, "d");
    CTArrayAddEntry(array, "di0ee");
    for (int i = 0; i < array->count; i++)
	{
        uint64_t start = 0;
		error = NULL;
        CTBencodeParse(allocator, CTStringUTF8String(CTArrayObjectAtIndex(array, i)), &start, &error);
		assert(error);
        printf("%s\n", CTStringUTF8String(CTErrorGetError(error)));
        CTErrorRelease(error);
	}
    
#pragma mark - CTBencode Test End
    
    CTArrayRelease(array);
    CTAllocatorRelease(CTAllocatorGetDefault());
    CTAllocatorRelease(allocator);
    
#pragma mark - CTAllocator Test End
    printf("%lu ticks\n", clock() - t);
    return 0;
}