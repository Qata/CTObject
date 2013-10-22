//
//  CTFunctions.c
//  WatchBoxCGI
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTFunctions.h"
#include <string.h>

char * stringDuplicate(CTAllocator * alloc, const char * restrict str)
{
    char * string;
    string = CTAllocatorAllocate(alloc, strlen(str) + 1);
    strcpy(string, str);
    string[strlen(str)] = 0;
    return string;
}
