//
//  CTDNSSDRegister.c
//  CTObject
//
//  Created by Carlo Tortorella on 08/11/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTDNSSDRegister.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

unsigned netUDPResolveAddress(const char * address)
{
    char * c_address = malloc(strlen(address) + 1);
    strncpy(c_address, address, strlen(address));
    c_address[strlen(address)] = 0;
    unsigned int addressIntegers[4];
    sscanf(strtok(c_address, "."), "%d", &addressIntegers[0]);
    
    for(int i = 1; i < 4; i++)
    {
        sscanf(strtok(NULL, "."), "%d", &addressIntegers[i]);
    }
    
    unsigned int dest_addr = 0;
    for(int i = 0; i < 4; i++)
    {
        //Bit shift the integers to make it into a computer-friendly address.
        dest_addr |= addressIntegers[i] << (8 * (3 - i));
    }
    
    free(c_address);
    
    return dest_addr;
}

void CTDNSSDRegister(CTDNSSDEntry * entry)
{
    int handle;
    if ((handle = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP )) < 0)
    {
        close(handle);
        printf("Failed to create socket.");
    }
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons((unsigned short) 0);
    
    if ( bind( handle, (const struct sockaddr*) &address, sizeof(address) ) < 0 )
    {
        close(handle);
        printf("Failed to bind socket.\n");
    }
    if ( fcntl( handle, F_SETFL, O_NONBLOCK, 1 ) == -1 )
    {
        close(handle);
        printf("Failed to set non-blocking socket.\n");
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(netUDPResolveAddress("224.0.0.251"));
    address.sin_port = htons(5353);
    
    printf("%li\n", sendto(handle, "", 0, 0, (struct sockaddr *)&address, sizeof(struct sockaddr_in)));
    
    close(handle);
}