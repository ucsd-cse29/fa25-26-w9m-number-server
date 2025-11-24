#include "http-server.h"
#include <string.h>

char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";

// Does it have to be exactly HTTP/1.1 (was there an update? HTTP/1.2? 1.5?
// Web traffic tends to be HTTP/1.1
/*
HTTP/1.1 200 OK
Content-Type: text/plain

Hello!
*/

// A request looks like this:

//GET /special-path-for-you-all HTTP/1.1
//Host: ieng6-202.ucsd.edu:8000
//User-Agent: curl/8.7.1
//Accept: */*

/* Would like to support two paths:
   /increment         # add 1 to the number
   /add?538           # add 538 to the number (or any number after the ?)
   /                  # the current number
*/

int THE_NUMBER = 0;

void send_ok(int sock, char* body) {
  write(sock, header, strlen(header));
  write(sock, body, strlen(body));
}

void simple_handler(char* request, int response_socket) {
  printf("---start of request----\n%s\n---end of request---\n", request);
  char INCREMENT_PATTERN[] = "GET /increment ";
  char ADD_PATTERN[] = "GET /add?";

  if(strncmp(INCREMENT_PATTERN, request, strlen(INCREMENT_PATTERN)) == 0) {
    THE_NUMBER += 1;
    char response[100];
    snprintf(response, sizeof(response), "Incremented to: %d\n", THE_NUMBER);
    send_ok(response_socket, response);
  }
  else if(strncmp(ADD_PATTERN, request, strlen(ADD_PATTERN)) == 0) {
    char* addr_of_number = strstr(request, "?") + 1;
    int input_num;
    sscanf(addr_of_number, "%d", &input_num);
    char response[100];
    THE_NUMBER += input_num;
    snprintf(response, sizeof(response), "Added to: %d\n", THE_NUMBER);
    send_ok(response_socket, response);
  }
  else {
    send_ok(response_socket, "Hello!");
  }
  return;
}

int main() {
  start_server(&simple_handler, 8000);
  int a[] = {1};
  printf("a:\t%p\nsimple:\t%p\n", &a, &simple_handler);
}









