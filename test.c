#define _GNU_SOURCE

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#define BLOCK_SIZE 4096
#define BLOCK_COUNT 450000
#define NUM_THREADS 4
#define SECONDS 10800

struct thread_data{
    int  thread_id;
    char *device_name;
    long latency;
};

struct thread_data thread_data_array[NUM_THREADS];

void *directIO(void *threadarg){
    int thread_id;
    char *device_name;
    
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;
    
    thread_id = my_data->thread_id;
    device_name = my_data->device_name;
    
    printf("Thread %d: %s \n", thread_id, device_name);
    
    int random;
    void *buff;
    int i;
    int fd;
    long rand_byte;
    int mem;
    long seek;
    int read_result;
    struct timeval start_tv, end_tv;
    long start, end, time_spent;
    long all_byte = (long) BLOCK_SIZE * BLOCK_COUNT;
    
    fd = open(device_name, O_DIRECT | O_SYNC | O_RDWR);
    if(fd < 0) {
        fprintf(stderr, "Value of errno: %d\n", errno);
        printf("Cannot open %s\n", device_name);
        exit(1);
    }
    srand(time(NULL));
    posix_memalign(&buff, BLOCK_SIZE, BLOCK_SIZE);

    rand_byte = (rand() / (double) RAND_MAX) * BLOCK_COUNT;
    rand_byte *= BLOCK_SIZE;
    gettimeofday(&start_tv, NULL);
    seek = lseek(fd, rand_byte, SEEK_SET);
    if(seek < 0) {
        printf("Cannot seek %ld\n", rand_byte);

    }

    read_result = write(fd, buff, BLOCK_SIZE);
    gettimeofday(&end_tv, NULL);
    start = start_tv.tv_sec * 1000000 + start_tv.tv_usec;
    end = end_tv.tv_sec * 1000000 + end_tv.tv_usec;
    time_spent = (end - start);
    printf("%ld\n", time_spent);
    my_data->latency = time_spent;

    close(fd);
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    time_t rawtime;
    struct tm * timeinfo;
    

    char device[NUM_THREADS][64];
    

    FILE *f = fopen("file.txt", "w+");
    if (f == NULL){
        printf("Error opening file!\n");
        exit(1);
    }
    
    fprintf(f,"%s","second\t");
    
    int nDevice;
    
    for(nDevice = 0; nDevice <NUM_THREADS; nDevice++){
        int numDevice = nDevice +1;
        fprintf(f,"%s%d\t","device",numDevice);
    }

    
    fprintf(f,"%s","tail\tmedian\t\ttime\n");
    

    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    int rc,t,try;
    try=0;
    

    if(argc <= NUM_THREADS) {
        printf("Please enough device number\n");
        exit(1);
    } else {
        int i;
        for (i = 0; i<NUM_THREADS; i++){
            sprintf(device[i], "%s", argv[i+1]);
        }
    }
    
    while(try<SECONDS){
        try++;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        
        for(t=0; t<NUM_THREADS; t++){
            thread_data_array[t].thread_id = t;
            thread_data_array[t].device_name = device[t];
            
            printf("Creating thread %d\n", t);
            
            rc = pthread_create(&threads[t], &attr, directIO,
                                (void *) &thread_data_array[t]);
            if (rc){
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }
        for (t=0; t<NUM_THREADS; t++) {
            pthread_join(threads[t], NULL);
        }
        
        printf ("Main(): Waited on %d  threads. Done.\n", NUM_THREADS);

        printf ("after 1 sec, try times %d.\n", try);
        
        long longest = thread_data_array[0].latency;
        for (t = 0 ; t < NUM_THREADS ; t++ )
        {
            if ( thread_data_array[t].latency > longest )
            {
                longest = thread_data_array[t].latency;
            }
        }
        
        long sorted_latency_array[NUM_THREADS];
        for (t = 0 ; t < NUM_THREADS ; t++ )
        {
            sorted_latency_array[t] = thread_data_array[t].latency;
        }
        
        
        long temp;
        double median;
        int i, j;

        for(i=0; i<NUM_THREADS-1; i++) {
            for(j=i+1; j<NUM_THREADS; j++) {
                if(sorted_latency_array[j] < sorted_latency_array[i]) {

                    temp = sorted_latency_array[i];
                    sorted_latency_array[i] = sorted_latency_array[j];
                    sorted_latency_array[j] = temp;
                }
            }
        }
        
        if(NUM_THREADS%2==0){
            median = (double)(sorted_latency_array[NUM_THREADS/2] + sorted_latency_array[NUM_THREADS/2 -1]) /2.0;
        }else{
            median = (double)(sorted_latency_array[NUM_THREADS/2]);
        }
        
        
        fprintf(f, "%d\t", try);
        
        for(nDevice=0; nDevice < NUM_THREADS; nDevice++){
            fprintf(f,"%ld\t",thread_data_array[nDevice].latency);
            
        }
        
        
        fprintf(f, "%ld\t%f\t%s", longest,median,asctime (timeinfo));
        fflush(f);
        sleep(1);
    }
    
    fclose(f);
    
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
}





