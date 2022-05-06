/*
https://qiita.com/Ki4mTaria/items/dabee07097acbab469d9
sudo apt-get install libpcap-dev
linuxで動く
*/
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <string.h>
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#define DPCP_RCV_MAXSIZE   68
#define DPCP_PROMSCS_MODE  1
#define DPCP_RCV_TIMEOUT   1000
#define DPCP_NOLIMIT_LOOP  -1



char * convmac_tostr(u_char *hwaddr,char *mac,size_t size){
  snprintf(mac,size,"%02x:%02x:%02x:%02x:%02x:%02x",
                     hwaddr[0],hwaddr[1],hwaddr[2],
                     hwaddr[3],hwaddr[4],hwaddr[5]);
  return mac;
}

//callbackに使用する関数 引数はこれらが使用になっている
void start_pktfunc( u_char *user,                 // pcap_loop関数の第4引数 pcap_dispatchから渡される
                    const struct pcap_pkthdr *h , // 受信したPacketの補足情報
                    const u_char *p               // 受信したpacketへのポインタ
                  ){
  char dmac[18] = {0};
  char smac[18] = {0};
  struct ether_header *eth_hdr = (struct ether_header *)p;

  printf("ether header---------\n");
  printf("dest mac %s\n",convmac_tostr(eth_hdr->ether_dhost,dmac,sizeof(dmac)));
  printf("src mac %s\n",convmac_tostr(eth_hdr->ether_shost,smac,sizeof(smac)));
  printf("ether type %x\n\n",ntohs(eth_hdr->ether_type));
    switch(ntohs(eth_hdr->ether_type)){
        case ETHERTYPE_PUP:
            printf("(Xerox PUP)\n");
            break;
        
        case ETHERTYPE_IP:
            printf("(IP)\n");
            break;
        
        case ETHERTYPE_ARP:
            printf("Address resolution\n");
            break;
/*
        case ETHERTYPE_AT:
            printf("Appe Talk protocol\n");
            break;

        case ETHERTYPE_AARP:
            printf("Appe Talk ARRP\n");
            break;
        
        case ETHERTYPE_REVARP:
            printf("Reverse ARP\n");
            break;
        
        case ETHERTYPE_VLAN:
            printf("VLAN tagging\n");
            break;


        case ETHERTYPE_IPX:
            printf("IPX\n");
            break;


       

        case ETHERTYPE_IPV6:
            printf("IPv6\n");
            break;

        case ETHERTYPE_LOOPBACK:
          printf("loop back\n");
          break;
*/
        default:
            printf("unknown");
            break;
    }


}




int main( void ){
  pcap_t *pd = NULL;
  char ebuf[PCAP_ERRBUF_SIZE];
    printf("initialize pcap\n");
    //初期設定を行う
    if( (pd = pcap_open_live( "eth0" ,             // インターフェイス名 自分のラズパイの場合はwlan0　にするべき
                            DPCP_RCV_MAXSIZE ,  // 最大受信サイズ(最初の68byteまで受信する)
                            DPCP_PROMSCS_MODE , // 自分宛以外のパケットも処理の対象にする
                            DPCP_RCV_TIMEOUT ,  // タイムアウト時間(ミリ秒)
                            ebuf )) == NULL ){
    // error
    perror("pcap_open_live() failed.");//Devie not configured
    exit(-1);
    }

    printf("pcap loop\n");
    //この中にcallback関数を設定するとパケット受信毎にcallback関数がコールされる
    if( pcap_loop( pd ,
                DPCP_NOLIMIT_LOOP , // エラーが発生するまで取得を続ける(基本無限ループ)
                start_pktfunc,      // パケット受信した時のCallBack関数
                NULL                   //  CallBack関数へ渡す引数 追加の引数がないためNULL
                ) < 0 ){
    // error
    perror("pcap_loop() failed.");
    exit(-1);
    }

    pcap_close(pd);
    return 0;
}
