// 必要的头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/ethernet.h>
// IP结构体
struct ip_packet
{
	unsigned char dst_mac[6];
	unsigned char soc_mac[6];
	unsigned short ethertype;
	unsigned char ip_v:4; // 版本
	unsigned char ip_hl:4; // 首部长度
	unsigned char ip_tos; // 服务类型
	unsigned short tot_len; //总长度
	unsigned short id; //标志
	unsigned short off; // 分片偏移
	unsigned char ttl; //生存时间
	unsigned char pro; 
	unsigned short chk_sum;
	unsigned char soc_ip[4];
	unsigned char dst_ip[4];
};

int main()
{
	int fd,i;
	struct ip_packet ip;
	fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_IP));
	if(fd == -1)
	{
		perror("创建套接字失败！");
	}
	while(1)
	{
		if(recv(fd,&ip,sizeof(ip),0) == -1)
			continue;
		printf("\n==========================================");
		printf("\n\t%d bytes read",ntohs(ip.tot_len));
		// 打印接收到的IP数据报中MAC地址和IP地址的信息
		printf("\n\tDestMACAddr:");
		for(i=0;i<6;i++)
		{
			printf(i > 0 ? ":%.2x" : "%.2x", ip.dst_mac[i]);
		}
		printf("\n\tSourceMACAddr:");
		for(i=0;i<6;i++)
		{
			printf(i > 0 ? ":%.2x" : "%.2x", ip.soc_mac[i]);
		}
		printf("\n\tSource host:");
		for(i=0;i<4;i++)
		{
			printf(i > 0 ? ".%d" : "%d", ip.soc_ip[i]);
		}
		printf("\n\tDest host:");
		for(i=0;i<4;i++)
		{
			printf(i > 0 ? ".%d" : "%d", ip.dst_ip[i]);
		}
	}
	return 0;
}
