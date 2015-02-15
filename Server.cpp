//
//  Server.cpp
//  Proxy
//
//  Created by Seán Hargadon on 30/01/2015.
//  Copyright (c) 2015 Seán Hargadon. All rights reserved.
//

#include "Server.h"


Server::Server(int port)
{
    
    this->threads = std::vector<std::thread>(1);
    
    clean_up = false;
    
    struct sockaddr_in server_address; //server's address
    
    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0); //open socket in internet domain using a stream, OS picks tcp for streams
    
    if (this->socket_fd < 0) {
        error("Couldn't open socket");
    }
    
    //initialise to 0
    memset(&server_address, 0, sizeof(server_address));
    
    //address family
    server_address.sin_family = AF_INET;
    
    //server IP
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    //port, convert to host byte order
    server_address.sin_port = htons(port);
    
    //fix bind errors
    int a = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int));
    
    //bind socket to address
    int success_bind = bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address));
    
    
    if (success_bind < 0) {
        close(socket_fd);
        error("Couldn't bind socket to address");
    }
    
    //fcntl(socket_fd, F_SETFL, O_NONBLOCK);
    
    //listen to port, backlog of 5
    listen(socket_fd, 5);
    
    
    std::thread console( &Server::consoleInput, this );
    
    //wait for requests
    
    for (int i = 0; i < this->threads.size(); i++) {
        this->threads[i] = std::thread( &Server::threadHandler, this );
    }
    
    std::thread listener( &Server::listenForRequests, this );
    
    this->thread_lock.lock();
    long numberOfThreads = this->threads.size();
    this->thread_lock.unlock();
    
    for (int i = numberOfThreads-1; i >= 0; i--) {
        this->threads[i].join();
    }
    
    listener.join();
    console.join();

    
    
    
//    switch (status) {
//        case error_connection_failed:
//            std::cout << "Connection failed" << std::endl;
//
//            break;
//        case error_read_failed:
//            std::cout << "Write failed" << std::endl;
//            break;
//        case error_request_failed:
//            std::cout << "Request failed" << std::endl;
//
//            break;
//        case error_write_failed:
//            std::cout << "Write failed" << std::endl;
//
//            break;
//        case success:
//            std::cout << "Success" << std::endl;
//
//            break;
//            
//        default:
//            break;
//    }
}


Server::~Server()
{
    //close(socket_fd);
}


Server::Status Server::listenForRequests()
{
    while (!clean_up) {
        std::cout << "Listening" << std::endl;
        struct sockaddr_in client_address; //client's address
        
        //length of client's address
        socklen_t client_length =  sizeof(client_address);
        
        //wait for connection
        int accept_fd = accept(socket_fd, (struct sockaddr *) &client_address, &client_length);
        
        if (clean_up) {
            std::cout << "Listener dying" << std::endl;
            break;
        }
        
        if ( accept_fd < 0 ) {
            return Server::error_connection_failed;
        }
        int a = 1;
        setsockopt(accept_fd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&a, sizeof(int));

        Connection* c = new Connection(accept_fd);
        
        std::unique_lock<std::mutex> lock(connections_lock);
        
        std::cout << "Adding connection" << std::endl;

        this->connections.push( c );
        lock.unlock();
        this->connection_available.notify_all();
    }
    return Server::success;
}

void Server::threadHandler()
{
    while (!clean_up) {
        Connection* c = nullptr;
        
        std::cout << "Sleeping" << std::endl;
        std::unique_lock<std::mutex> lock(connections_lock);
        while (this->connections.empty() && !clean_up) this->connection_available.wait(lock);
        std::cout << "Waking" << std::endl;
        
        if ( clean_up ) {
            break;
        }

        if ( this->connections.size() > 0) {
            std::cout << "Getting connection" << std::endl;
            c = this->connections.front();
            this->connections.pop();
        }
        lock.unlock();
        
        if (c != nullptr) {
            std::cout << "Handling request" << std::endl;
            c->handleRequest(this->blocked_ip, this->cached_pages);
            delete c;
        }
    }
    std::cout << "Dying" << std::endl;
}


void Server::consoleInput()
{
    while (!clean_up) {
        std::string input = "";        
        std::getline(std::cin, input);
        
        std::vector<std::string> command = partition(input, " ");
        
        if (input.compare("__shutdown") == 0) {
            clean_up = true;
            close(socket_fd);
            this->connection_available.notify_all();
        }
        else if (command[0].compare("ban") == 0) {
            std::vector<std::string>::iterator it;

            it = std::find(this->blocked_ip.begin(), this->blocked_ip.end(), command[1]);
            if (it == this->blocked_ip.end()) {
                this->blocked_ip.push_back(command[1]);
                std::cout << "Banning" << std::endl;
            }
        }
        else if (command[0].compare("unban") == 0) {
            std::vector<std::string>::iterator it;
            
            it = std::find(this->blocked_ip.begin(), this->blocked_ip.end(), command[1]);
            if (it != this->blocked_ip.end()) {
                this->blocked_ip.erase( it );
                std::cout << "unbanning" << std::endl;
            }
        }
        else if ( input.compare("view") == 0 ) {
            
            for ( std::string s : this->blocked_ip ) {
                std::cout << s << std::endl;
            }
            
        }
        else if (input.compare("log") == 0) {
            
        }
    }
    std::cout << "shutting down server" << std::endl;
}


std::vector<std::string> Server::partition(std::string s, std::string delimiter)
{
    std::vector<std::string> result;
    
    std::string s1 = s.substr(0, s.find(delimiter));
    std::string s2 = s.substr(s.find(delimiter)+1, s.length());
    
    result.push_back(s1);
    result.push_back(s2);
    
    return result;
}









