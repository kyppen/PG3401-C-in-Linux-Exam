#include <stdio.h>

void send_response(int sockFd, const char *response){
   send(sockFd, response, strlen(response), 0);
}

