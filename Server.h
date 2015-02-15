//
//  Server.h
//  Proxy
//
//  Created by Seán Hargadon on 30/01/2015.
//  Copyright (c) 2015 Seán Hargadon. All rights reserved.
//

#ifndef __Proxy__Server__
#define __Proxy__Server__

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include <thread>
#include <queue>
#include <mutex>

#include "Connection.h"

#define error(s) std::cout << s << std::endl; exit(1);



class Server {
private:
    
    enum Status {
        error_connection_failed, error_read_failed, error_write_failed, error_request_failed, success
    };
    
    struct Request {
        std::string request_method;
        std::string address;
        Request() {}
    };
    
    int port;
    int socket_fd; //socket file descriptor
    
    bool clean_up;
    
    std::vector<std::string> blocked_ip;
    std::map<std::string, std::string> cached_pages;
    
    std::mutex connections_lock;
    std::mutex thread_lock;
    std::condition_variable connection_available;
    
    int available;
    
    int thread_pointer;
    
    std::vector<std::string> partition(std::string s, std::string delimiter);
    
    std::queue<Connection*> connections;
    std::vector<std::thread> threads;
        
    Server::Status listenForRequests();
    void threadHandler();
    
    void consoleInput();
public:
    Server(int port);
    ~Server();
    
    
    
    
};

#endif /* defined(__Proxy__Server__) */
