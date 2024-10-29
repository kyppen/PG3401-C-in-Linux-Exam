#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include "sha1.h"

#define BUFFER_SIZE 4096
#define NUM_THREADS 2
#define BYTE_RANGE 256

typedef struct{
   int count[BYTE_RANGE];
   int bytes_in_buffer;
   char fileName[500]; /*Namefile from Args[1]*/ 
   unsigned char buffer[BUFFER_SIZE]; /*Shared buffer*/
   sem_t mutex; /* State to determine if buffer is avaliable*/
   sem_t full; /* State to determine if we can read from buffer*/
   sem_t empty; /* State to determine if we can read from file*/
   int endFlag; /*Signals between threads if the file is empty*/
   int total_bytes; /* Total bytes read*/
   SHA1_CTX context; /* Hashing context*/
   unsigned char digest[20]; /* Hash*/
   
}Args;

void* thread_A(void* argsVoid) {
   int i;
   int Acount = 0;
   puts("Thread A");
   Args* args = (Args*)argsVoid; /*casting void ptr to Args struct*/
   FILE* fp = fopen(args->fileName, "rb"); /*Opening file to read*/
   if (!fp) {
      perror("Failed to open file");
      exit(EXIT_FAILURE);
   }

   while (1) {
      Acount++;
      /*Thread A checks if the buffer is empty*/
      sem_wait(&args->empty);
      /*Thread A waits for shared data to be avaliable*/ 
      /*Reads BUFFER_SIZE amount of bytes from file to buffer*/
      int read_bytes = fread(args->buffer + args->bytes_in_buffer, 1, BUFFER_SIZE - args->bytes_in_buffer, fp);
      /*Wait for mutex to safely update bytes_in_buffer*/
      sem_wait(&args->mutex);
      args->bytes_in_buffer += read_bytes;
      /*bytes_in_buffer has been updates so we can release the mutex*/ 
      sem_post(&args->mutex);
      /*Signals that we have data in buffer*/
      sem_post(&args->full);
      /*Check if we've reached END OF FILE; */
      if (read_bytes < BUFFER_SIZE - args->bytes_in_buffer) {
         args->endFlag = 1; /*sets end of file flag*/
         sem_post(&args->full); /*signals that we have data in the buffer*/
         break;
      }
   }
   /*Closing file*/
   fclose(fp);
   /*Exit thread*/
   printf("A: WHILE RAN %d\n", Acount);
   pthread_exit(NULL);
}

void* thread_B(void* argsVoid) {
   puts("Thread B");
   /* Cast the void pointer to an Args struct*/
   Args* args = (Args*)argsVoid;
   /*initializes SHA1 context*/
   SHA1Init(&args->context);
   /*initializes each item to 0, so we avoid undefined behavior*/
   memset(args->count, 0, sizeof(args->count));
   int Bcount = 0;
   while (1) {
      Bcount++;
      /*checks if we have data in the buffer*/
      sem_wait(&args->full);
      /*Waits for sharable data to be avaliable*/
      sem_wait(&args->mutex);
      /*SHA1Update updates the hash with buffer*/
      SHA1Update(&args->context, args->buffer, strlen(args->buffer));
      
      /*check if we've reach end of file and that there is no more data in buffer*/
      if(args->bytes_in_buffer == 0 && args->endFlag == 1){
         /*Release mutex since we are done*/
         sem_post(&args->mutex);
         break; /*Done processing data*/
      }
      /*loops for each byte in the buffer*/
      /*and updates based on what characters we see*/
      int i;
      for (i = 0; i < args->bytes_in_buffer; i++)
         /*updates the count for each letter1*/
         args->count[args->buffer[i]]++;
      
      args->total_bytes = args->total_bytes + args->bytes_in_buffer;
      /*We are done processing the buffer, so we its reset to allow the next characters*/
      args->bytes_in_buffer = 0;
      /*releasing mutex after*/
      sem_post(&args->mutex);
      /*Signaling thread A to read more from file*/
      sem_post(&args->empty);
   }
   int i;
   for (i = 0; i < BYTE_RANGE; i++)
      printf("%d: %d\n", i, args->count[i]);
     
   /*Finalizing Hash after everything is processed*/
   SHA1Final(args->digest, &args->context);
   for(i = 0; i < 20; i++){ /*prints each byte in digest as hex*/
      printf("%x", args->digest[i]);
   }
   printf("\n"); /*Newline after hex-digest print;*/
   pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
   if(argc != 2){ /*Checks if we have input, exits if we dont;*/
      puts("Invalid arguments");
      return -1;     
   }
   Args args; /*create our struct for passing arguments to threads*/
   strcpy(args.fileName, argv[1]); /*copying filename gotten from program arguments into struct*/
   args.bytes_in_buffer = 0; /*instantiating bytes_in_buffer to 0*/
   args.total_bytes = 0; /*Counting total bytes*/
   sem_init(&args.mutex, 0, 1); /*instantiating mutex to 1 allowing the first wait to access it*/
   sem_init(&args.full, 0, 0); /*instantiating full to 0 because we have to wait for data from Thread A*/
   sem_init(&args.empty, 0,1); /*instantiating empty to 1 since Thread A needs to read from file first*/
   args.endFlag = 0; /* end flag*/
   
   pthread_t threadA, threadB; /*creating our threads*/


   /*Starting Thread A and passing in our Args struct*/
   if (pthread_create(&threadA, NULL,thread_A, &args) != 0) {
      perror("Could not create thread A");
      exit(1);
   }
   /*Starting Thread B and passing in our Args struct*/
   if (pthread_create(&threadB, NULL, thread_B, &args) != 0) {
      perror("Could not create thread B");
      exit(1);
   }
   /*Main will wait until Thread A finishes*/
   if (pthread_join(threadA, NULL) != 0) {
      perror("Could not join thread A");
      exit(1);
   }
   /*Main will wait until Thread B finishes*/
   if (pthread_join(threadB, NULL) != 0) {
      perror("Could not join thread B");
      exit(1);
   }
   printf("Total bytes: %d\n", args.total_bytes);
   /*Since our program is done we'll destroy the semaphores*/
   sem_destroy(&args.full);
   sem_destroy(&args.empty);
   sem_destroy(&args.mutex);

   return 0;
}










