//
//  Proxy.h
//  Proxy
//
//  Created by Seán Hargadon on 06/02/2015.
//  Copyright (c) 2015 Seán Hargadon. All rights reserved.
//

#ifndef Proxy_Proxy_h
#define Proxy_Proxy_h


#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>

static long readTimeout(int fd, void* buffer, long size, int seconds, int milliseconds)
{
    // File descriptor sets
    fd_set read_fd;
    fd_set write_fd;
    fd_set except_fd;
    FD_ZERO(&read_fd);
    FD_ZERO(&write_fd);
    FD_ZERO(&except_fd);
    FD_SET(fd, &read_fd);
    
    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = milliseconds;
    
    // Wait for input to become ready or until the time out; the first parameter is
    // 1 more than the largest file descriptor in any of the sets
    if ( select(fd + 1, &read_fd, &write_fd, &except_fd, &timeout) == 1 )
    {
        return recv(fd, buffer, size, 0);
        //return read(fd, buffer, size);
    }
    else
    {
        return 0;
    }
}



#endif
