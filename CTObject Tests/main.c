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
            printf ("{\n");
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
                        printf("'%s' = \n", key->characters);
                        recurseJSON(ptr, CTJSON_TYPE_OBJECT, indentation + 1);
                        break;
                    case CTJSON_TYPE_ARRAY:
                        printf("'%s' = \n", key->characters);
                        recurseJSON(ptr, CTJSON_TYPE_ARRAY, indentation + 1);
                        break;
                    case CTJSON_TYPE_STRING:
                        printf("'%s' = '%s'\n", key->characters, ((CTString *)ptr)->characters);
                        break;
                    case CTJSON_TYPE_BOOLEAN:
                        printf("'%s' = '%u'\n", key->characters, ((CTNumber *)ptr)->value.UInt);
                        break;
                    case CTJSON_TYPE_DOUBLE:
                        printf("'%s' = '%Lf'\n", key->characters, ((CTNumber *)ptr)->value.Double);
                        break;
                    case CTJSON_TYPE_LONG:
                        printf("'%s' = '%li'\n", key->characters, ((CTNumber *)ptr)->value.Long);
                        break;
                    case CTJSON_TYPE_NULL:
                        printf("'%s' = %s\n", key->characters, ((CTNull *)ptr)->value);
                        break;
                }
            }
            for (int i = 0; i < indentation; i++)
                printf("\t");
            printf ("}\n");
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
                        printf("'%li'\n", ((CTNumber *)ptr)->value.Long);
                        break;
                    case CTJSON_TYPE_NULL:
                        printf("'%s'\n", ((CTNull *)ptr)->value);
                        break;
                }
            }
            for (int i = 0; i < indentation; i++)
                printf("\t");
            printf ("]\n");
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
    CTStringAppendCharacters(string, append);
    CTStringPrependCharacters(string, prepend);
    assert(strcmp(string->characters, "Prepended Characters. Test of string. Appended Characters") == 0);
    assert(strcmp(CTStringStringBetween(string, prepend, append), "Test of string") == 0);
    assert(CTStringStringBetween(string, append, prepend) == NULL);
    assert(CTStringStringBetween(string, "not found", "in string") == NULL);
    CTStringRemoveCharactersFromEnd(string, strlen(append));
    assert(strcmp(string->characters, "Prepended Characters. Test of string") == 0);
    CTStringRemoveCharactersFromStart(string, strlen(prepend));
    assert(strcmp(string->characters, stringTest) == 0);
    
#pragma mark - CTNumber Test Begin
    CTNumber * number = CTNumberCreateWithLong(allocator, 0xFF);
    assert((number->value.Int & number->value.UInt & number->value.ULong & number->value.Long) == 0xFF);
    number->value.Double = 255.5;
    assert(number->value.Double == 255.5);
    
#pragma mark - CTJSON Test Begin
    CTError * error = NULL;
    CTJSONObject * object = CTJSONParse(allocator, "    { \"hello\":  \"Yes\",    \"parser\":\"yay!\", \"life\":null, \"Heyo\":true, \"keylo\":false, \"another key\":1278e-2, \"a.key\":{\"nested, yo\":\"yes\"}, \"can I help you?\":\"Yes\", \"keyvalueyo\":[1E8, 12e1, \"\\u0012\", true, false, null, \"yes\", {}, []], \"ha\":[[[[[[[[[[{\"So you found me\":\"congratulations\"}]]]]]]]]]]}", &error);
    assert(!error);
    recurseJSON(object, CTJSON_TYPE_OBJECT, 0);
    
    error = NULL;
    CTJSONParse(allocator, "{ \"hello\":  \"Yes\"'}", &error);
    assert(error);
    
    error = NULL;
    CTJSONParse(allocator, "{ \"hello\":;  \"Yes\"}", &error);
    assert(error);
    
    error = NULL;
    CTJSONParse(allocator, "{ \"hello\":  \"Yes\"'}", &error);
    assert(error);
    
    error = NULL;
    object = CTJSONParse(allocator, "    { \"hello\":  \"Yes\"\"e\",    \"parser\":\"yay!\", \"life\":null, \"Heyo\":true, \"keylo\":false, \"another key\":1278e-2, \"a.key\":{\"nested, yo\":\"yes\"}, \"can I help you?\":\"Yes\", \"keyvalueyo\":[1E8, 12e1, \"\\u0012\", true, false, null, \"yes\", {}, []], \"ha\":[[[[[[[[[[{\"So you found me\":\"congratulations\"}]]]]]]]]]\"e\"]}", &error);
    assert(error);
    
    error = NULL;
    CTJSONParse(allocator, "{\"howdy\":{ \"hello\":  \"Yes\"}\"}", &error);
    assert(error);
    
    CTAllocatorRelease(allocator);
#pragma mark - CTAllocator Test End
    
    printf("%f seconds (%lu ticks)\n", (clock() - t) / (double)CLOCKS_PER_SEC, clock() - t);
    return 0;
}