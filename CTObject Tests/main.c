//
//  main.c
//  CTObject Tests
//
//  Created by Carlo Tortorella on 23/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTPrelude.h"
#include <string.h>
#include <assert.h>

int main(int argc, const char * argv[])
{
    char * emergencyBroadcastTest = "This is a test of the emergency broadcast system, do not be alarmed. Be alarmed when this is not a test.";
    
#pragma mark - CTAllocator Test Begin
    CTAllocator * allocator = CTAllocatorCreate();
    
#pragma mark - CTArray Test Begin
    CTArray * array = CTArrayCreate(allocator);
    CTArrayAddEntry(array, emergencyBroadcastTest, strlen(emergencyBroadcastTest));
    CTArrayAddEntry(array, "supercalafragalisticexpialadoshus", strlen("supercalafragalisticexpialadoshus"));
    assert(CTArrayIndexOfEntry(array, "This is a test of the emergency broadcast system, do not be alarmed. Be alarmed when this is not a test.") == 0);
    assert(CTArrayIndexOfEntry(array, "supercalafragalisticexpialadoshus") == 1);
    assert(strcmp(array->values[0]->characters, emergencyBroadcastTest) == 0);
    CTArrayDeleteEntry(array, 0);
    CTArrayDeleteEntry(array, 1);
#pragma mark - CTArray Test End
    
#pragma mark - CTDictionary Test Begin
    CTDictionary * dict = CTDictionaryCreate(allocator);
    CTDictionaryAddEntry(dict, "Hello", "Yes");
    CTDictionaryAddEntry(dict, "Yes", "Hello");
    CTDictionaryAddEntry(dict, "Only", "Once");
    assert(CTDictionaryIndexOfEntry(dict, "Yes") == 1);
    assert(strcmp(CTDictionaryValueForKey(dict, "Hello"), "Yes") == 0);
    assert(strcmp(CTDictionaryValueForKey(dict, "Yes"), "Hello") == 0);
    assert(strcmp(CTDictionaryValueForKey(dict, "Only"), "Once") == 0);
#pragma mark - CTDictionary Test End
    
#pragma mark - CTString Test Begin
    char * stringTest = "Test of string";
    char * prepend = "Prepended Characters. ";
    char * append = ". Appended Characters";
    CTString * string = CTStringCreate(allocator, stringTest);
    CTStringAppendCharacters(string, append);
    CTStringPrependCharacters(string, prepend);
    assert(strcmp(string->characters, "Prepended Characters. Test of string. Appended Characters") == 0);
    assert(strcmp(CTStringStringBetween(string, prepend, ". Appended Characters"), "Test of string") == 0);
    assert(CTStringStringBetween(string, "not found", "in string") == NULL);
    CTStringRemoveCharactersFromEnd(string, strlen(append));
    assert(strcmp(string->characters, "Prepended Characters. Test of string") == 0);
    CTStringRemoveCharactersFromStart(string, strlen(prepend));
    assert(strcmp(string->characters, stringTest) == 0);
#pragma mark - CTString Test End
    
    CTAllocatorRelease(allocator);
#pragma mark - CTAllocator Test End
    
    return 0;
}

