/* Documentation--------------------------------------------------
Name: PRASHANTH KB
Date: 02/04/2024
Description :  WAP to print the address which is causing Segmentation fault
sample Input and output :
 ./a.out 

Segmentation fault ..!! 

Address 0x123456 caused error
 ---------------------------------------------------------------*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<errno.h>

/* signal handler function */
void signal_handler(int signum, siginfo_t *info , void *arg)
{
    printf("Segmentation Fault...\n");
    printf("Address %p caused the error.\n",info->si_addr);
    exit(0);
}


int main()
{
    struct sigaction act;
    
    memset(&act,0,sizeof(act));         // to intilaize the structure members to zero

    /* When the SA_SIGINFO flag is specified in act.sa_flags, the signal handler address is passed via the act.sa_sigaction  field.*/    
    act.sa_sigaction = signal_handler; 
    act.sa_flags = SA_SIGINFO;

    sigaction(SIGSEGV,&act,NULL);       // to change the action   register the signal handler function

    char * str = "prashanth";
    str[0]='P';
 
    return 0;
}

