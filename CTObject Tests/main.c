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
    for (int i = 0; i < indentation; ++i) printf("\t");
    switch (type)
    {
        case CTOBJECT_TYPE_DICTIONARY:
        {
            printf("{\n");
            CTDictionary * object = (CTDictionary *)obj;
            for (unsigned long i = 0; i < object->count; ++i)
            {
                CTString * key = object->elements[i]->key;
                void * ptr = CTObjectValue(object->elements[i]->value);
                for (int i = 0; i <= indentation; ++i) printf("\t");
                switch (object->elements[i]->value->type)
                {
                    case CTOBJECT_TYPE_DICTIONARY:
                        printf("'%s' = \n", CTStringUTF8String(key));
                        recurseJSON(ptr, CTOBJECT_TYPE_DICTIONARY, indentation + 1);
                        break;
                    case CTOBJECT_TYPE_ARRAY:
                        printf("'%s' = \n", CTStringUTF8String(key));
                        recurseJSON(ptr, CTOBJECT_TYPE_ARRAY, indentation + 1);
                        break;
                    case CTOBJECT_TYPE_STRING:
                        printf("'%s' = '%s'\n", CTStringUTF8String(key), ((CTString *)ptr)->characters);
                        break;
                    case CTOBJECT_TYPE_NUMBER:
					{
						switch (((CTNumber *)ptr)->type)
						{
							case CTNUMBER_TYPE_DOUBLE:
								printf("'%s' = %Lf\n", CTStringUTF8String(key), ((CTNumber *)ptr)->value.Double);
								break;
							case CTNUMBER_TYPE_INT:
								printf("'%s' = %i\n", CTStringUTF8String(key), ((CTNumber *)ptr)->value.Int);
								break;
							case CTNUMBER_TYPE_UINT:
								printf("'%s' = %u\n", CTStringUTF8String(key), ((CTNumber *)ptr)->value.UInt);
								break;
							case CTNUMBER_TYPE_LONG:
								printf("'%s' = %lli\n", CTStringUTF8String(key), (long long)((CTNumber *)ptr)->value.Long);
								break;
							case CTNUMBER_TYPE_ULONG:
								printf("'%s' = %llu\n", CTStringUTF8String(key), (long long)((CTNumber *)ptr)->value.ULong);
								break;
						}
                        break;
					}
                    case CTOBJECT_TYPE_NULL:
                        printf("'%s' = %s\n", CTStringUTF8String(key), ((CTNull *)ptr)->value);
                        break;
					case CTOBJECT_TYPE_LARGE_NUMBER:
						printf("'%s' = ", CTStringUTF8String(key));
						switch (((CTLargeNumber *)ptr)->base->type)
						{
							case CTNUMBER_TYPE_DOUBLE:
								printf("%Lf", ((CTLargeNumber *)ptr)->base->value.Double);
								break;
							case CTNUMBER_TYPE_INT:
								printf("%i", ((CTLargeNumber *)ptr)->base->value.Int);
								break;
							case CTNUMBER_TYPE_UINT:
								printf("%u", ((CTLargeNumber *)ptr)->base->value.UInt);
								break;
							case CTNUMBER_TYPE_LONG:
								printf("%lli", (long long)((CTLargeNumber *)ptr)->base->value.Long);
								break;
							case CTNUMBER_TYPE_ULONG:
								printf("%llu", (long long)((CTLargeNumber *)ptr)->base->value.ULong);
								break;
						}
						printf("e%lli\n", (long long)((CTLargeNumber *)ptr)->exponent->value.Long);
						break;
				}
            }
            for (int i = 0; i < indentation; ++i) printf("\t");
            printf("}\n");
            break;
        }
            
        case CTOBJECT_TYPE_ARRAY:
        {
            printf ("[\n");
            CTArray * array = (CTArray *)obj;
            for (unsigned long i = 0; i < array->count; ++i)
            {
                if (array->elements[i]->type != CTOBJECT_TYPE_DICTIONARY && array->elements[i]->type != CTOBJECT_TYPE_ARRAY)
                    for (int i = 0; i <= indentation; ++i) printf("\t");
                void * ptr = array->elements[i]->ptr;
                switch (array->elements[i]->type)
                {
                    case CTOBJECT_TYPE_DICTIONARY:
                        recurseJSON(ptr, CTOBJECT_TYPE_DICTIONARY, indentation + 1);
                        break;
                    case CTOBJECT_TYPE_ARRAY:
                        recurseJSON(ptr, CTOBJECT_TYPE_ARRAY, indentation + 1);
                        break;
                    case CTOBJECT_TYPE_STRING:
                        printf("'%s'\n", ((CTString *)ptr)->characters);
                        break;
                    case CTOBJECT_TYPE_NUMBER:
					{
						switch (((CTNumber *)ptr)->type)
						{
							case CTNUMBER_TYPE_DOUBLE:
								printf("%Lf\n", ((CTNumber *)ptr)->value.Double);
								break;
							case CTNUMBER_TYPE_INT:
								printf("%i\n", ((CTNumber *)ptr)->value.Int);
								break;
							case CTNUMBER_TYPE_UINT:
								printf("%u\n", ((CTNumber *)ptr)->value.UInt);
								break;
							case CTNUMBER_TYPE_LONG:
								printf("%lli\n", (long long)((CTNumber *)ptr)->value.Long);
								break;
							case CTNUMBER_TYPE_ULONG:
								printf("%llu\n", (long long)((CTNumber *)ptr)->value.ULong);
								break;
						}
                        break;
					}
					case CTOBJECT_TYPE_LARGE_NUMBER:
						switch (((CTLargeNumber *)ptr)->base->type)
						{
							case CTNUMBER_TYPE_DOUBLE:
								printf("%Lf", ((CTLargeNumber *)ptr)->base->value.Double);
								break;
							case CTNUMBER_TYPE_INT:
								printf("%i", ((CTLargeNumber *)ptr)->base->value.Int);
								break;
							case CTNUMBER_TYPE_UINT:
								printf("%u", ((CTLargeNumber *)ptr)->base->value.UInt);
								break;
							case CTNUMBER_TYPE_LONG:
								printf("%lli", (long long)((CTLargeNumber *)ptr)->base->value.Long);
								break;
							case CTNUMBER_TYPE_ULONG:
								printf("%llu", (long long)((CTLargeNumber *)ptr)->base->value.ULong);
								break;
						}
						printf("e%lli\n", (long long)((CTLargeNumber *)ptr)->exponent->value.Long);
						break;
                    case CTOBJECT_TYPE_NULL:
                        printf("'%s'\n", ((CTNull *)ptr)->value);
                        break;
                }
            }
            for (int i = 0; i < indentation; ++i) printf("\t");
            printf("]\n");
            break;
        }
    }
}

