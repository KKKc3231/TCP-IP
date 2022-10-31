#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#define u_char unsigned char
#define u_short unsigned short
struct arp_packet
{
    u_char mac_target[ETH_ALEN];
    u_char mac_source[ETH_ALEN];
    u_short ethertype;
    u_short hw_type; 
    u_short proto_type;
    u_char mac_addr_len;
    u_char ip_addr_len;
    u_short operation_code;
    u_char mac_sender[ETH_ALEN];
    u_char ip_sender[4];
    u_char mac_receiver[ETH_ALEN];
    u_char ip_receiver[4];
    u_char padding[18];
};
void die(const char*pre);
void print_arp_packet(struct arp_packet *ap,char *buffer); // **漏了一个参数
int main()
{
	// ** 为修改
    int sock;
    struct arp_packet *rcvBuffer; // ** 这里改成指针
    char buffer[2048]; // ** 没有给出空间，会报错的
    sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP)); //发送接收
//以太网数据帧
    if(-1 == sock)
    { die("socket");}
    while(1)
    {    if(-1 == recv(sock, &buffer, sizeof(buffer), 0))
            continue;
        rcvBuffer = (struct arp_packet *)buffer; // ** 这里强转为指针类型，那么rcvBuffer也应该为指针类型，上面定义改一下
        print_arp_packet(rcvBuffer,buffer);
    }
 return 0;
}
void die(const char*pre)
{
    perror(pre);
    exit(1);
}
 void print_arp_packet(struct arp_packet *ap,char *buffer)
{
	// ** ap.改成ap->
    printf("\n\n---------------arp package begin-----------------\n");
	printf("mac_target = ");
    for(int i = 0; i < ETH_ALEN; i++)
    {
        printf(i > 0 ? ":%.2x" : "%.2x", ap->mac_target[i]);
    }
    printf("\nmac_source = ");
    for(int i = 0; i < ETH_ALEN; i++)
    {
        printf(i > 0 ? ":%.2x" : "%.2x", ap->mac_source[i]);
    }
    printf("\nethertype = 0%x", ntohs(ap->ethertype));
    printf("\nhw_type = 0%x", ntohs(ap->hw_type));
    printf("\nproto_type = 0%x", ntohs(ap->proto_type));
    printf("\nmac_addr_len = %x", ap->mac_addr_len);
    printf("\nip_addr_len = %x", ap->ip_addr_len);
    printf("\noperation_code = 0%x", ntohs(ap->operation_code));
    printf("\nmac_sender = ");
    for(int i = 0; i < ETH_ALEN; i++)
    {
        printf(i > 0 ? ":%.2x" : "%.2x", ap->mac_sender[i]);
    }
    printf("\nip_sender = %s", inet_ntoa(*(struct in_addr*)(ap->ip_sender)));
    printf("\nmac_receiver = ");
    for(int i = 0; i < ETH_ALEN; i++)
    {
        printf(i > 0 ? ":%.2x" : "%.2x", ap->mac_receiver[i]);
    }
    printf("\nip_receiver = %s", inet_ntoa(*(struct in_addr*)(ap->ip_receiver)));
    printf("\n padding ::: %X ",ap->padding);
    printf("\n-------------arp package end-------------------\n");
    for(int i=0;i<sizeof(ap);i++)
    {
        printf(i > 0 ? ":%.2x" : "%.2x", buffer[i]);
    }
}
