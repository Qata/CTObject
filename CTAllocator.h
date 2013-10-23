//
//  CTAllocator.h
//  Lefty
//
//  Created by Carlo Tortorella on 21/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once

typedef struct
{
	unsigned long count;
	void ** objects;
} CTAllocator;

CTAllocator * CTAllocatorCreate();
void CTAllocatorRelease(CTAllocator * restrict allocator);

void * CTAllocatorAllocate(CTAllocator * restrict allocator, unsigned long size);
void * CTAllocatorReallocate(CTAllocator * restrict allocator, void * ptr, unsigned long size);
void CTAllocatorDeallocate(CTAllocator * restrict allocator, void * ptr);