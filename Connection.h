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
#include <fstream>
#include <vector>
#include <map>
#include <string.h>
#include "Proxy.h"
#include "HeaderParser.h"



class Connection {
private:
    
    enum Status {
        error_connection_failed, error_read_failed, error_write_failed, error_request_failed, success
    };
    
    enum RequestStatus {
        request_cached, request_not_cached
    };
    
    struct Request {
        std::string request_method;
        std::string address;
        Request() {}
    };

    std::vector<Status> log;
    int accept_fd;
    
    Request parseRequest(char* buf);
    
    std::string getHostFromRequest(std::string request);
    std::string trimRequest(char* buf);
    
    int request(char* host, RequestStatus status);
    int forwardRequest(char* host);
    
    void manageConnection(std::string host, char* request_buffer, std::string resource, RequestStatus status);
    void nonCachedRequest(std::string host, std::string resource, char* request_buffer);
    void cachedRequest(std::string resource);
public:
    Connection(int accept_fd);
    //~Connection();
    
    void handleRequest(const std::vector<std::string> &banned_ips, std::map<std::string, std::string> &cached_pages);
    
};

#endif /* defined(__Proxy__Connection__) */
