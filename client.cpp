#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <chrono>
#include <thread>

const int BUFFER_SIZE = 1024;
const int MESSAGE_INTERVAL = 1; // send message every second.

void error(const char *msg) {// when there is an error print message and exit.
    std::cerr << msg << std::endl;
    exit(1);
}

int main(int argc, char *argv[]) {//command line arguments

    //Create a Socket
    int client_socket, portno, n;// number of bytes read or written;
    struct sockaddr_in serv_addr;// adress
    char buffer[BUFFER_SIZE];// for message storing
    if (argc < 3) {// first name second hostname third port
        std::cerr << "Usage: " << argv[0] << " hostname port" << std::endl;
        exit(1);
    }
    portno = atoi(argv[2]);// get the protno number (atoi convert string to  integer)
    client_socket = socket(AF_INET, SOCK_STREAM, 0);// 
    if (client_socket < 0) {//check if it be created successfully.
        error("ERROR opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));// set all 0


    // Connect to server
    serv_addr.sin_family = AF_INET;// for IPv4
    serv_addr.sin_port = htons(portno);// port number to connet to. here is local
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) { //convert server address in text form argv[1] to binarry form and store it in sin_addr.
        error("ERROR invalid server address");
    }
    if (connect(client_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) { //connect client_socket to the srveraddress.
        error("ERROR connecting");
    }
    std::cout << "Connected to server" << std::endl;
    int message_count = 0;
    while (true) {
        ++message_count;
        std::string message = "Message #" + std::to_string(message_count);
        bzero(buffer, BUFFER_SIZE);
        strncpy(buffer, message.c_str(), BUFFER_SIZE - 1);// compy  message to buffer.
        n = write(client_socket, buffer, strlen(buffer));// wirte buffer to socket. n indecated how many char be writen in.
        if (n < 0) {
            error("ERROR writing to socket");
        }
        std::cout << "Sent message: " << message << std::endl;
        bzero(buffer, BUFFER_SIZE);
        n = read(client_socket, buffer, BUFFER_SIZE - 1);// read from server through socket.strore it to the buffer
        if (n < 0) {
            error("ERROR reading from socket");
        }
        std::cout << "Received response: " << buffer << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(MESSAGE_INTERVAL));
    }
    close(client_socket);
    return 0;
}
