//
//  Connection.h
//  Proxy
//
//  Created by Seán Hargadon on 01/02/2015.
//  Copyright (c) 2015 Seán Hargadon. All rights reserved.
//

#ifndef __Proxy__Connection__
#define __Proxy__Connection__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <string.h>
#include "Proxy.h"

class Connection {
private:
    
    enum Status {
        error_connection_failed, error_read_failed, error_write_failed, error_request_failed, success
    };
    
    struct Request {
        std::string request_method;
        std::string address;
        Request() {}
    };

    std::vector<Status> log;
    
    int accept_fd;
    
    int forwardRequest(char *host);
    
    Request parseRequest(char* buf);
    
    std::string getHostFromRequest(std::string request);
    
    std::string trimRequest(char* buf);
    
public:
    Connection(int accept_fd);
    //~Connection();
    
    void handleRequest(const std::vector<std::string> &banned_ips, const std::map<std::string, std::string> &cached_pages);
    
};

#endif /* defined(__Proxy__Connection__) */
