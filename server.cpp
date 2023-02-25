#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <vector>




const int BUFFER_SIZE = 1024;

void error(const char *msg) {
    std::cerr << msg << std::endl;
    exit(1);
}



int main(int argc, char *argv[]) {
     
    // Create a Socket

    
    
    
    // While loop: accept and echo message back to client
    // Close the sock
    // Shutdown sys socket
    int server_socket, new_socket, portno;
    socklen_t clilen;//store the size of the client address structure
    struct sockaddr_in serv_addr, cli_addr;
    if (argc < 2) {// need name and port number 
        std::cerr << "Usage: " << argv[0] << " port" << std::endl;
        exit(1);
    }
    portno = atoi(argv[1]);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        error("ERROR opening socket");
    }

    // Bind the  socket to an ip adress and port
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;// ipv4
    serv_addr.sin_addr.s_addr = INADDR_ANY;//accept connections on any of its network interfaces
    serv_addr.sin_port = htons(portno);//local
    if (bind(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }
    // Wait for a connection
    listen(server_socket, 5);// start listening for incoming connections, with a maximum number of 5
    std::cout << "Server listening on port " << portno << std::endl;
    std::vector<std::thread> threads;
    while (true) {
        clilen = sizeof(cli_addr);
        new_socket = accept(server_socket, (struct sockaddr *) &cli_addr, &clilen);
        if (new_socket < 0) {
            error("ERROR on accept");
        }
        std::cout << "Accepted new client" << std::endl;
        threads.emplace_back(handle_client, new_socket);
    }
    close(server_socket);
    
    return 0;
}

void handle_client(int server_socket) {
    char buffer[BUFFER_SIZE];
    int n;
    while (true) {
        bzero(buffer, BUFFER_SIZE);
        n = read(server_socket, buffer, BUFFER_SIZE - 1);
        if (n < 0) {
            error("ERROR reading from socket");
        }
        if (n == 0) {
            break;  // client closed connection
        }
        std::cout << "Received message: " << buffer << std::endl;
        n = write(server_socket, buffer, strlen(buffer));
        if (n < 0) {
            error("ERROR writing to socket");
        }
    }
    close(server_socket);
}
