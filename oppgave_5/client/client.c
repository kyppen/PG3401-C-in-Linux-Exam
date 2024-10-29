#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "famSend.h"

#define MESSAGE_LENGTH 1024
#define MAGIC 42

typedef struct {
int iMagicNumber; /* Set to a fixed protocol id no*/
int iMessageSize; /* Set to sizeof(TASK5_SENDMESSAGE)*/
/* + strlen(szMessage)*/
} SENDMESSAGE;

typedef struct{
   int iMagicNumber; /* Set to a fixed protocol id no*/
   int iMessageSize; /* Set to sizeof(TASK5_CONNECT_USR)*/
   uint32_t iIpAddress; /* Set to local client IP address*/
   int iPhoneNumber; /* Set to «phone number»*/
}UserInfo;


struct in_addr getHostIp(){
    char hostbuffer[500];
    struct in_addr iAddress = { 0 };
    struct hostent* host;
    int hostname;
 
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
 
    host = gethostbyname(hostbuffer);
    iAddress = *(struct in_addr*)host->h_addr_list[0];
    return iAddress;
}

int sendPackage(int sockFd, const char* buffer, char* server_reply){
        if(send(sockFd, buffer, strlen(buffer), 0) < 0){
                puts("Send failed");
                exit(1);
        }
        if(recv(sockFd, server_reply, 1025, 0) < 0){
                puts("recv failed");
                exit(1);
        }
        return 0;
}


int sendMessageStruct(int sockFd, SENDMESSAGE* message, char* messagebuffer, char* server_reply){
   /*memcpy(buffer, thingy, sizeof(thingy));*/
   /*i used to have alot of send statement, but then i realized you can send this as a struct*/
   /*and have the programs ping pong, checking each return with strncmp(input, "word", "Length of keyword");*/
   /*though this doesnt take into account big or little endian :(*/
   /*PG3401_Lecture_10_Network.pptx slide 25:*/
   printf("%d\n", message->iMessageSize);
   if(send(sockFd, message, sizeof(SENDMESSAGE), 0)< 0){
      puts("Send failed");
      exit(1);
   }
   if(recv(sockFd, server_reply, 1025, 0) < 0){
      puts("recv failed");
      exit(1);
   }
   sendPackage(sockFd, messagebuffer, server_reply);
   printf("SERVER:");
   puts(server_reply);
   return 0;
}

int sendIntroStruct(int sockFd, UserInfo* userinfo, char* server_reply){
   if(send(sockFd, userinfo, sizeof(UserInfo), 0)< 0){
      puts("Send failed");
      exit(1);
   }
   if(recv(sockFd, server_reply, 1025, 0) < 0){
      puts("recv failed");
      exit(1);
   }
   puts(server_reply);
}

int sendClientInfo(int sockFd, char* phone, char* buffer, char* server_reply){
   char magicNum[2000];
   int magicNr;
   puts("sendClientinfo");
   /*prepares the user struct to be sendt*/
   UserInfo user;
   user.iPhoneNumber = atoi(phone);
   user.iMagicNumber = MAGIC;
   user.iMessageSize = sizeof(user);
   struct in_addr address;
   address = getHostIp();
   user.iIpAddress = address.s_addr;
   puts("before intro struct");
   sendIntroStruct(sockFd, &user, server_reply);
   puts("after intro struct");
   magicNr = atoi(server_reply);
   return magicNr;
}

int sendMessage(int sockFd, int MagicNr, char* buffer, char* server_reply){
   /*prepares sending the struct to server*/
   SENDMESSAGE Message;
   Message.iMagicNumber = MagicNr;
   Message.iMessageSize = sizeof(SENDMESSAGE) + strlen(buffer);
   sendMessageStruct(sockFd, &Message, buffer, server_reply);
}


int communicate(int port, char* ip, char* tlf){
   
   int connectRes, sendRes;
   int sockFd, new_socket, c;
   char buffer[1025] = {0};
   char server_reply[1025] = {0};
   struct sockaddr_in server, client;
   int iPort = port;
   sockFd = socket(AF_INET, SOCK_STREAM, 0);
   printf("SockFd: %d\n", sockFd);
   if(sockFd < 0){
       printf("ERROR OPENING SOCKET\n");
       /*return -1;*/
   }
   server.sin_addr.s_addr = inet_addr(ip);
   server.sin_family = AF_INET;
   server.sin_port = htons(iPort);
   
   connectRes = connect(sockFd, (struct sockaddr *)&server, sizeof(server));
   if(connectRes < 0){
      puts("Connect res error");
      /*return -1;*/
   }
   puts("Connect success");
   /*Sending connection info*/
   int MagicNr = sendClientInfo(sockFd, tlf, buffer, server_reply);
   puts("Magic gotten");
   /*introductions done, entering chating loop*/
   while(1){
      char message[MESSAGE_LENGTH]; 
      fgets(message, MESSAGE_LENGTH, stdin);
      message[strcspn(message, "\n\r")] = '\0';
      if(strlen(message) != 0){
         sendMessage(sockFd, MagicNr, message, server_reply);
         if(strncmp(server_reply, "240", 3) == 0){
            /*closing connection*/
            puts("Should close connection");
            close(sockFd);
            break;
         }
      }
   }
}

int main(int argc, char *argv[]){

 
   puts("Client");
   if(argc < 4){ /* checks if we have enough arguments*/
      puts("Invalid arguments");
      return -1;
   }
   int tlf = atoi(argv[3]);
   if(tlf == 0){
      puts("invalid phone nr");
      return -1;
   }
   
   int iPort = atoi(argv[2]);
   communicate(iPort, argv[1], argv[3]);



 

}
