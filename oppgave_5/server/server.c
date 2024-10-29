#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <unistd.h>
#include "famSend.h"

#define MAGIC 42

/*Valgrind screams at this program, but it works*/

typedef struct {
int iMagicNumber; /* Set to a fixed protocol id no*/
int iMessageSize; /* Set to sizeof(TASK5_SENDMESSAGE)*/
/* + strlen(szMessage)*/
} SENDMESSAGE;


typedef struct{
   int iPort;
   int bConnected;
} Args;

typedef struct{
   int iMagicNumber; /* Set to a fixed protocol id no*/
   int iMessageSize; /* Set to sizeof(TASK5_CONNECT_USR)*/
   uint32_t iIpAddress; /* Set to local client IP address*/
   int iPhoneNumber; /* Set to «phone number»*/
}UserInfo;


int chatting(int new_socket, UserInfo *user){
   /*puts("Introduction done");*/
   SENDMESSAGE message;
   message.iMagicNumber = 0;
   message.iMessageSize = 0;
   /*printf("Tlf: %d\n", user->iPhoneNumber);*/
   int messageSize = 0;
   
   /*We first get the struct containing MagicNumber and the totalMessage size*/
   int remaining = recv(new_socket, &message, sizeof(SENDMESSAGE), 0);
   /*Check if client sends the magic number back*/
   if(remaining > 0 && message.iMagicNumber == MAGIC){
      messageSize = message.iMessageSize - sizeof(SENDMESSAGE);
      /*printf("messagelen: %d\n", messageSize);*/
      
      char sanatizedMessage[messageSize+1]; /*add null termination*/
      memset(sanatizedMessage, 0, strlen(sanatizedMessage));
      if(remaining == 8){ /*First struct the client sends in 8 bytes
         /*We got header*/
         send_response(new_socket, "200 OK");
         int bytes_read = recv(new_socket, &sanatizedMessage, messageSize, 0);
         /*Sets the spot after message to a null terminator, couldnt get it to work properly without this. I assume this is this is an issue because i've transfered the data wrong or something, but if it works.*/
         sanatizedMessage[bytes_read] = '\0';
         
         /*if user types /leave exit thread;*/
         if(strncmp(sanatizedMessage, "/leave", 6) == 0){
            printf("%d left\n", user->iPhoneNumber);
            send_response(new_socket, "240 Closing");
            return -1;
            
         /*else send the message*/
         }else{
            printf("Number %d: %s\n",user->iPhoneNumber, sanatizedMessage);
            send_response(new_socket,"200 OK");               
         }
        }
      }
      return 1;
}


int clientDetails(int new_socket, UserInfo *user, Args Targs){
   char buffer[1024] = {0};
   printf("%d\n", sizeof(UserInfo));
   int amount = recv(new_socket, user, sizeof(UserInfo), 0);
   if(user->iMagicNumber == MAGIC){
      send_response(new_socket, "42");
      /*put uint32_t into a in_addr struct so we can use inet_ntoa functions*/
      struct in_addr ip;
      ip.s_addr = user->iIpAddress;
      printf("Connected: %s\n", inet_ntoa(ip));
      return 1;
   }else{
      printf("wtf\n");
   }
   return 0;
   
   
   
}

void* handleClient(void* args){
   
   Args* Targs = (Args*)args;
   
   /*c = 0 fixes: Syscall param socketcall.accept(addrlen_in) points to uninitialised byte(s)*/

   int sockFd, new_socket, c = 0;
   char buffer[1025] = {0};
   UserInfo *user;
   user = malloc(sizeof(user));
   /*user.iMagicNumber = atoi(Magic);*/
   struct sockaddr_in server = {0};
   struct sockaddr_in client = {0};
   
   sockFd = socket(AF_INET, SOCK_STREAM, 0);
   printf("sockFd: %d\n", sockFd);
   if(sockFd < 0){
      puts("Error opening socket");
      exit(1);
    }
    
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = inet_addr("127.0.0.1");
   server.sin_port = htons(Targs->iPort);
   
   int bindres = bind(sockFd, (struct sockaddr*)&server, sizeof(server));
   
   if(bindres > 0){
      close(sockFd);
      exit(1);     
   }
   puts("Bind done");
   if(listen(sockFd, 3) < 0){
      close(sockFd);
      puts("Erorr listen");
      exit(1);
   }
   puts("Waiting for connections");
   if((new_socket = accept(sockFd, (struct sockaddr*)&client, (socklen_t*)&c))<0){
          puts("accept error");
          exit(1);
   }
   
   while(1){
      
      while(Targs->bConnected == 1){
      /*Here we've gotten what we need, so the user can start to chat*/
         if(chatting(new_socket, user) == -1){
            /*puts("pthread_exit");*/
            free(user);
            close(new_socket); 
            pthread_exit(NULL);
         }   
      }
         if(Targs->bConnected == 0){
            /*greets the client, getting Ip, Tlf, size of UserInfo struct? and ipAddress in uint32_t*/
          
            Targs->bConnected = clientDetails(new_socket,user,*Targs);
      }
   }
   free(user);
   pthread_exit(NULL);  
}

int main(int argc, char *argv[]){
   if(argc < 3){
      puts("Invalid arguments");
      return -1;
   }
      
   int iPort1 = atoi(argv[1]);
   if(iPort1 == 0 || iPort1 > 65535 || iPort1 == -1){
      printf("Port: %d\n", iPort1);
      puts("Invalid port");
      return -1;
   }

   int iPort2 = atoi(argv[2]);
   if(iPort2 == 0 || iPort2 > 65535 || iPort2 == -1){
      printf("Port: %d\n", iPort2);
      puts("Invalid port");
      return -1;
   }
   
   Args Thread1Args;
   Args Thread2Args;
   Thread1Args.bConnected = 0;
   Thread1Args.iPort = iPort1;
   Thread2Args.bConnected = 0;
   Thread2Args.iPort = iPort2;
   

   pthread_t Client1, Client2;
   
   if(pthread_create(&Client1, NULL, handleClient, &Thread1Args) != 0){
      puts("Error creating Client thread");
      return -1;
   }
   if(pthread_create(&Client2, NULL, handleClient, &Thread2Args) != 0){
      puts("Error creating Client thread");
      return -1;
   }
   if (pthread_join(Client1, NULL) != 0) {
      puts("Could not join Client1");
      return -1;
   }
   if (pthread_join(Client2, NULL) != 0) {
      puts("Could not join Client2");
      return -1;
   }

}


