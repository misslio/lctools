#include "packet_type.h"

static u_char tcp_flags[8][5] = {"FIN", "SYN", "RST", "PUSH", "ACK", "URG", "ECE", "CWR"};

/**
 * desc - Return direction of a packet, from server or client.
 * 		  It need initialize the structure of tcp_session_t when the session begins.
 */
u_char packet_direction(tcp_session_t *ses, pkt_info_t *pkt_info)
{
	if((NULL == ses) || (NULL == pkt_info))
		return -1;

	if((ses->ip_src.s_addr == pkt_info->ip->ip_src.s_addr) && (ses->sport == pkt_info->tcp->th_sport))
		return PACKET_FROM_CLIENT;
	else if((ses->ip_des.s_addr == pkt_info->ip->ip_src.s_addr) && (ses->dport == pkt_info->tcp->th_sport))
		return PACKET_FROM_SERVER;
	else
		return -1;

}

/* Fill tcp from packet */
void ntoh_tcp(struct sniff_tcp *tcp)
{
	if(NULL == tcp)
		return ;
	
	u_char *ptr = (u_char *)tcp;
	tcp->th_sport = ntohs(*(u_short *)ptr);
	ptr += sizeof(u_short);
	tcp->th_dport = ntohs(*(u_short *)ptr);
	ptr += sizeof(u_short);
	tcp->th_seq = ntohl(*(uint32_t *) ptr);
	ptr += sizeof(uint32_t);
	tcp->th_ack = ntohl(*(uint32_t *) ptr);
	ptr += sizeof(uint32_t);

	tcp->th_offx2 = *ptr;
	ptr +=sizeof(u_char);
	tcp->th_flags = *ptr;
	ptr +=sizeof(u_char);
	
	tcp->th_win = ntohs(*(u_short *)ptr);
	ptr += sizeof(u_short);
	tcp->th_sum = ntohs(*(u_short *)ptr);
	ptr += sizeof(u_short);
	tcp->th_urp = ntohs(*(u_short *)ptr);
	ptr += sizeof(u_short);
}

/* Fill ip from packet */
void ntoh_ip(struct sniff_ip *ip)
{
	if(NULL == ip)
		return;

	u_char *ptr = (u_char *)ip;
	
	ip->ip_vh1 = *ptr;
	ptr += sizeof(ip->ip_vh1);
	ip->ip_tos = *ptr;
	ptr += sizeof(ip->ip_tos);
	ip->ip_len = ntohs(*(u_short *)ptr);
	ptr += sizeof(ip->ip_len);
	ip->ip_id = ntohs(*(u_short *)ptr);
	ptr += sizeof(ip->ip_id);
	
	ptr += sizeof(ip->ip_off);
	
	ip->ip_ttl = *ptr;
	ptr += sizeof(ip->ip_ttl);
	ip->ip_p = *ptr;
	ptr += sizeof(ip->ip_p);

	ip->ip_sum = ntohs(*(u_short *)ptr);
	ptr += sizeof(ip->ip_sum);	

	/* IP地址转换 */
}

	
u_char * slice_packet(const u_char *packet, pkt_info_t *pkt_info, tcp_session_t *tcp_session)
{
	if((NULL == packet) || (NULL == pkt_info))
		return NULL;
	
	pkt_info->ethernet = (struct sniff_ethernet*)packet;
	pkt_info->ip = (struct sniff_ip *)(packet + SNIFF_SIZE_ETHERNET);
	u_int size_ip = SNIFF_IP_HL(pkt_info->ip) * 4;
	if(size_ip < 10){
		fprintf(stderr, "Invalid IP header length: %u bytes\n", size_ip);
		return NULL;
	}

	ntoh_ip(pkt_info->ip);
	
	pkt_info->tcp = (struct sniff_tcp *)(packet + SNIFF_SIZE_ETHERNET + size_ip);
	ntoh_tcp(pkt_info->tcp);
	if(tcp_session->flag == SESSION_NO_INIT){
		memcpy(&(tcp_session->ip_src), &(pkt_info->ip->ip_src), sizeof(tcp_session->ip_src));
		memcpy(&(tcp_session->ip_des), &(pkt_info->ip->ip_dst), sizeof(tcp_session->ip_src));
		tcp_session->sport = pkt_info->tcp->th_sport;
		tcp_session->dport = pkt_info->tcp->th_dport;
		printf("sport = %u, dport = %u\n", tcp_session->sport, tcp_session->dport);
		tcp_session->flag = SESSION_START;
		
	}
	u_int size_tcp = SNIFF_TH_OFF(pkt_info->tcp) * 4;
	if(size_tcp < 20){
		fprintf(stderr, "Invalid TCP header length: %u bytes\n", size_tcp);
		return NULL;
	}

	pkt_info->payload = (u_char *)(packet + SNIFF_SIZE_ETHERNET + size_ip + size_tcp);
	pkt_info->len -= (SNIFF_SIZE_ETHERNET + size_ip + size_tcp);
	pkt_info->direction = packet_direction(tcp_session, pkt_info);
	
	return pkt_info->payload;
}

