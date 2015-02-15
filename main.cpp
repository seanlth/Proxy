#include "Server.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>


void f(int i)
{
    
}

std::vector<std::string> partition( std::string s, std::string delimiter )
{
    std::vector<std::string> result;
    
    std::string s1 = s.substr(0, s.find(delimiter));
    std::string s2 = s.substr(s.find(delimiter)+1, s.length());

    result.push_back(s1);
    result.push_back(s2);
    
    return result;
}



int main(int argc, const char* argv[])
{
//    std::vector<std::string> v = partition(input, " ");
//
//    std::cout << v[0] << std::endl;
//    std::cout << v[1] << std::endl;

    
    signal(SIGPIPE, f);
    
    Server* server = new Server(8080);
    
    
//    char* s = "GET http://seanlth.com/pipeline HTTP/1.1\r\nHost: seanlth.com\r\n";
//    
//    std::cout << trimRequest(s) << s;
    
    
//    struct addrinfo hints;
//    struct addrinfo *result;
//    struct addrinfo *p;
//    int ret;
//    
//    int connect_fd = 0;
//    
//    
//    memset(&hints, 0, sizeof(struct addrinfo));
//    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
//    hints.ai_socktype = SOCK_STREAM; /* Stream socket */
//    hints.ai_protocol = 0;          /* Any protocol */
//    hints.ai_flags = 0;
//    
//    ret = getaddrinfo("seanlth.duckdns.org/pipeline", "80", &hints, &result);
//    if (ret != 0) {
//        return -1;
//    }
//    
//    
//    for(p = result; p != NULL; p = p->ai_next) {
//        if ((connect_fd = socket(p->ai_family, p->ai_socktype,
//                                 p->ai_protocol)) == -1) {
//            perror("client: socket");
//            continue;
//        }
//        
//        if (connect(connect_fd, p->ai_addr, p->ai_addrlen) == -1) {
//            close(connect_fd);
//            perror("client: connect");
//            continue;
//        }
//        
//        break;
//    }

    
    
    
    
    
//    struct addrinfo hints;
//    struct addrinfo *result;
//    int ret;
//    
//    
//    memset(&hints, 0, sizeof(struct addrinfo));
//    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
//    hints.ai_socktype = SOCK_STREAM; /* Stream socket */
//    hints.ai_protocol = 0;          /* Any protocol */
//    hints.ai_flags = 0;
//    
//    ret = getaddrinfo("seanlth.duckdns.org/pipeline", "80", &hints, &result);
//    if (ret != 0)
//        printf("getaddrinfo returned %d (%s), errno is errno: %d\n",
//               ret, gai_strerror(ret), errno);
//    else
//        printf("getaddrinfo succeeded.");
//
//
//    for(p = result; p != NULL; p = p->ai_next) {
//        if ((connect_fd = socket(p->ai_family, p->ai_socktype,
//                                 p->ai_protocol)) == -1) {
//            perror("client: socket");
//            continue;
//        }
//        
//        if (connect(connect_fd, p->ai_addr, p->ai_addrlen) == -1) {
//            close(connect_fd);
//            perror("client: connect");
//            continue;
//        }
//        
//        break;
//    }

    
    
}






