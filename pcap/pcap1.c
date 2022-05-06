/*
pcap_pkthdr{
    struct timeval ts;
    buf_u_int32 caplen
    buf_uint32 len;
}

gcc pcap1.c -o 実行ファイル名 -lpcap
管理者権限で実行

*/

#include<stdio.h>
#include<pcap.h>
#include<stdlib.h>
#include<signal.h>
#include "DieWithError.h"

pcap_t *pcap_handle;

void close_pcap(int signum){
    fprintf(stdout,"packet close");
    pcap_close(pcap_handle);
    exit(0);
}



int main(int argc,char *argv[]){
    
    if(argc!=2){
        fprintf(stdout,"Usage: %s <device name>\n",argv[0]);
        exit(1);
    }


    char *device;
    /*
    if( (device=pcap_findalldevs(argv[1]))==NULL  ){
        DieWithError("No such device\n");
    }
    */
    if( (device=pcap_lookupdev(argv[1]))==NULL  ){
        DieWithError("No such device\n");
    }

    pcap_t *pcap_handle;
    /*
    対象デバイス device
    最大パケットサイズ 4096
    プロミスキャスフラグ 1
    タイムアウト値 0
    エラーバッファ ebf
    */
    char errorbuf[PCAP_ERRBUF_SIZE];
    if( (pcap_handle=pcap_open_live(device,4096,1,0,errorbuf))==NULL){
        DieWithError("pcap open live failed()\n");
    }
    fprintf(stdout,"success, argv[1]=%s\n",argv[1]);
    
    while(1){
        signal(SIGINT,close_pcap);
    }
    



}