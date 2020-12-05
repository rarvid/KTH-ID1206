#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <assert.h>

#define PORT 8080
#define MAX 512
#define TEST "Hello, hello"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main(void) {
  int sock;
  char buffer[MAX];

  /* A socket is created */
  assert((sock = socket(AF_INET, SOCK_DGRAM, 0)) != -1);

  struct sockaddr_in name;
  name.sin_family = AF_INET;
  name.sin_port = 0;
  name.sin_addr.s_addr = htonl(INADDR_ANY);

  assert(bind(sock, (struct sockaddr *) &name, sizeof(name)) != -1);

  struct sockaddr_in server;
  unsigned int size = sizeof(struct sockaddr_in);

  server.sin_family = AF_INET;
  server.sin_port = htons(SERVER_PORT);
  server.sin_addr.s_addr = inet_addr(SERVER_IP);

  int n = sizeof(TEST);   // get size of message

  assert(sendto(sock, TEST, n, 0, (struct sockaddr *) &server, size) != -1);    // send message to server socket
  n = recvfrom(sock, buffer, MAX-1, 0, (struct sockaddr *) &server, &size);     // receive message from server socket
  if (n == -1) {
    perror("server");
  }

  buffer[n] = 0;    // NULL terminate received string (I GUESS???)
  printf("Client received: %s\n", buffer);    // printf received string
  exit(0);
}
