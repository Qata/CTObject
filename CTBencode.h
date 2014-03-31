//
//  CTBencode.h
//  CTObject
//
//  Created by Carlo Tortorella on 11/12/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include "CTError.h"
#include "CTArray.h"
#include "CTDictionary.h"
#include "CTObject.h"
#include "CTData.h"

/**
 * Turn a string of bencoded data into a meaningful CTObject chain.
 * @param alloc		A properly initialised CTAllocator that was created with CTAllocatorCreate.
 * @param bencoded	A character array of bencoded data.
 * @param error		An error container to warn of issues with processing the data. Should be passed as the address of a pointer that has been initialised to NULL.
 * @return			Returns a CTObject created from the bencoded data.
 **/
CTObject * CTBencodeParse(CTAllocator * restrict alloc, const char * restrict bencoded, CTError ** error);
/**
 * Turn a string of bencoded data into a meaningful CTObject chain.
 * @param alloc		A properly initialised CTAllocator that was created with CTAllocatorCreate.
 * @param bencoded	A character array of bencoded data.
 * @param start		The index to start processing the data from.
 * @param error		An error container to warn of issues with processing the data. Should be passed as the address of a pointer that has been initialised to NULL.
 * @return			Returns a CTObject created from the bencoded data.
 **/
CTObject * CTBencodeParse2(CTAllocator * restrict alloc, const char * restrict bencoded, uint64_t * start, CTError ** error);
/**
 * Turn a CTObject chain into a CTString of bencoded data.
 * @param alloc		A properly initialised CTAllocator that was created with CTAllocatorCreate.
 * @param bencoded	A CTObject to be converted into bencoded data.
 * @param error		An error container to warn of issues with processing the data. Should be passed as the address of a pointer that has been initialised to NULL.
 * @return			Returns a CTString created from the CTObject by parsing it into bencoded data.
 **/
CTString * CTBencodeSerialise(CTAllocator * restrict alloc, CTObject * restrict bencoded, CTError ** error);