//
//#include <iostream>
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <fcntl.h>
//
//#include <netinet/tcp.h>
//#include <sys/socket.h>
//#include <sys/types.h>
//#include <netinet/in.h>
//#include <netdb.h>
//
//
//#define port 1234
//#define error(s) std::cout << s << std::endl; exit(1);
//
//
//
//
//struct Request {
//    std::string request_method;
//    std::string address;
//    Request() {}
//};
//
//
//Request parseRequest(char* buf)
//{
//    Request r;
//    
//    bool finished = false;
//    
//    int state = 0;
//    
//    while (!finished) {
//        
//        switch (state) {
//            case 0:
//                
//                for (char c = *buf++; c != ' '; c = *buf++) {
//                    r.request_method += c;
//                }
//                
//                if (r.request_method.compare("CONNECT") == 0) {
//                    r.request_method = "GET";
//                }
//                
//                state = 1;
//                break;
//            case 1:
//                
//                for (char c = *buf++; c != ' '; c = *buf++) {
//                    r.address += c;
//                }
//                
//                finished = true;
//            default:
//                break;
//        }
//        
//    }
//    
//    return r;
//}
//
////void send()
////{
////    
////    
////    
////    int sockfd, portno, n;
////    
////    
////    struct sockaddr_in serv_addr;
////    struct hostent *server;
////    
////    char buffer[256];
////    
//////    if (argc < 3) {
//////        fprintf(stderr,"usage %s hostname port\n", argv[0]);
//////        exit(0);
//////    }
////    
////    
////    sockfd = socket(AF_INET, SOCK_STREAM, 0);
////    
////    if (sockfd < 0) {
////        error("ERROR opening socket");
////    }
////    
////    server = gethostbyname( "seanlth" );
////    
////    
////    if (server == NULL) {
////        fprintf(stderr,"ERROR, no such host\n");
////        exit(0);
////    }
////    
////    bzero((char *) &serv_addr, sizeof(serv_addr));
////    
////    serv_addr.sin_family = AF_INET;
////    
////    bcopy((char *)server->h_addr,
////          (char *)&serv_addr.sin_addr.s_addr,
////          server->h_length);
////    
////    serv_addr.sin_port = htons(port);
////    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
////        error("ERROR connecting");
////    printf("Please enter the message: ");
////    bzero(buffer,256);
////    fgets(buffer,255,stdin);
////    n = write(sockfd,buffer,strlen(buffer));
////    
////    if (n < 0)
////        error("ERROR writing to socket");
////    bzero(buffer,256);
////    n = read(sockfd,buffer,255);
////    if (n < 0)
////        error("ERROR reading from socket");
////    printf("%s\n",buffer);
////    close(sockfd);
////}
//
//
//int socket_connect(char *host, in_port_t p){
//    struct hostent *hp;
//    struct sockaddr_in addr;
//    int on = 1, sock;
//    
//    if((hp = gethostbyname(host)) == NULL){
//        herror("gethostbyname");
//        exit(1);
//    }
//    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
//    addr.sin_port = htons(p);
//    addr.sin_family = AF_INET;
//    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
//    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
//    
//    if(sock == -1){
//        perror("setsockopt");
//        exit(1);
//    }
//    
//    if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1){
//        perror("connect");
//        exit(1);
//    }
//    
//    return sock;
//}
//
//
//
//
//int main(int argc, char *argv[])
//{
//    int socket_fd; //socket file descriptor
//    
//    struct sockaddr_in server_address; //server's address
//    
//    socket_fd = socket(AF_INET, SOCK_STREAM, 0); //open socket in internet domain using a stream, OS picks tcp for streams
//    
//    if (socket_fd < 0) {
//        error("Couldn't open socket");
//    }
//    
//    //initialise to 0
//    memset(&server_address, 0, sizeof(server_address));
//    
//    //address family
//    server_address.sin_family = AF_INET;
//    
//    //server IP
//    server_address.sin_addr.s_addr = INADDR_ANY;
//    
//    //port, convert to host byte order
//    server_address.sin_port = htons(port);
//    
//    int i = 1;
//    
//    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int));
//    
//    //bind socket to address
//    int success_bind = bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address));
//    
//    if (success_bind < 0) {
//        close(socket_fd);
//        error("Couldn't bind socket to address");
//    }
//    
//    //listen to port, backlog of 5
//    listen(socket_fd, 5);
//    
//    struct sockaddr_in client_address; //client's address
//    
//    //length of client's address
//    socklen_t client_length =  sizeof(client_address);
//    
//    
//    //wait for connection
//    int accept_fd = accept(socket_fd, (struct sockaddr *) &client_address, &client_length);
//    
//    
//    if ( accept_fd < 0 ) {
//        close(socket_fd);
//        error("Connection failed");
//    }
//    
//    //initialise array to 0
//    char buffer[256] = {0};
//    
//    //read from buffer
//    long success_read_write = read(accept_fd, buffer, 255);
//    
//    if (success_read_write < 0) {
//        
//        close(accept_fd);
//        close(socket_fd);
//        
//        error("Could not read from socket");
//    }
//    
//    //std::cout << "Request method: " << buffer << std::endl;
//
//    
//    Request r = parseRequest(buffer);
//    
//#define BUFFER_SIZE 1024
//
//    
//    int fd;
//    char buffer2[BUFFER_SIZE];
//    
////    if(argc < 3){
////        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
////        exit(1);
////    }
//    
//    fd = socket_connect("seanlth.com", 80);
//    
//    
//    char* s = "GET / HTTP/1.1\r\nHost: seanlth.com\r\n\r\n";
//    
//    
//    write(fd, s, strlen(s)); // write(fd, char[]*, len);
//    bzero(buffer2, BUFFER_SIZE);
//    
//    while(read(fd, buffer2, BUFFER_SIZE - 1) != 0){
//        printf(buffer2);
//        //fprintf(stderr, "%s", buffer2);
//        //bzero(buffer2, BUFFER_SIZE);
//    }
//    
//    //shutdown(fd, SHUT_RDWR);
//    close(fd);
//
//    
//    
//    
//    
//    
//    
//    
//    
//    //std::cout << "Request method: " << r.request_method << std::endl;
//    //std::cout << "Request address: " << r.address << std::endl;
//        
//        
//    
//    
//        
//        
//    
//
////    std::string command = "curl -s -i -X " + r.request_method + " " + r.address;
////    
////    FILE* fp = popen(command.c_str(), "r");
////    
////    char page_buffer[1024] = {0};
////    
////    std::string page = "";
////    while (fgets(page_buffer, sizeof(page_buffer), fp)) {
////        page += page_buffer;
////    }
////        
////        pclose(fp);
////    
////        //std::cout << page << std::endl;
////        //std::cout << page.length() << std::endl;
////
////        
//    success_read_write = write(accept_fd, buffer2, BUFFER_SIZE);
////    
////    if (success_read_write < 0) {
////        
////        close(accept_fd);
////        close(socket_fd);
////        
////        error("Could not write to socket");
////    }
////    
////    
//    close(accept_fd);
//    
//    
//    close(socket_fd);
//    
//    return 0;
//}
//
//
