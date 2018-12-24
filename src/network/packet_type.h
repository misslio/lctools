#ifndef PACKET_TYPE_H
#define PACKET_TYPE_H

#include <stdio.h>
#include <string.h>
#include <pcap.h>
#include <arpa/inet.h>


#define SNIFF_ETHER_ADDR_LEN		6 /* RFC894 */

#define PACKET_FROM_CLIENT	0x08
#define PACKET_FROM_SERVER	0x09

#define SIMPLE_DEBUG
#ifdef SIMPLE_DEBUG
#define PRINT_DEBUG(format, args...)	printf("[\033[33mDEBUG INFO\033[0m]\033[33mfunc\033[0m: %s \033[33mline\033[0m: %d :"format, __func__, __LINE__, ##args)
#else
#define PRINT_DEBUG(format, args...)	do{;}while(0)
#endif

#define PRINT_ERR(format, args...) printf("[\033[31mERR INFO\033[0m]\033[31mfunc\033[0m: %s \033[31mline\033[0m: %d :"format, __func__, __LINE__, ##args)


#ifndef LC_FREE_MALLOC
#define LC_FREE_MALLOC(p) do	{\
		if((p) != NULL)\
			{\
				free(p);\
				(p) = NULL;\
			}\
	}while(0)
#endif


/* Ethernet head */
struct sniff_ethernet{
	u_char ether_dhost[SNIFF_ETHER_ADDR_LEN];	/* 6 bytes destination addr */
	u_char ether_shost[SNIFF_ETHER_ADDR_LEN];	/* 6 bytes source addr */
	u_short	ether_type;		/* 2 bytes tyep*/
};

#define SNIFF_SIZE_ETHERNET	14

/* IP head */
struct sniff_ip{
	u_char ip_vh1;	/* 4 bits in lower is version, 4 bits in higher is length of head */
	u_char ip_tos;	/* type of service TOS */
	u_short ip_len;	/* total length */
	u_short ip_id; 	/* identification */
	u_short ip_off;	/* fragment offset field */
#define SNIFF_IP_RF	0X8000
#define SNIFF_IP_DF	0X4000
#define SNIFF_IP_MF	0x2000
#define SNIFF_IP_OFFMASK	0x1fff
	u_char ip_ttl;	/* time to live */
	u_char ip_p;	/* protocol */
	u_short ip_sum;	/* checksum */
	struct in_addr ip_src, ip_dst;	/* source and dest address */
} __attribute__((packed, aligned(1)));

#define SNIFF_IP_HL(ip)	(((ip)->ip_vh1) & (0x0f))
#define SNIFF_IP_V(ip)	(((ip)->ip_vh1) >> 4)


typedef uint32_t tcp_seq_t;

/* TCP head */
struct sniff_tcp{
	u_short th_sport;	/* source port */
	u_short th_dport;	/* dest prot */
	tcp_seq_t th_seq;		/* sequence number */
	tcp_seq_t th_ack;		/* acknowledgement number */

	u_char th_offx2;		/* data offset, rsvd */
#define SNIFF_TH_OFF(th)	((((th)->th_offx2) & 0xf0) >> 4)
	u_char th_flags;

#define SNIFF_TH_FIN 0x01
#define SNIFF_TH_SYN 0x02
#define SNIFF_TH_RST 0x04
#define SNIFF_TH_PUSH 0x08
#define SNIFF_TH_ACK 0x10
#define SNIFF_TH_URG 0x20
#define SNIFF_TH_ECE 0x40
#define SNIFF_TH_CWR 0x80
#define SNIFF_TH_FLAGS (SNIFF_TH_FIN|SNIFF_TH_SYN|SNIFF_TH_RST|SNIFF_TH_ACK|SNIFF_TH_URG|SNIFF_TH_ECE|SNIFF_TH_CWR)

	u_short th_win;		/* window */
	u_short th_sum;		/* checksum */
	u_short th_urp;		/* urgent pointer */
} __attribute__((packed, aligned(1)));

/* custom structure */
typedef struct _pkt_info{
	struct sniff_ethernet *ethernet;
	struct sniff_ip	*ip;
	struct sniff_tcp *tcp;
	u_char *payload;
	u_short len;
	u_char direction;
}pkt_info_t;

typedef struct _tcp_session{
#define SESSION_NO_INIT	0
#define SESSION_START		1
	u_char flag;
	struct in_addr ip_src;
	u_short sport;

	struct in_addr ip_des;
	u_short dport;
}tcp_session_t;


u_char packet_direction(tcp_session_t *ses, pkt_info_t *pkt_info);
void ntoh_tcp(struct sniff_tcp *tcp);
void ntoh_ip(struct sniff_ip *ip);
u_char * slice_packet(const u_char *packet, pkt_info_t *pkt_info, tcp_session_t *tcp_session);
int same_session(tcp_session_t *ses, pkt_info_t *pkt_info);
void read_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
int read_pcap_from_file(char *pcap_file_path);

#endif
