//
// Created by Pommi on 9/30/16.
//

#include "sockserver/server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

Server::Server(int port, std::string target_dir) : port(port), target_dir(target_dir) {
  /* variable holding our socket*/
  int server_socket;
  /* length of the clients address */
  socklen_t clilen;
  /* Socket we will be creating when we accept a client request*/
  int listen_socket;
  /* Buffer size for our messages */
  char buffer[256];
  /* Structs holding both server and client address */
  struct sockaddr_in server_addr, client_addr;
  /* telling the server address all values will be IP addresses*/
  server_addr.sin_family = AF_INET;
  /* no idea yet*/
  server_addr.sin_addr.s_addr = INADDR_ANY;
  /* telling the server address the port number given to our object and
   * putting it through htons (host to network short) just in case
   */
  server_addr.sin_port = htons(port);

  /*creating the socket with socket()
   * AF_INET means all values will be IP addresses
   * SOCK_STREAM means it will be TCP
   */
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
    /* if is -1 give error
     * else start listening
     */
  }

  /* if we did not encounter an error we will start listening */
  /* we listen at our socket
   * the integer is the number of clients we want to listen to at once
   * now we only want one so 1
   */
  listen(server_socket,1);

  /* Length of client address clilen is set */
  clilen = sizeof(client_addr);

  /* we start listening */
  listen_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clilen);
  if (listen_socket < 0) {/* write error*/};
  /* Variable vor receiving the message. Variable type yet to be determined */
  std::string message;
  /* Reading the message with read() */
  message = read(server_socket,buffer,255);

  if (message < 0) {/* write error*/};

  printf("Here is the message: %s\n",buffer);

  /* Write back a message to the sending client */
  message = write(server_socket,"I got your message",18);

  if (message < 0) {/* write error*/};

  /* Close both sockets */
  close(listen_socket);
  close(server_socket);
}


Server::~Server() {

}