/*
 * File : hello-pthread.c
 * Author : Pradeep Singh 
 * Date : 2/26/2018
 * Description: First pthread program; prints "hello world!" for every thread created.
 * Program usage: 
 *                gcc -o hello-pthread hello-pthread.c -lpthread    // compile using -lpthread (library for pthread)
 *                ./hello-pthread                                   // run
 */ 

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS	5

void *PrintHello(void *threadid)
{
   long tid;
   tid = (long)threadid;
   printf("Hello World! Thread number %ld!\n", tid);
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   int a;
   long t;
   for(t=0;t<NUM_THREADS;t++){
      a = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
   }
   pthread_exit(NULL);
}

