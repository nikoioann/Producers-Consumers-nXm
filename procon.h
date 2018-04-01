/**************************************************************
 *                                                            *
 * author: Nikolas Ioannou                                    *
 *                                                            *
 **************************************************************/

#define _BSD_SOURCE
#define PRODUCE 1000000
#define EACH 32 

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <string.h>


unsigned long long int *con_cnt=0,*con_sum=0;
int *buffer,maxbuffsize,producers_no,consumers_no,pos,full,done,gl_cnt,i;
pthread_t *producers=0,*consumers=0,load;
pthread_mutex_t buf_mutex;
sem_t empty;

void *producer(void*);
void *consumer(void*);
void read_args(int,char**);
void initialize();
void create_threads();
void join_threads();
void print_results();
void destroy();
void *prompt(void*);
