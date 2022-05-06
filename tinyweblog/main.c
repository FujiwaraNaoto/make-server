/*
これは完成なのである
*/
//クッキー機能もつけたいのである
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<time.h>
#include<signal.h>
#include "hacking.h"
#include "hacking-network.h"
#include "logger.h"

#define PORT 80
//const char WEBROOT[] = "./webroot";//root directory
//この部分はファイルを置いてある場所に自分でカスタマイズすべし
const char WEBROOT[] = "/Users/tinyweblog/";//
const char LOGFILE[] = "/Users/tinyweb.log";//tinywebというファイル名(拡張子は.log)

static int sockfd;//このtinyweb.cのみで使用できる
//static char filename[]="index.php";//これだとPHPがそのまま出てきてしまうな
static char filename[]="index.html";

/*
//この書き方は無理かな
//signal kill command
void handle_shutdown(int signalnum,int logfd){
    timestamp(logfd);
    write(logfd,"shut down\n",sizeof("shut down"));
    close(logfd);
    close(sockfd);
    exit(0);
}
*/

//signal kill command
void handle_shutdown(int signalnum){
    timestamp(logfd);
    write(logfd,"shut down\n",strlen("shut down\n"));
    close(logfd);
    close(sockfd);
    exit(0);
}


void DieWithError(const char *errorMessage);
void handle_connection(int sockfd,struct sockaddr_in *client_addr,int logfd);// deal with web request
int get_file_size(int fd);//it returns file descriptor


int main(int argc,char *argv[]){
    
    int new_sockfd,yes=1;

    struct sockaddr_in host_addr, client_addr;
    socklen_t sin_size;
    
    if( (logfd = open(LOGFILE,O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR))<0){
        DieWithError("Coudln't open logfile");
    }

    printf("this program accepts Port web request whose port number is %d \n",PORT);
    
    if((sockfd=socket(PF_INET,SOCK_STREAM,0))==-1){
        DieWithError("Couldn't create a socket instance");
    }

    if( (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)))==-1){
        DieWithError("Setting a socket with SO_REUSEADDR");
    }

    //signal(SIGINT,handle_shutdown(logfd));
    //signal(SIGTERM,handle_shutdown(logfd));//killプロセス
    signal(SIGINT,handle_shutdown);
    signal(SIGTERM,handle_shutdown);//killプロセス


    timestamp(logfd);
    write(logfd,"running...\n",strlen("running..."));

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(host_addr.sin_zero),'\0',8);//zero clear

    if(bind(sockfd,(struct sockaddr*)&host_addr,sizeof(struct sockaddr)) == -1){
        DieWithError("failed to bind a socket");
    }

    if(listen(sockfd,20) == -1){
        DieWithError("failed to listen to a socket");
    }

    while(1){
        sin_size = sizeof(struct sockaddr_in);
        if( (new_sockfd = accept(sockfd,(struct sockaddr*)&client_addr, &sin_size))==-1){
            DieWithError("failed to accept while connectin with a client");
        }

        handle_connection(new_sockfd,&client_addr,logfd);
    }
}

void handle_connection(int sockfd,struct sockaddr_in *client_addr_ptr, int logfd){
    char *ptr,request[500],resource[500], log_buffer[500];
    int fd,length;

    //fprintf(stdout,"request = %s\n",request);

    
    //inet_ntoaで ドット10進表記を文字列にする
    // networkバイト順に変換
    length = recv_line(sockfd,request);//hacking-network.c
    sprintf(log_buffer,"%s:%d \"%s\"\t",
        inet_ntoa(client_addr_ptr->sin_addr),ntohs(client_addr_ptr->sin_port),request);
    
    /*
    request = GET /HTTP/1.1
    request = GET /index.html
    */
    printf("Accept a request from %s. port number = %d. request = %s\n",
        inet_ntoa(client_addr_ptr->sin_addr), ntohs(client_addr_ptr->sin_port),request);

    if( (ptr = strstr(request," HTTP/"))==NULL){
        printf("This is not HTTP\n");
    }else{
        *ptr = 0;
        //fprintf(stdout,"In %s\n",ptr);
        ptr = NULL;
        
        if(strncmp(request,"GET ",4)==0){
            ptr = request+4;//先頭から4文字ぶん
        }
        if(strncmp(request,"HEAD ",5)==0){
            ptr = request+5;//先頭から5文字ぶん
        }

        if(ptr==NULL){
            printf("\t Unknown request\n");
        }else{


            if(ptr[strlen(ptr)-1]=='/'){//末尾が/で終わっている場合
                //fprintf(stdout,"request ends with /");
                strcat(ptr,filename);//indexファイルをつける
            }
            strcpy(resource,WEBROOT);//resource バッファに ファイルのあるpath名をつける
            strcat(resource,ptr);//path名にindexファイル名をつける
            /*
            1回目は GET / HTTP/1.1
            2回目は GET / image.jpg HTTP/1.1 これに対応させるため
            */
            fprintf(stdout,"in resource %s",resource);
            fd = open(resource,O_RDONLY,0);//ここでhtmlファイルを開く

            printf("\t Open '%s' \t",resource);

            if(fd==-1){
                strcat(log_buffer," 404 Not Found\n");//文字列の連結
                printf("404 Not Found\n");
                send_string(sockfd,"HTTP/1.0 404 NOT FOUND\r\n");
                send_string(sockfd,"Server: Tiny webserver \r\n\r\n");
                send_string(sockfd,"<html><head><title>404 Not Found</title></head><body><h1>URL not found</h1></body></html>\r\n");
                
            }else{
                strcat(log_buffer," 200 OK\n");
                printf("200 OK\n");
                send_string(sockfd,"HTTP/1.0 200 OK\r\n");//このメッセージを送った後に GET /index.htmlが飛んでくるかな
                send_string(sockfd,"Server: Tiny webserver\r\n\r\n");//Serverの情報
                //send_string(sockfd,"Content-Type:\r\n\r\n");
                if(ptr==request+4){
                    //GET request
                    if( (length = get_file_size(fd)) == -1){
                        DieWithError("failed to get file size");
                    }
                    if( (ptr = (char *)malloc(length))==NULL){
                        DieWithError("failed to allocate memory");
                    }
                    //ptrにhtmlファイルの内容を入れる
                    read(fd,ptr,length);//fdは htmlファイルのファイルディスクリプタ
                    //fprintf(stdout,"In server");
                    //fprintf(stdout,"%s",ptr);

                    char tempbuffer[500];
                    snprintf(tempbuffer,sizeof(tempbuffer),"Content-Length %d\r\n\r\n",length);
                    //send_string(sockfd,tempbuffer);
                    send(sockfd,ptr,length,0);//body部分
                    free(ptr);
                }
                close(fd);
            }
        }
    }
    timestamp(logfd);
    length = strlen(log_buffer);
    write(logfd,log_buffer,length);//write out logfd  
    shutdown(sockfd,SHUT_RDWR);
    

}

int get_file_size(int fd){
    struct stat stat_struct;
    if(fstat(fd,&stat_struct)==-1){
        return -1;
    }else{
        return (int) stat_struct.st_size;
    }
}