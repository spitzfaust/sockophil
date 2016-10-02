//
// Created by Pommi on 9/30/16.
//

#include "sockserver/server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <memory>
#include <cstring>
#include <iostream>
namespace sockclient {

  Server::Server(unsigned short port, std::string target_directory) : port(port), target_directory(target_directory) {
    this->create_socket();
    this->server_bind();
    this->server_listen();



    /* Write back a message to the sending client */
    int message = write(server_socket, "I got your message", 18);

    if (message < 0) {
      /* write error*/
      server_error("ERROR writing back to client");
    };
    this->close_socket();
  }

  void Server::create_socket() {
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(this->server_socket == -1) {
      //throw error, in client: throw SocketCreationException(errno);
    }
    std::memset(&this->server_address,0,sizeof(this->server_address));
    /* telling the server address all values will be IP addresses */
    this->server_address.sin_family = AF_INET;
    /* dont know yet */
    this->server_address.sin_addr.s_addr = INADDR_ANY;
    /* telling the server address the port number given to our object and
     * putting it through htons (host to network short) just in case
     */
    this->server_address.sin_port = htons(this->port);

  }

  void Server::server_bind() {
    if (bind(this->server_socket, (struct sockaddr *) &this->server_address, sizeof(server_address)) < 0) {
      /* if is -1 give error
       * else start listening
       */
      server_error("ERROR connecting to client");
    }
  }

  void Server::server_listen() {
    /* we listen at our socket
     * the integer is the number of clients we want to listen to at once
     */
    listen(this->server_socket, 5);
    char buffer[1024];
    int size;
    socklen_t client_address_length = sizeof (struct sockaddr_in);


    while(1) {
      std::cout << "Waiting for clients..." << std::endl;
      /* we start listening */
      this->new_socket = accept(server_socket, (struct sockaddr *) &this->client_address, &client_address_length);
      if (this->new_socket > 0) {
          printf ("Client connected from %s:%d...\n", inet_ntoa (this->client_address.sin_addr),ntohs(this->client_address.sin_port));
          strcpy(buffer,"Welcome to myserver, Please enter your command:\n");
          send(new_socket, buffer, strlen(buffer),0);
        }
      do {
        size = recv (new_socket, buffer, (1024-1), 0);
        if( size > 0) {
          buffer[size] = '\0';
          std::cout << "Message received: " << buffer << std::endl;
        } else if (size == 0) {
          std::cout << "Client closed remote socket" << std::endl;
          break;
        } else {
          server_error("recv error");
          std::terminate();
        }
      } while (strncmp (buffer, "quit", 4)  != 0);
      close(this->new_socket);
      printf("Here is the message: %s\n", buffer);
    }
  }

  void Server::close_socket() {
    close(this->server_socket);
  }

  void Server::server_error(std::string error) {
    std::cout << error << std::endl;
    std::terminate();
  }
}