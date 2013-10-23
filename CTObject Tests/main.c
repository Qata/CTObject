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

int main(int argc, const char * argv[])
{
    clock_t t = clock();
#pragma mark - CTAllocator Test Begin
    CTAllocator * allocator = CTAllocatorCreate();
    
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
        assert(strcmp(array->values[i]->characters, testStrings[i]) == 0);
    }
    
    for (int i = 0xF; i >= 0x0; i--)
    {
        CTArrayDeleteEntry(array, i);
    }
    
    assert(array->count == 0x0);
#pragma mark - CTArray Test End
    
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
        assert(strcmp(CTDictionaryValueForKey(dict, testStrings[i]), testStrings2[i]) == 0);
    }
    
    for (int i = 0; i < 0x10; i++)
    {
        CTDictionaryDeleteEntry(dict, testStrings[i]);
    }
    
    assert(dict->count == 0x0);
#pragma mark - CTDictionary Test End
    
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
#pragma mark - CTString Test End
    
    CTAllocatorRelease(allocator);
#pragma mark - CTAllocator Test End
    printf("%f seconds (%lu ticks)\n", (clock() - t) / (double)CLOCKS_PER_SEC, clock() - t);
    return 0;
}

