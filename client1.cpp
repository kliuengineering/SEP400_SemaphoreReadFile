#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/stat.h>
#include "client.h"

bool is_running;

static void ISR( int sig ) 
{
    switch(sig)
    {
        case SIGINT: 
            std::cout << "Interrupt: " << sig << "... \n";
            is_running = false;
        break;
    }
}

int main(void)
{
    sem_t *sem_id;
    char buffer[BUF_LEN];
    int fd;
    int offset = 0;

    struct sigaction action;
    action.sa_handler = ISR;
    sigemptyset( &action.sa_mask );
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);

    // initializes the named semaphore used between unrelated processes
    sem_id = sem_open( SEM_NAME, O_CREAT, SEM_PERMS, 0 );

    is_running = true;
    while(is_running)
    {
        memset(buffer, 0, BUF_LEN);
        sem_wait(sem_id);               // count - 1

        fd = open( FILE_NAME, O_RDONLY );
        if( fd < 0 )
        {
            std::cerr << "Error: opening file...\n";
            exit(EXIT_FAILURE);
        }
        else
        {
            int ret = pread( fd, buffer, BUF_LEN - 1, offset );
            if( ret > 0 )
            {
                std::cout << "client 1: " << buffer << std::endl;
                offset += ret;
            }
            close(fd);
        }
        sem_post( sem_id );
        if( offset > 300 ) offset = 0;
        sleep(1);
    }

    // clean up
    std::cout << "Exiting on SUCCESS\n";
    close(fd);
    sem_close(sem_id);
    sem_unlink(SEM_NAME);
    return 0;
}