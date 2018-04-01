/**************************************************************
 *                                                            *
 * author: Nikolas Ioannou                                    *
 *                                                            *
 **************************************************************/

#include "procon.h"

int main(int argc,char** argv){

	read_args(argc,argv);

	initialize();

	create_threads();

	join_threads();
	
	print_results();

	destroy();
	
	return 0;
} 

void *producer(void* ptr){
	int count;
	for(count=0;count<PRODUCE;count++){
		usleep(1 + rand() % 9);
		sem_wait(&empty);
		pthread_mutex_lock(&buf_mutex);

		buffer[++pos]=1+rand() % 99;	
		full++;
		gl_cnt++;

		pthread_mutex_unlock(&buf_mutex);
	}

	if(gl_cnt==(PRODUCE*producers_no)) done++;

	pthread_exit( NULL );
}

void *consumer(void* ptr){
	int item,con_id=(int)(intptr_t)ptr;
	
	while(!done || full){
		pthread_mutex_lock(&buf_mutex);

		if(full){
			item=buffer[pos--];
			full--;
			con_cnt[con_id]++;
			con_sum[con_id]+=item;
			sem_post(&empty);
		}

		pthread_mutex_unlock(&buf_mutex);
	}

	pthread_exit( NULL );
}

void read_args(int argc,char** argv){
	
	consumers_no=producers_no=0;

	if(argc<5){
		fprintf(stderr,"Expected at least two parameters(-p,-c)\n");
		exit(-1);
	}else{
		for(i=0;i<argc;i++){
			if(!strcmp((argv)[i] ,"-p")) producers_no = atoi(argv[i+1]);
			else if(!strcmp((argv)[i],"-c")) consumers_no = atoi(argv[i+1]);
		}

		if(!producers_no || !consumers_no){
			fprintf(stderr,"Please state correct the number for both, producers and consumers\n");
			exit(-1);
		}
	}
	
}

void initialize(){
	pos=-1;
	full=done=gl_cnt=0;

	maxbuffsize = (EACH*producers_no);
	srand(time(NULL));

	sem_init(&empty,0,maxbuffsize);
	pthread_mutex_init(&buf_mutex,0);

	con_cnt = malloc(sizeof(unsigned long long int)*consumers_no);
	con_sum = malloc(sizeof(unsigned long long int)*consumers_no);

	buffer = malloc(sizeof(int)*maxbuffsize);

	producers=malloc(sizeof(pthread_t)*producers_no);
	consumers=malloc(sizeof(pthread_t)*consumers_no);
}

void create_threads(){
	for(i=0;i<producers_no;i++){
		pthread_create(&producers[i],0,producer,0);
	}

	pthread_create(&load,0,&prompt,0);

	for(i=0;i<consumers_no;i++){
		pthread_create(&consumers[i],0,consumer,(void*)(intptr_t)i);
	}

}

void join_threads(){
	for(i=0;i<producers_no;i++){
		pthread_join(producers[i],0);
	}
	
	pthread_join(load,0);

	for(i=0;i<consumers_no;i++){
		pthread_join(consumers[i],0);
	}
}

void print_results(){
	printf("\rDone.          \n");
	for(i=0;i<consumers_no;i++)
		printf("Consumer#%d: %llu [%llu]\n",i+1,con_sum[i],con_cnt[i]);

}

void destroy(){
	if(producers) free(producers);
	if(consumers) free(consumers);
	if(buffer) free(buffer);
	if(con_cnt) free(con_cnt);
	if(con_sum) free(con_sum);

	pthread_mutex_destroy(&buf_mutex);
	sem_destroy(&empty);
}

void *prompt(void* ptr){

	int j;
	while(!done){
		fprintf(stderr,"\rproducing");
		for(j=0;j<3;j++){
			sleep(1);

			fprintf(stderr,".");
		}
		sleep(2);
		fprintf(stderr,"\rproducing   ");
		sleep(1);
	}

	pthread_exit(NULL);
}


