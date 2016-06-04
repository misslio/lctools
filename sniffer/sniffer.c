#include "packet_type.h"



u_char * get_payload(const u_char *packet, pkt_info_t *pkt_info)
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



void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

int same_session(tcp_session_t *ses, pkt_info_t *pkt_info)
{
	if((NULL == ses) || (NULL == pkt_info))
		return -1;

	return ((ses->ip_src.s_addr == pkt_info->ip->ip_src.s_addr) ? \
		((ses->sport == pkt_info->tcp->th_sport) ? ((ses->ip_des.s_addr == pkt_info->ip->ip_dst.s_addr) ? ((ses->dport == pkt_info->tcp->th_dport) ? 1: 0): 0) : 0):\
		((ses->ip_src.s_addr == pkt_info->ip->ip_dst.s_addr)?\
		((ses->sport == pkt_info->tcp->th_dport) ? ((ses->ip_des.s_addr == pkt_info->ip->ip_src.s_addr) ? ((ses->dport == pkt_info->tcp->th_sport)? 1 : 0): 0): 0) : 0));
}

tcp_session_t ses;
static int ses_init_flag = 0;

int main()
{

	char *dev = "eno16777736", errbuf[PCAP_ERRBUF_SIZE];

//	dev = pcap_lookupdev(errbuf);
	if(NULL == dev){
		fprintf(stderr, "Errinfo: %s\n", errbuf);
		return -1;
	}

	printf("Device: %s\n", dev + 1);

	pcap_t *handle;

	handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
	if(NULL == handle){
		fprintf(stderr, "Errinfo: %s\n", errbuf);
		return -1;
	}

	if(pcap_datalink(handle) != DLT_EN10MB){
		fprintf(stderr, "Device %s don't provide Ethernet headers - not supported\n", dev);
		return -1;
	}

	pcap_dumper_t *pdumper;
	pdumper = pcap_dump_open(handle, "./ttt.pcap");

	struct bpf_program fp;
	//src or dst net 192.168.77.131 and dst net 14.215.9.85
	char filter_exp[] = "src or dst net 113.98.246.81";
	bpf_u_int32 mask;
	bpf_u_int32 net;

	if(pcap_compile(handle, &fp, filter_exp, 0, net) == -1){
		fprintf(stderr, "Couldn't parser filter\n");
		return -1;
	}

	if(pcap_setfilter(handle, &fp) == -1){
		fprintf(stderr, "Counldn't install filter.\n");
		return -1;
	}

	struct pcap_pkthdr header;
	const u_char *packet;

	memset(&header, 0, sizeof(header));
	
	pcap_loop(handle, 1000, got_packet, (u_char *)pdumper);

	pcap_dump_flush(pdumper);
	pcap_dump_close(pdumper);
	
	pcap_close(handle);
	return 0;
}

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	printf("len = %d\n", header->len);	
	pcap_dumper_t *pdumper = (pcap_dumper_t *)args;

	pkt_info_t pkt_info;
	
	get_payload(packet, &pkt_info);
	
	if(!ses_init_flag)
		if(((pkt_info.tcp)->th_flags & SNIFF_TH_FLAGS) == SNIFF_TH_SYN){
			ses.ip_src = (pkt_info.ip)->ip_src;
			ses.sport = (pkt_info.tcp)->th_sport;

			ses.ip_des = (pkt_info.ip)->ip_dst;
			ses.dport = (pkt_info.tcp)->th_dport;
			
			ses_init_flag = 1;
		}

//	if(((pkt_info.tcp)->th_flags & SNIFF_TH_FIN) != 0)
//		printf("get fin\n");
//	if(ses_init_flag && (same_session(&ses, &pkt_info) == 1))	
		pcap_dump((u_char *)pdumper, header, packet);	
	
}



