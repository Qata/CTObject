//
//  CTDNSSDRegister.h
//  CTObject
//
//  Created by Carlo Tortorella on 08/11/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include "CTString.h"
#include <stdint.h>

typedef struct
{
    CTString * name;
    CTString * service;
    CTString * domain;
    uint16_t port;
    CTAllocator * alloc;
} CTDNSSDEntry;

void CTDNSSDRegister(CTDNSSDEntry * entry);