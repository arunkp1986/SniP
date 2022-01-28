#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4
#define CHUNK_SIZE 256

void *thread_function(void * arr);

void printArray(int arr[], int size);

void merge_sort(int arr[], unsigned low, unsigned high);
void merge(int arr[], unsigned low, unsigned middle, unsigned high);

struct data{
    int tid;
    void * addr;
};

int data[NUM_THREADS*CHUNK_SIZE];
int main()
{
     pthread_t thread[NUM_THREADS];
     struct data thread_arg[NUM_THREADS];
     int  iret;
     int loop_index;
     srand(10);
     for (int j = 0; j < NUM_THREADS*CHUNK_SIZE; j++) {
         data[j] =  rand()%100;
     }
     //printArray(data,NUM_THREADS*CHUNK_SIZE);
     for(loop_index = 0; loop_index < NUM_THREADS; loop_index++){
	 thread_arg[loop_index] = (struct data){.tid = loop_index, .addr = (void*)&data[0]};
         iret = pthread_create( &thread[loop_index], NULL, thread_function, (void*)&thread_arg[loop_index]);
	 if(iret){
	     printf("thread creation failed\n");
	     exit(-1);
	 }}
     
     for(loop_index = 0; loop_index < NUM_THREADS; loop_index++){
         pthread_join( thread[loop_index], NULL);
     }
     //printArray(data, NUM_THREADS*CHUNK_SIZE);
     merge_sort(&data[0],0,(NUM_THREADS*CHUNK_SIZE)-1);
     //printArray(data, NUM_THREADS*CHUNK_SIZE);
     //sleep(10);
}

 
/* Function to print an array */
void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}


void merge_sort(int arr[], unsigned low, unsigned high){
    unsigned middle = low+(high-low)/2;
    if(low<high){
        merge_sort(arr, low, middle);
	merge_sort(arr, middle+1, high);
	merge(arr, low, middle, high);
    }
}

void merge(int arr[], unsigned low, unsigned middle, unsigned high){
    unsigned temp_num1 = (middle-low)+1;
    unsigned temp_num2 = high-middle;
    //printf("low: %u, middle:%u, high:%u, temp_num1:%u, temp_num2:%u\n",low,middle,high,temp_num1,temp_num2);
    int temp_arr1[temp_num1];
    int temp_arr2[temp_num2];
    int loop_index;
    int loop_index1 = 0;
    int loop_index2 = 0;
    
    for(loop_index = 0; loop_index < temp_num1; loop_index++){
        temp_arr1[loop_index] = arr[low+loop_index];
    }
    for(loop_index = 0; loop_index < temp_num2; loop_index++){
        temp_arr2[loop_index] = arr[loop_index+middle+1];
    }
    
    loop_index = 0;
    while(loop_index1 < temp_num1 && loop_index2 < temp_num2 ){
	//printf("index:%u\n",low+loop_index);
	//printf("temp_arr1:%u temp_arr2:%u\n",temp_arr1[loop_index1],temp_arr2[loop_index2]);
        if(temp_arr1[loop_index1] < temp_arr2[loop_index2]){
	    arr[low+loop_index] = temp_arr1[loop_index1];
            loop_index1 += 1;
        }
	else{
            arr[low+loop_index] = temp_arr2[loop_index2];
	    loop_index2 += 1;
	}
	loop_index += 1;
    }
    while(loop_index1 < temp_num1){
        arr[low+loop_index] = temp_arr1[loop_index1];
	loop_index1 += 1;
	loop_index += 1;
    }
    while(loop_index2 < temp_num2){
        arr[low+loop_index] = temp_arr2[loop_index2];
	loop_index2 += 1;
	loop_index += 1;
    }
}

void *thread_function(void * arr)
{
    struct data* d = (struct data *)arr;
    int * chunk_addr = (int*)d->addr;
    int low = (d->tid)*CHUNK_SIZE;
    int high = low+CHUNK_SIZE-1;
    sleep(20);
    //printf("addr:%p, low:%u, high:%u\n",chunk_addr,low,high);
    merge_sort(chunk_addr, low , high);
   // printArray(chunk_addr,5);
}
