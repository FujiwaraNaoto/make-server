#include<stdio.h>
#include "hacking.h"

void dump(const char *data_buffer,const unsigned int length)
{
    unsigned char byte;
   

    for(unsigned int i=0;i<length;i++){
        byte = data_buffer[i];
        printf("%02x ",data_buffer[i]);

        if( ((i%16)==15) || (i==length-1) ){
            for(unsigned int j=0;j<15-(i%16);j++){
                printf(" ");
            }
            printf("| ");

            for(unsigned int j=i-(i%16);j<i;j++){
                byte = data_buffer[j];

            if((byte>31) && (byte<127))
                printf("%c",byte);
            else
                printf(".");
            }
            printf("\n");
        }
    }
}
