#ifndef HACKINGNETWORK_H
#define HACKINGNETWORK_H

int send_string(int sockfd,const char *buffer);
int recv_line(int sockfd,char *dest_buffer);

#endif