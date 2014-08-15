//
//  CTNetServer.c
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTNetServer.h"
#include "CTFunctions.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

CTNetServer * CTNetServerOpen(CTAllocator * restrict alloc, const char * restrict address, unsigned short port)
{
    CTNetServer * server = CTAllocatorAllocate(alloc, sizeof(CTNetServer));
    server->alloc = alloc;
    if ((server->handle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        close(server->handle);
        fputs("Failed to create socket\n", stderr);
        return NULL;
    }
    
    server->socketAddress.sin_family = AF_INET;
    struct hostent * he;
    if (address && (he = gethostbyname(address)))
    {
        server->address = stringDuplicate(alloc, address);
        memcpy(&server->socketAddress.sin_addr, he->h_addr_list[0], he->h_length);
    }
    else
    {
        server->address = NULL;
        server->socketAddress.sin_addr.s_addr = INADDR_ANY;
    }
    server->socketAddress.sin_port = htons(port);
    
    if (connect(server->handle, (const struct sockaddr *)&server->socketAddress, sizeof(server->socketAddress)) < 0)
    {
        close(server->handle);
        fputs("Failed to connect to socket\n", stderr);
        return NULL;
    }
    return server;
}

void CTNetServerRelease(CTNetServer * server)
{
	CTNetServerClose(server);
    CTAllocatorDeallocate(server->alloc, server->address);
    CTAllocatorDeallocate(server->alloc, server);
}

void CTNetServerClose(const CTNetServer * restrict server)
{
    close(server->handle);
}

long CTNetServerSend(const CTNetServer * restrict server, const char * restrict bytes, uint64_t size)
{
    return send(server->handle, bytes, size, 0);
}

const char * CTNetServerReceive(const CTNetServer * restrict server, uint64_t size)
{
    CTAllocator * alloc = CTAllocatorCreate();
    char * receive = CTAllocatorAllocate(alloc, size);
    long receivedLength = recv(server->handle, receive, size, 0);
    char * retVal = CTAllocatorAllocate(server->alloc, receivedLength + 1);
    memcpy(retVal, receive, receivedLength);
    retVal[receivedLength] = 0;
    CTAllocatorRelease(alloc);
    return retVal;
}