//
//  CTNetServer.h
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include <unistd.h>
#if defined (_POSIX_VERSION) || (__CYGWIN__)
#include <arpa/inet.h>
#elif defined _WIN32
#include <winsock2.h>
#else
#error "Unsupported platform"
#endif

typedef struct
{
    CTAllocatorRef alloc;
    char * address;
    struct sockaddr_in socketAddress;
    int handle;
} CTNetServer, * CTNetServerRef;

CTNetServerRef CTNetServerOpen(CTAllocatorRef restrict alloc, const char * restrict address, unsigned short port);
void CTNetServerRelease(CTNetServerRef server);
void CTNetServerClose(const CTNetServerRef restrict server);
long CTNetServerSend(const CTNetServerRef restrict server, const char * restrict bytes, uint64_t size);
const char * CTNetServerReceive(const CTNetServerRef restrict server, uint64_t size);