//
//  CTNetServer.h
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include <arpa/inet.h>

typedef struct
{
    CTAllocator * alloc;
    char * address;
    struct sockaddr_in socketAddress;
    int handle;
} CTNetServer;

CTNetServer * CTNetServerOpen(CTAllocator * restrict alloc, const char * restrict address, unsigned short port);
void CTNetServerRelease(CTNetServer * server);
void CTNetServerClose(const CTNetServer * restrict server);
long CTNetServerSend(const CTNetServer * restrict server, const char * restrict bytes, unsigned long size);
const char * CTNetServerReceive(const CTNetServer * restrict server, unsigned long size);