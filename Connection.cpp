
#include "Connection.h"
#define BUFFER_SIZE 65536

Connection::Connection(int accept_fd)
{
    this->accept_fd = accept_fd;
}


void Connection::handleRequest(const std::vector<std::string> &banned_ips, std::map<std::string, std::string> &cached_pages)
{
    //initialise array to 0
    char request_buffer[8192] = {0};
    
    //read from buffer
    long success_read_write = read(accept_fd, request_buffer, 8192);
    
    if (success_read_write < 0) {
        close(accept_fd);
        this->log.push_back(Connection::error_read_failed);
        std::cout << "Failed to read request" << std::endl;

        return;
    }
    
    HeaderParser p = HeaderParser(request_buffer, strlen(request_buffer));
    std::vector<std::pair<std::string, std::string>> n_v = p.parseHeader();
    
    bool noCache = false;
    
    for (std::pair<std::string, std::string> p : n_v) {
        std::cout << p.first << " ";
        std::cout << p.second << std::endl;
        
        if (p.first.compare("Cache-Control") == 0) {
            noCache = true;
            std::cout << "No cache" << std::endl;
        }
    }
    
    //get host
    std::string host = n_v[1].second;
    
    if ( std::find(banned_ips.begin(), banned_ips.end(), host) != banned_ips.end() ) {
        std::cout << "Address blacklisted" << std::endl;
        close(accept_fd);
        return;
    }

    //get trimmed request, removes compression which doesn't work with caching
    std::string trimmed_request = trimRequest(request_buffer);
    
    std::string file = n_v[0].second;
    
    //modify name for saving 
    std::replace( file.begin(), file.end(), '/', '_');

    if ( cached_pages.find(file) == cached_pages.end() || noCache) {
        cached_pages[file] = file;
        manageConnection(host, (char*)trimmed_request.c_str(), file, request_not_cached);
        std::cout << "Not cached" << std::endl;
    }
    else {
        manageConnection(host, (char*)trimmed_request.c_str(), file, request_cached);
        std::cout << "Cached" << std::endl;
    }

//    
//    //parse request from user
//    //Request r = parseRequest(buffer);
//    
//    //std::cout << r.request_method << std::endl;
//    //std::cout << r.address << std::endl;
//    
//    //establish connection
//    int request_fd = forwardRequest( (char*)host.c_str());
//    
//    if (request_fd == -1) {
//        std::cout << "Failed to establish connection" << std::endl;
//        this->log.push_back(Connection::error_connection_failed);
//        return;
//    }
//    
//    std::cout << "Connection established" << std::endl;
//    
//    //request method
//    //std::string s = "GET / HTTP/1.1\r\nHost: " + r.address + "\r\n\r\n";
//    
//    
//    //send request
//    long success_send_request = send( request_fd, request_buffer, strlen(request_buffer), SO_NOSIGPIPE );
//
//    std::cout << "Send request" << std::endl;
//
//    
//    if (success_send_request < 0) {
//        close(accept_fd);
//        this->log.push_back(Connection::error_request_failed);
//        std::cout << "Failed to send request" << std::endl;
//        return;
//    }
//    
//    char buffer[BUFFER_SIZE] = {0};
// 
//    //request container
//    std::string requestReturned = "";
//    
//    long n = 0;
//    
//    while( (n = readTimeout(request_fd, buffer, BUFFER_SIZE, 5, 0)) != 0 ) {
//        success_read_write = send( accept_fd, buffer, n, SO_NOSIGPIPE );
//        
//        if (success_read_write < 0) {
//            this->log.push_back(Connection::error_write_failed);
//            std::cout << "Failed to send response to user" << std::endl;
//            
//            return;
//        }
//
//        memset(buffer, 0, BUFFER_SIZE);
//    }
//    
//    close(request_fd);
//    
//    std::cout << "Sending Response" << std::endl;
//    
//    //write request response back to user
//    //success_read_write = write( accept_fd, requestReturned.c_str(), requestReturned.size() );
//    
//    std::cout << "Response Sent" << std::endl;
//
//    if (success_read_write < 0) {
//        this->log.push_back(Connection::error_write_failed);
//        std::cout << "Failed to send response to user" << std::endl;
//
//        return;
//    }
//    
//    close(accept_fd);
//    
//    return;
 
}

