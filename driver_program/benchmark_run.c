/*
 *****************************************************************
   _____       _ _____  
  / ____|     (_)  __ \
 | (___  _ __  _| |__) |
  \___ \| '_ \| |  ___/ 
  ____) | | | | | |     
 |_____/|_| |_|_|_|     
                        
An Efficient Stack Tracing Framework for Multi-threaded Programs
*******************************************************************
*/

/*
 * This is SniP's driver program which executes the application which to be traced and Intel's Pin which is used for tracing.
 * This program passes it's pid to monitor module (kernel module) through "mychardev-0" character device.
 * One can enable or disable SniP by using "STACK_TRACE" environment variable (in run.sh).
 * One need to set PIN_PATH, PIN_TOOL, BENCH_PATH, BENCH_NAME, BENCH_ARGS environment variables correctly to use SniP. 
 * */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#include "benchmark_run.h"


int main(int argc, char * argv[]){

    int wstatus;
   
#ifdef STACK_TRACE
    int chdev_fd = open("/dev/mychardev-0",O_RDWR);

    if(chdev_fd < 0) {
        perror("open:");
	exit(-1);
    }
    char buffer[10];
    int passed_pid = 0;
    sprintf(buffer,"%d",getpid());
    printf("buffer:%s\n",buffer);

    if(write(chdev_fd, buffer, strlen(buffer)) < 0) {
        perror("write: ");
	exit(-1);
    }
#endif
    int pid;
    pid = fork();
    if(pid){
        /*parent here exec Intel's Pin*/
        int pid2;
        pid2 = fork();
        if(!pid2){
            char pid_str[10];
            sprintf(pid_str,"%u",pid);
            printf("bench child pid:%s\n",pid_str);

#ifdef STACK_TRACE
            char filename[30];
	    printf("inside\n");
            sprintf(filename,"/sys/kernel/kobject_example/0");
            printf("%s\n",filename);
            while(access(filename, F_OK) != 0); /*wait till first sysfs is created before exec Pin.*/
#endif
#ifndef STACK_TRACE
	    printf("This is vanilla pin tracing, ie. SniP is disabled\n");
	    sleep(1);
#endif
	    printf("pin tool:%s, %s\n",PIN_PATH,PIN_TOOL);
            if(execlp(PIN_PATH,"pin","-pid",pid_str,"-t",PIN_TOOL,(char*)NULL) == -1){
                perror("exec failed: ");
	        exit(-1);
            }
        }
    waitpid(pid,&wstatus,0); /*parent waits for child to finish*/
    }
    else{
#ifdef MULTI
     if(execlp(BENCH_PATH,BENCH_NAME,BENCH_ARGS,(char*)NULL) == -1){
            perror("exec failed: ");
	    exit(-1);
	    }
#endif
#ifndef MULTI
        int pid3;
        pid3 = fork();
        if(pid3){
		/*The Application execute here*/
            if(execlp(BENCH_PATH,BENCH_NAME,BENCH_ARGS,(char*)NULL) == -1){
                perror("exec failed: ");
                exit(-1);
            }
        }
        else{
	    /* No work to do here, this is to handle single threaded application in SniP*/
            exit(0);
        }

#endif
    }
   return 0;
}
