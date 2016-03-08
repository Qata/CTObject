//
//  CTError.c
//  CTObject
//
//  Created by Carlo Tortorella on 11/11/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTError.h"

CTErrorRef CTErrorCreate(CTAllocatorRef alloc, const char * restrict error, int code)
{
    CTErrorRef retVal = CTAllocatorAllocate(alloc, sizeof(CTError));
    retVal->alloc = alloc;
    retVal->error = CTStringCreate(alloc, error);
    retVal->code = code;
    
    return retVal;
}

void CTErrorAppend(CTErrorRef previous_error, CTErrorRef new_error)
{
	CTStringAppendCharacters(previous_error->error, ", ", CTSTRING_NO_LIMIT);
	CTStringAppendString(previous_error->error, new_error->error);
}

void CTErrorRelease(CTErrorRef error)
{
    CTStringRelease(error->error);
    CTAllocatorDeallocate(error->alloc, error);
}

const CTString * CTErrorGetErrorString(CTErrorRef restrict error)
{
    return error->error;
}