void Connection::manageConnection(std::string host, char* request_buffer, std::string resource, RequestStatus status)
{    
    //establish connection
    
    if (status == request_cached) {
        cachedRequest(resource);
    }
    else if (status == request_not_cached) {
        nonCachedRequest(host, resource, request_buffer);
    }
}



int Connection::request(char* host, RequestStatus status)
{
    int fd = 0;
    
    if (status == request_cached) {
        
    }
    else if (status == request_not_cached) {
        fd = forwardRequest(host);
    }
    return fd;
}


void Connection::cachedRequest(std::string address)
{
    long success_read_write = 0;
    
    std::string line;
    
    std::ifstream f("/Users/Seanlth/Documents/College Notes/Third Year/Telecoms/Proxy/cache/" + address, std::fstream::in | std::ios::binary);
    if (f.is_open()) {
        
        char buffer[BUFFER_SIZE];
        
        while ( !f.eof() ) {
            f.read (buffer, BUFFER_SIZE);
            
                success_read_write = send( accept_fd, buffer, strlen(buffer), SO_NOSIGPIPE );
                if (success_read_write < 0) {
                    this->log.push_back(Connection::error_write_failed);
                    std::cout << "Failed to send response to user" << std::endl;
                    
                    return;
                }
        }
        
//        while ( std::getline (f, line) ) {
//            
//            line += "\r\n";
//                        
//            success_read_write = send( accept_fd, line.c_str(), line.length(), SO_NOSIGPIPE );
//            
//            if (success_read_write < 0) {
//                this->log.push_back(Connection::error_write_failed);
//                std::cout << "Failed to send response to user" << std::endl;
//                
//                return;
//            }
//        }
    }
    else {
        std::cout << "Error reading cache" << std::endl;
    }
    f.close();
    

    std::cout << "Sending Response" << std::endl;

    //write request response back to user
    //success_read_write = write( accept_fd, requestReturned.c_str(), requestReturned.size() );

    std::cout << "Response Sent" << std::endl;

    if (success_read_write < 0) {
        this->log.push_back(Connection::error_write_failed);
        std::cout << "Failed to send response to user" << std::endl;
        
        return;
    }

    close(accept_fd);

    return;
}

void Connection::nonCachedRequest(std::string host, std::string resource, char* request_buffer)
{
    long success_read_write = 0;
    
    //establish connection
    int request_fd = forwardRequest( (char*)host.c_str());
    
    if (request_fd == -1) {
        std::cout << "Failed to establish connection" << std::endl;
        this->log.push_back(Connection::error_connection_failed);
        return;
    }
    
    std::cout << "Connection established" << std::endl;
    
    //send request
    long success_send_request = send( request_fd, request_buffer, strlen(request_buffer), SO_NOSIGPIPE );
    
    std::cout << "Send request" << std::endl;
    
    if (success_send_request < 0) {
        close(accept_fd);
        this->log.push_back(Connection::error_request_failed);
        std::cout << "Failed to send request" << std::endl;
        return;
    }
    
    char buffer[BUFFER_SIZE] = {0};
    
    //request container
    std::string requestReturned = "";
    
    long n = 0;
    
    std::ofstream f;
    
    std::string file = resource;
    
    
    file = "/Users/Seanlth/Documents/College Notes/Third Year/Telecoms/Proxy/cache/" + file;
    
    FILE * pFile;
    pFile = std::fopen (file.c_str(), "wb");
    if (!pFile) {return;}
    
    
//    f.open("/Users/Seanlth/Documents/College Notes/Third Year/Telecoms/Proxy/cache/" + file);
//    if ( f.is_open() ) {
//        
//    }
//    else {
//        std::cout << "Failed to open" << std::endl;
//    }
    
    
    while( (n = readTimeout(request_fd, buffer, BUFFER_SIZE, 6, 0)) != 0 ) {
        
        if (n == -1) {
            std::cout << "!!!! ERROR !!!!" << std::endl;
            printf("%s\n", strerror(errno) );
            std::cout << "!!!! ERROR !!!!" << std::endl;
            return;
        }
        
        success_read_write = send( accept_fd, buffer, n, SO_NOSIGPIPE );
        
        std::fwrite (buffer , sizeof(char), n, pFile);

        //std::string b = buffer;
        
        //f << buffer;
        
        if (success_read_write < 0) {
            this->log.push_back(Connection::error_write_failed);
            std::cout << "Failed to send response to user" << std::endl;
            
            return;
        }
        
        memset(buffer, 0, BUFFER_SIZE);
    }
    //f.close();
    std::fclose (pFile);

    close(request_fd);
    
    
    
    std::cout << "Sending Response" << std::endl;
    
    //write request response back to user
    //success_read_write = write( accept_fd, requestReturned.c_str(), requestReturned.size() );
    
    std::cout << "Response Sent" << std::endl;
    
    if (success_read_write < 0) {
        this->log.push_back(Connection::error_write_failed);
        std::cout << "Failed to send response to user" << std::endl;
        
        return;
    }
    
    close(accept_fd);
    
    return;
}


