#include "packet_type.h"

u_char * slice_packet(const u_char *packet, pkt_info_t *pkt_info)
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

	pkt_info->tcp = (struct sniff_tcp *)(packet + SNIFF_SIZE_ETHERNET + size_ip);
	u_int size_tcp = SNIFF_TH_OFF(pkt_info->tcp) * 4;
	if(size_tcp < 20){
		fprintf(stderr, "Invalid TCP header length: %u bytes\n", size_tcp);
		return NULL;
	}

	pkt_info->payload = (u_char *)(packet + SNIFF_SIZE_ETHERNET + size_ip +size_tcp);

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
static int count = 0;
void read_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	pkt_info_t pkt_info;

	u_char *payload = slice_packet(packet, &pkt_info);
	if(payload == NULL){
		fprintf(stderr, "LINE %d, read packet from file error\n", __LINE__);	
		return;
	}
	++count;
//	printf("read packet succeed, %d\n", ++count);
	
	u_char tcp_flags[8][5] = {"FIN ", "SYN ", "RST ", "PUSH ", "ACK ", "URG ", "ECE ", "CWR "};
	int i = 0;
	u_char flag = pkt_info.tcp->th_flags; 
	u_char tmp = 0;
	
	for(; i < 8; i++){
		tmp = flag >> i;
		if(tmp & 0x01)
			printf("%s ", tcp_flags[i]);
	}

	printf("count : %d\n", count);
}

int read_pcap_from_file(char *pcap_file_path)
{
	if(NULL == pcap_file_path)
		return -1;
	
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t *handle = pcap_open_offline(pcap_file_path, errbuf);
	if(handle == NULL){
		fprintf(stderr, "line %d,pcap_open_offline error\n", __LINE__);
		return -1;
	}

	pcap_loop(handle, 0, read_packet, NULL);
}

int main()
{
	read_pcap_from_file("./ttt.pcap");	
}
