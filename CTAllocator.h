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
void CTAllocatorRelease(CTAllocator * allocator);

void * CTAllocatorAllocate(CTAllocator * allocator, unsigned long size);
void * CTAllocatorReallocate(CTAllocator * allocator, void * ptr, unsigned long size);
void CTAllocatorDeallocate(CTAllocator * allocator, void * ptr);