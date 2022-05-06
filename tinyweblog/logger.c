#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include "logger.h"

int logfd;

void timestamp(int fd)
{
    time_t now;
    struct tm *time_struct;
    int length;
    char time_buffer[40];

    time(&now);//起源からの秒数の経過
    time_struct = localtime((time_t *)&now);
    length = strftime(time_buffer,40,"%m/%d/%Y %H:%M:%S>",time_struct);
    write(fd,time_buffer,length);

}