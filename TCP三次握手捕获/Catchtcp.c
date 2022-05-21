#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include<netinet/if_ether.h>
#include <sys/types.h>
int main() {
	int sock, bytes_recieved, fromlen,n,id=1,on=1,s;
	unsigned char buffer[65535];
	struct sockaddr_in from;
	struct ip  *ip;
	struct tcphdr *tcp;
	struct ethhdr *ethh;
	
	/* 建立原始TCP包方式收到 以太网帧头+IP+TCP信息包 */
	sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	printf(" The IPPROTO_TCP value is %d \n",IPPROTO_TCP);
	if (sock>0) {
		printf("Prepare caught!! \n");
	} else
		return(0);
	id=1;
	while(1){
		bytes_recieved = recvfrom(sock, buffer, sizeof(buffer),0,NULL,NULL);//int  bytes_recieved
		if (bytes_recieved>0)	{
			printf("OK!Start analytic data packet!\n");
			ethh = (struct  ethhdr*)buffer;
			if (!(htons(ethh->h_proto) ==  ETH_P_IP))
			{
				continue;
			}
			ip = (struct ip *)(buffer+14);
			/* tcp从 buffer + 14+(4*ip->ip_hl) 地址处开始 */
			tcp = (struct tcphdr *)(buffer +14+ (4*ip->ip_hl));
			if (tcp->syn)
			{	
				printf("【SYN】\n"); // tcp的syn标志为1表示为前两次握手，再根据ack判断是第一次握手还是第二次握手；
			}
			if (ntohs(tcp->dest)!=23 ) { /*23为Telnet端口，也可改为其它端口*/
			printf("************** ^TCP^ ************** \n");
			printf("\n ID=::: %d\n",id);
			printf("Bytes received ::: %5d\n",bytes_recieved);
			printf("************** IP info begin************* \n");
			printf("IP header length ::: %d\n",ip->ip_hl);
			printf("IP sum size ::: %d\n",ntohs(ip->ip_len));
			printf("Protocol ::: %d\n",ip->ip_p);
			printf("IP_source address ::: %s \n",inet_ntoa(ip->ip_src));
			printf("IP_dest address ::: %s \n",inet_ntoa(ip->ip_dst));
			printf("\n************** IP info end************* \n");
			printf("\n************** TCP info begin************** \n");
			if (tcp->ack)
			{
				printf("【ACK】\n"); 
			}
			printf("Source port ::: %d\n",ntohs(tcp->source));
			printf("Dest port  ::: %d\n",ntohs(tcp->dest));
			printf("seq ::: %u\n",ntohl(tcp->seq));
			printf("ack::: %u\n",ntohl(tcp->ack_seq));
			printf("\n************** TCP info end**************  \n");
			id=id+1;
			}  /*>23 end */
		} /*>0 end */
	} /*while end */
}