int same_session(tcp_session_t *ses, pkt_info_t *pkt_info)
{
	if((NULL == ses) || (NULL == pkt_info))
		return -1;

	return ((ses->ip_src.s_addr == pkt_info->ip->ip_src.s_addr) ? \
		((ses->sport == pkt_info->tcp->th_sport) ? ((ses->ip_des.s_addr == pkt_info->ip->ip_dst.s_addr) ? ((ses->dport == pkt_info->tcp->th_dport) ? 1: 0): 0) : 0):\
		((ses->ip_src.s_addr == pkt_info->ip->ip_dst.s_addr)?\
		((ses->sport == pkt_info->tcp->th_dport) ? ((ses->ip_des.s_addr == pkt_info->ip->ip_src.s_addr) ? ((ses->dport == pkt_info->tcp->th_sport)? 1 : 0): 0): 0) : 0));

}
/**
struct pcap_pkthdr
{
      struct timeval ts;   ts是一个结构struct timeval，它有两个部分，第一部分是1900开始以来的秒数，第二部分是当前秒之后的毫秒数
      bpf_u_int32 caplen;  表示抓到的数据长度
      bpf_u_int32 len;    表示数据包的实际长度
}

struct timeval
{
	__time_t tv_sec;         Seconds. 
	__suseconds_t tv_usec;   Microseconds. 
};

**/
void read_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	tcp_session_t *tcp_session = (tcp_session_t *)args;

	pkt_info_t pkt_info;
	pkt_info.len = header->caplen;	/* Length of packet, it includes link layer(14 bytes). */

	u_char *payload = slice_packet(packet, &pkt_info, tcp_session);
	if(payload == NULL){
		fprintf(stderr, "LINE %d, read packet from file error\n", __LINE__);	
		return;
	}

	/* Display packet information */
	#if 0
	int i = 0;
	u_char flag = pkt_info.tcp->th_flags; 
	u_char tmp = 0;
	if( pkt_info.direction== PACKET_FROM_CLIENT){
		for(; i < 8; i++){
			tmp = flag >> i;
			if(tmp & 0x01)
				printf("%s,", tcp_flags[i]);
		}
		printf("(seq:%u, ack:%u, win:%u)----->\n\n", (pkt_info.tcp)->th_seq, (pkt_info.tcp)->th_ack, (pkt_info.tcp)->th_win);
	} else if(pkt_info.direction == PACKET_FROM_SERVER){
		printf("<-----");
		for(; i < 8; i++){
			tmp = flag >> i;
			if(tmp & 0x01)
				printf("%s,", tcp_flags[i]);
		}
		printf("(seq:%u, ack:%u, win:%u)\n\n", (pkt_info.tcp)->th_seq, (pkt_info.tcp)->th_ack, (pkt_info.tcp)->th_win);
		
	}
	#else
	u_char str_tcp_flag[64], tmp, flag = pkt_info.tcp->th_flags;
	memset(str_tcp_flag, 0, 64);
	int i, len = 0;
	for(i = 0; i < 8; i++){
		tmp = flag >> i;
			if(tmp & 0x01){
				memcpy(str_tcp_flag + len, tcp_flags[i], strlen(tcp_flags[i]));
				len += strlen(tcp_flags[i]);
				str_tcp_flag[len] = ',';
				len += 1;
				if(len > 60)
					break;
			}
	}
	/* eat the last comma */
	str_tcp_flag[len - 1] = '\0';
	/* source ip ,port > destination ip ,port : [SYN FIN...] data length: len */
	if(pkt_info.direction == PACKET_FROM_SERVER)
		printf("\033[31m%u.%u - src ip %s, port %u > dst ip %s, port %u : [ %s ] SYN %u, ACK %u WIN %u ---->data length %u\033[0m\n",\
			header->ts.tv_sec,header->ts.tv_usec, inet_ntoa(pkt_info.ip->ip_src), pkt_info.tcp->th_sport, inet_ntoa(pkt_info.ip->ip_dst), pkt_info.tcp->th_dport,\
			str_tcp_flag, pkt_info.tcp->th_seq, pkt_info.tcp->th_ack, pkt_info.tcp->th_win, pkt_info.len);
	else
		printf("%u.%u - src ip %s, port %u > dst ip %s, port %u : [ %s ] SYN %u, ACK %u WIN %u ---->data length %u\n",\
			header->ts.tv_sec,header->ts.tv_usec, inet_ntoa(pkt_info.ip->ip_src), pkt_info.tcp->th_sport, inet_ntoa(pkt_info.ip->ip_dst), pkt_info.tcp->th_dport,\
			str_tcp_flag, pkt_info.tcp->th_seq, pkt_info.tcp->th_ack, pkt_info.tcp->th_win, pkt_info.len);
	
	#endif
	
	/* End display packet information */
}

int read_pcap_from_file(char *pcap_file_path)
{
	if(NULL == pcap_file_path)
		return -1;
	
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t *handle = pcap_open_offline(pcap_file_path, errbuf);
	if(handle == NULL){
		PRINT_ERR("pcap_open_offline error\n");
		return -1;
	}

	tcp_session_t tcp_session_org;
	memset(&tcp_session_org, 0, sizeof(tcp_session_t));
	
//	pcap_loop(handle, 0, read_packet, NULL);
	pcap_loop(handle, 0, read_packet, (u_char *)&tcp_session_org);
}


int main(int argc, char **argv)
{
	if(argc != 2){
		fprintf(stderr, "Input error!\n");
		return -1;
	}

	read_pcap_from_file(argv[1]);	
}