int Connection::forwardRequest(char *host)
{
    
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *p;
    int ret;
    
    int connect_fd = 0;
    
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Stream socket */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_flags = 0;
    
    ret = getaddrinfo(host, "80", &hints, &result);
    
    //ret = getaddrinfo(host, "80", &hints, &result);
    if (ret != 0) {
        return -1;
    }
    
    std::cout << "Resolving address" << std::endl;

    int a = 1;
    setsockopt(connect_fd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&a, sizeof(int));
    
    for(p = result; p != NULL; p = p->ai_next) {
        if ((connect_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }
        
        if (connect(connect_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(connect_fd);
            perror("client: connect");
            continue;
        }
        
        break;
    }
    std::cout << "Resolved address" << std::endl;
    
    freeaddrinfo(result);

    
    
//    struct hostent* server;
//    struct sockaddr_in serverAddress;
//        
//    server = gethostbyname("seanlth.duckdns.org");
//    
//    if (server == NULL) {
//        return -1;
//    }
//    
//    memcpy(&serverAddress.sin_addr, server->h_addr, server->h_length);
//    
//    serverAddress.sin_port = htons(80);
//    
//    serverAddress.sin_family = AF_INET;
//    
//    int connect_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
//    
//    int a = 1;
//    setsockopt(connect_fd, IPPROTO_TCP, TCP_NODELAY, (const char *)&a, sizeof(int));
//    
//    if (connect_fd == -1) {
//        return -1;
//    }
//    
//    int connect_success = connect(connect_fd, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in));
//    
//    if (connect_success == -1){
//        return -1;
//    }
//    
    return connect_fd;
}


Connection::Request Connection::parseRequest(char* buf)
{
    Request r;
    
    bool finished = false;
    
    int state = 0;
    
    while (!finished) {
        
        switch (state) {
            case 0:
                
                for (char c = *buf++; c != ' '; c = *buf++) {
                    r.request_method += c;
                }
                
                if (r.request_method.compare("CONNECT") == 0) {
                    r.request_method = "GET";
                }
                
                state = 1;
                break;
            case 1:
            {
                bool terminate;
                int state = 0;
                
                while (!terminate) {
                    char c = *buf++;
                    
                    switch (state) {
                        case 0:
                            
                            if (isalpha(c)) {
                                state = 0;
                            }
                            else if (c == ':') {
                                state = 1;
                            }
                            
                            break;
                        case 1:
                            if (c == '/') {
                                state = 2;
                            }
                            
                            break;
                        case 2:
                            if (c == '/') {
                                terminate = true;
                            }
                            
                            break;
                        default:
                            break;
                    }
                    
                }
                
                for (char c = *buf++; c != ' ' && c != '/'; c = *buf++) {
                    r.address += c;
                }
                if (r.address[r.address.length()-1] == '/') {
                    r.address.pop_back();
                }
                
                finished = true;
            }
            default:
                break;
        }
        
    }
    
    return r;
}


std::string Connection::getHostFromRequest(std::string request)
{
    std::string host;
    
    bool finished = false;
    
    int state = 0;
    
    for (int i = 0; i < request.size() && !finished; i++) {
        
        switch (state) {
            case 0:
                if (request[i] == '\n') {
                    state = 1;
                }
                break;
            case 1:
                if (request[i] == ':') {
                    state = 1;
                }
                else if (request[i] == ' ') {
                    state = 2;
                }
                break;
            case 2:
                if (request[i] == '\r') {
                    finished = true;
                }
                else {
                    host += request[i];
                }
            default:
                break;
        }
    }
    
    return host;
}

std::string Connection::trimRequest(char *buf)
{
    std::string s = "";
    
    while (*buf != '\n') {
        s += *buf++;
    }
    *buf++;
    s+= '\n';
    while (*buf != '\n') {
        s += *buf++;
    }
    
    s += "\n\r\n";
    
    return s;
}



