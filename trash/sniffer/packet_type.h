#ifndef PACKET_TYPE_H
#define PACKET_TYPE_H

#include <stdio.h>
#include <string.h>
#include <pcap.h>
#include <arpa/inet.h>


#define SNIFF_ETHER_ADDR_LEN		6 /* RFC894 */

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
};

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
};

typedef struct _pkt_info{
	struct sniff_ethernet *ethernet;
	struct sniff_ip	*ip;
	struct sniff_tcp *tcp;
	u_char *payload;
}pkt_info_t;

typedef struct _tcp_session{
	struct in_addr ip_src;
	u_short sport;

	struct in_addr ip_des;
	u_short dport;
}tcp_session_t;


#endif
