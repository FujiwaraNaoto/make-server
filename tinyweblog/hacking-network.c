#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include "hacking-network.h"

int send_string(int sockfd,const char *buffer)
{
    int sent_bytes,bytes_to_send;
    bytes_to_send = strlen(buffer);
    while(bytes_to_send>0){
        if((sent_bytes = send(sockfd,buffer,bytes_to_send,0))==-1){
            return 0;
        }
        
        bytes_to_send -= sent_bytes;
        buffer += sent_bytes;
    }
    return 1;//success
}

int recv_line(int sockfd,char *dest_buffer)
{
    
    const char EOL[] = "\r\n";
    const int EOL_SIZE = 2;
    
    /*
    #define EOL "\r\n"
    #define EOL_SIZE 2
    */
    char *ptr;
    int eol_matched = 0;

    ptr = dest_buffer;
    fprintf(stdout,"in recv_line\n");
    /*
    GET / HTTP/1.1
    GET /xmas_wallpaper.png HTTP/1.1
    がくる
    */
    while(recv(sockfd,ptr,1,0)==1){
        fprintf(stdout,"%c",*ptr); 
        if(*ptr == EOL[eol_matched]){
            eol_matched++;
            if(eol_matched == EOL_SIZE){
                *(ptr+1-EOL_SIZE) = '\0';
                return strlen(dest_buffer);
            }
        }
        else{
            eol_matched=0;//また最初から
        }

        ptr++;
    }

    return 0;//文字が見つからない場合
}