void recurseBencode(CTObject * obj, int indentation)
{
    for (int i = 0; i < indentation; ++i) printf("\t");
    switch (obj->type)
    {
        case CTOBJECT_TYPE_DICTIONARY:
        {
            printf("{\n");
            CTDictionary * object = CTObjectValue(obj);
            for (unsigned long i = 0; i < object->count; ++i)
            {
                CTString * key = object->elements[i]->key;
                void * ptr = CTObjectValue(object->elements[i]->value);
                for (int i = 0; i <= indentation; ++i)
                    printf("\t");
                switch (object->elements[i]->value->type)
                {
                    case CTOBJECT_TYPE_DICTIONARY:
                    case CTOBJECT_TYPE_ARRAY:
                        printf("'%s' = \n", CTStringUTF8String(key));
                        recurseBencode(object->elements[i]->value, indentation + 1);
                        break;
                    case CTOBJECT_TYPE_STRING:
                        printf("'%s' = '%s'\n", CTStringUTF8String(key), ((CTString *)ptr)->characters);
                        break;
                    case CTOBJECT_TYPE_NUMBER:
                        printf("'%s' = '%i'\n", CTStringUTF8String(key), ((CTNumber *)ptr)->value.Int);
                        break;
                }
            }
            for (int i = 0; i < indentation; ++i) printf("\t");
            printf("}\n");
            break;
        }
            
        case CTOBJECT_TYPE_ARRAY:
        {
            printf ("[\n");
            CTArray * array = CTObjectValue(obj);
            for (unsigned long i = 0; i < array->count; ++i)
            {
                if (array->elements[i]->type != CTOBJECT_TYPE_DICTIONARY && array->elements[i]->type != CTOBJECT_TYPE_ARRAY)
                    for (int i = 0; i <= indentation; ++i) printf("\t");
                void * ptr = array->elements[i]->ptr;
                switch (array->elements[i]->type)
                {
                    case CTOBJECT_TYPE_DICTIONARY:
                    case CTOBJECT_TYPE_ARRAY:
                        recurseBencode(array->elements[i], indentation + 1);
                        break;
                    case CTOBJECT_TYPE_STRING:
                        printf("'%s'\n", ((CTString *)ptr)->characters);
                        break;
                    case CTOBJECT_TYPE_NUMBER:
                        printf("'%i'\n", ((CTNumber *)ptr)->value.Int);
                        break;
                }
            }
            for (int i = 0; i < indentation; ++i) printf("\t");
            printf("]\n");
            break;
        }
        case CTOBJECT_TYPE_STRING:
        {
            printf("'%s'\n", CTStringUTF8String(CTObjectValue(obj)));
            break;
        }
            
        case CTOBJECT_TYPE_NUMBER:
        {
            printf("'%lli'\n", CTNumberLongValue(CTObjectValue(obj)));
            break;
        }
    }
}

void CTArrayTests()
{
	{
		CTAllocator * allocator = CTAllocatorCreate();
		CTError * error = NULL;
		CTObject * array1 = CTJSONParse(allocator, "['a', 1, '2', 2, [], {}, 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		CTObject * array2 = CTJSONParse(allocator, "[1, '2', [], {}, 2, 'a', 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		assert(CTArrayCompare(CTObjectValue(array1), CTObjectValue(array2)));
		
		recurseJSON(array1->ptr, CTOBJECT_TYPE_ARRAY, 0);
		
		assert(CTArrayIndexOfEntryByValue(CTObjectValue(array1), CTObjectWithString(allocator, CTStringCreate(allocator, "a"))) == 0);
		assert(CTArrayIndexOfEntryByValue(CTObjectValue(array1), CTObjectWithNumber(allocator, CTNumberCreateWithLong(allocator, 1))) == 1);
		assert(CTArrayIndexOfEntryByValue(CTObjectValue(array1), CTObjectWithString(allocator, CTStringCreate(allocator, "2"))) == 2);
		assert(CTArrayIndexOfEntryByValue(CTObjectValue(array1), CTObjectWithNumber(allocator, CTNumberCreateWithLong(allocator, 2))) == 3);
		assert(CTArrayIndexOfEntryByValue(CTObjectValue(array1), CTObjectWithArray(allocator, CTArrayCreate(allocator))) == 4);
		assert(CTArrayIndexOfEntryByValue(CTObjectValue(array1), CTObjectWithDictionary(allocator, CTDictionaryCreate(allocator))) == 5);
		assert(CTArrayIndexOfEntryByValue(CTObjectValue(array1), CTObjectWithLargeNumber(allocator, CTLargeNumberCreate(allocator, CTNumberCreateWithLong(allocator, 1343), CTNumberCreateWithLong(allocator, 380)))) == 6);
		CTAllocatorRelease(allocator);
	}
	{
		CTAllocator * allocator = CTAllocatorCreate();
		CTError * error = NULL;
		CTObject * array1 = CTJSONParse(allocator, "['a', 1, '2', 2, [1, 2, '4', 3, '5'], {'1':1, '2':2, '3':3, '4':4, '5':5}, 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		CTObject * array2 = CTJSONParse(allocator, "[1, '2', [1, 2, 3, '4', '5'], {'5':5, '1':1, '2':2, '4':4, '3':3}, 2, 'a', 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		assert(CTArrayCompare(CTObjectValue(array1), CTObjectValue(array2)));
		CTAllocatorRelease(allocator);
	}
	{
		CTAllocator * allocator = CTAllocatorCreate();
		CTError * error = NULL;
		CTObject * array1 = CTJSONParse(allocator, "['a', 1, '2', 2, [1, 2, '4', 3, '5'], {'1':1, '2':2, '3':3, '4':4, '5':5}, 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		CTObject * array2 = CTJSONParse(allocator, "[1, '2', [1, 2, 3, '4', '5'], {'5':3, '1':1, '2':2, '4':4, '3':3}, 2, 'a', 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		assert(!CTArrayCompare(CTObjectValue(array1), CTObjectValue(array2)));
		CTAllocatorRelease(allocator);
	}
	{
		CTAllocator * allocator = CTAllocatorCreate();
		CTError * error = NULL;
		CTObject * array1 = CTJSONParse(allocator, "['a', 1, '2', 2, [1, 2, '4', 3, '5'], {'1':1, '2':2, '3':3, '4':4, '5':5}, 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		CTObject * array2 = CTJSONParse(allocator, "[1, '2', [1, 2, 3, '4', '5'], {'1':1, '2':2, '4':4, '3':3}, 2, 'a', 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		assert(!CTArrayCompare(CTObjectValue(array1), CTObjectValue(array2)));
		CTAllocatorRelease(allocator);
	}
	{
		CTAllocator * allocator = CTAllocatorCreate();
		CTError * error = NULL;
		CTObject * array1 = CTJSONParse(allocator, "['a', 1, '2', 2, [1, 2, '4', 3, '5'], {'1':1, '2':2, '3':3, '4':4, '5':5}, 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		CTObject * array2 = CTJSONParse(allocator, "[1, '2', [1, 2, 3, '4', '5'], {'5':5, '1':1, '2':2, '4':4, '3':3}, 2, 'a', 1343.0e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		assert(!CTArrayCompare(CTObjectValue(array1), CTObjectValue(array2)));
		CTAllocatorRelease(allocator);
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
    
    for (int i = 0; i < 0x10; ++i)
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
	CTArrayTests();
    CTArray * array = CTArrayCreate(allocator);
    
    for (int i = 0; i < 0x10; ++i)
    {
        CTArrayAddEntry(array, testStrings[i], CTOBJECT_NOT_AN_OBJECT);
    }
    
    assert(array->count == 0x10);
    assert(CTArrayIndexOfEntryByReference(array, (void *)"not found in array") == CT_NOT_FOUND);
    
    for (int i = 0; i < 0x10; ++i)
    {
        assert(strcmp(array->elements[i]->ptr, testStrings[i]) == 0);
    }
    
    for (int i = 0xF; i >= 0x0; --i)
    {
        CTArrayDeleteEntry(array, i);
    }
    
    assert(array->count == 0x0);
    
#pragma mark - CTDictionary Test Begin
    CTDictionary * dict = CTDictionaryCreate(allocator);
    
    for (int i = 0; i < 0x10; ++i)
    {
        CTDictionaryAddEntry(dict, testStrings[i], CTObjectCreate(allocator, testStrings2[i], CTOBJECT_NOT_AN_OBJECT));
    }
    
    assert(dict->count == 0x10);
    assert(CTDictionaryIndexOfEntry(dict, "not found in dictionary") == CT_NOT_FOUND);
    assert(CTDictionaryValueForKey(dict, "not found in dictionary") == NULL);
    
    for (int i = 0; i < 0x10; ++i)
    {
        assert(CTDictionaryIndexOfEntry(dict, testStrings[i]) == i);
        assert(strcmp(CTDictionaryValueForKey(dict, testStrings[i])->ptr, testStrings2[i]) == 0);
    }
    
    for (int i = 0; i < 0x10; ++i)
    {
        CTDictionaryDeleteEntry(dict, testStrings[i]);
    }
    
    assert(dict->count == 0x0);
    
#pragma mark - CTString Test Begin
    char * stringTest = "Test of string";
    char * prepend = "Prepended Characters. ";
    char * append = ". Appended Characters";
    
    CTString * string = CTStringCreate(allocator, stringTest);
	printf("%llu\n", CTStringHash(string));
    CTStringAppendCharacters(string, append, CTSTRING_NO_LIMIT);
	printf("%llu\n", CTStringHash(string));
    CTStringPrependCharacters(string, prepend, CTSTRING_NO_LIMIT);
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
	CTArrayAddEntry(array, "{}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"v\":\"1\"}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"v\":\"1\"\r\n}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"v\":1}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"v\":\"ab'c\"}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"PI\":323.141E-6}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"PI\":3.141E-10}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"PI\":3.1413e-10}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"v\":12345123456789}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"v\":123456789123456789123456789}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"v\":[ 0,-1,2,3,4]}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"v\":[ \"1\",\"2\",\"3\",\"4\"]}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"v\":[ { \n}, { },[]]}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"v\":\"\u03bc\u00bf\"}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"v\":\"\u00B1\u00B6\"}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"a\":\"hp://foo\"}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"a\":null}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"a\":true}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"a\":false}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"a\" : true }", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{ \"v\":1.797693134E308}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{\"menu\": {\"header\": \"SVG Viewer\",\"items\": [1.7, true, false, {\"id\": \"Open\"},{\"id\": \"OpenNew\", \"label\": \"Open New\"},null,{\"id\": \"ZoomIn\", \"label\": \"Zoom In\"},{\"id\": \"ZoomOut\", \"label\": \"Zoom Out\"},{\"id\": \"OriginalView\", \"label\": \"Original View\"},null,{\"id\": \"Quality\"},{\"id\": \"Pause\"},{\"id\": \"Mute\"},null,{\"id\": \"Find\", \"label\": \"Find...\"},{\"id\": \"FindAgain\", \"label\": \"Find Again\"},{\"id\": \"Copy\"},{\"id\": \"CopyAgain\", \"label\": \"Copy Again\"},{\"id\": \"CopySVG\", \"label\": \"Copy SVG\"},{\"id\": \"ViewSVG\", \"label\": \"View SVG\"},{\"id\": \"ViewSource\", \"label\": \"View Source\"},{\"id\": \"SaveAs\", \"label\": \"Save As\"},null,{\"id\": \"Help\"},{\"id\": \"About\", \"label\": \"About Adobe CVG Viewer...\"}]}}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "\"hello\"", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "1.0e7", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "1", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{\"method\":\"runcommand\",\"params\":[{\"type\":\"target\",\"line\":[1]}, {\"type\":\"indirect\",\"value\":\"on\"}],\"id\":59916}", CTOBJECT_NOT_AN_OBJECT);
    
    CTError * error = NULL;
	for (int i = 0; i < array->count; ++i)
	{
		CTObject * dict = CTJSONParse(allocator, array->elements[i]->ptr, 0, &error);
		recurseJSON(CTObjectValue(dict), dict->type, 0);
		assert(!error);
		CTJSONSerialise(allocator, dict, 0, &error);
		assert(!error);
	}
    CTArrayEmpty(array);
	
	CTArrayAddEntry(array, "", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{'X':'s", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{{\"k\":\"v\"}}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "{\"l\":[\"e\",\"]}", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "1.3.3", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "01", CTOBJECT_NOT_AN_OBJECT);
	for (int i = 0; i < array->count; ++i)
	{
		error = NULL;
		CTAllocator * allocll = CTAllocatorCreate();
		CTJSONParse(allocll, CTObjectValue(CTArrayObjectAtIndex(array, i)), 0, &error);
		assert(error);
		CTAllocatorRelease(allocll);
	}
#pragma mark - CTBencode Test Begin
    CTArrayEmpty(array);
	
    CTArrayAddEntry(array, "de", CTOBJECT_NOT_AN_OBJECT);
    CTArrayAddEntry(array, "li7483ee", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "d4:yololllleeeleli720eeli-230eld4:hulli-233eeeeee", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "d4:yoloi3ee", CTOBJECT_NOT_AN_OBJECT);
	CTArrayAddEntry(array, "i-3240.0e", CTOBJECT_NOT_AN_OBJECT);
    CTArrayAddEntry(array, "l0:e", CTOBJECT_NOT_AN_OBJECT);
    CTArrayAddEntry(array, "5:12345", CTOBJECT_NOT_AN_OBJECT);
	
    error = NULL;
	for (int i = 0; i < array->count; ++i)
	{
		CTObject * obj = CTBencodeParse(allocator, CTArrayObjectAtIndex(array, i)->ptr, &error);
		assert(!error);
        CTString * string = CTBencodeSerialise(allocator, obj, &error);
		puts(string->characters);
		assert(!error);
	}
    
    CTArrayEmpty(array);
	CTArrayAddEntry(array, "", CTOBJECT_NOT_AN_OBJECT);
    CTArrayAddEntry(array, "d", CTOBJECT_NOT_AN_OBJECT);
    CTArrayAddEntry(array, "l", CTOBJECT_NOT_AN_OBJECT);
    CTArrayAddEntry(array, "di0ee", CTOBJECT_NOT_AN_OBJECT);
    for (int i = 0; i < array->count; ++i)
	{
		error = NULL;
        CTBencodeParse(allocator, CTArrayObjectAtIndex(array, i)->ptr, &error);
		assert(error);
        fprintf(stderr, "%s\n", CTStringUTF8String(CTErrorGetErrorString(error)));
        CTErrorRelease(error);
	}
    
#pragma mark - CTBencode Test End
    
    CTArrayRelease(array);
    CTAllocatorRelease(CTAllocatorGetDefault());
	printf("%llu, %llu\n", CTStringHash(CTStringCreate(allocator, "a")), CTStringHash(CTStringCreate(allocator, "yolo")));
    CTAllocatorRelease(allocator);
    
#pragma mark - CTAllocator Test End
    printf("%.0f µseconds (%lu ticks)\n", ((clock() - t) / (double)CLOCKS_PER_SEC) * 1e6, clock() - t);
    return 0;
}