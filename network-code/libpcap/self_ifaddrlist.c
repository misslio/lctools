#include <stdio.h>
#include <string.h>
#include <pcap.h>
#include <sys/socket.h>


int self_getaddrlist()
{
	int nipaddr = 0;
	int i = 0;
	pcap_if_t *devlist = NULL;
	pcap_if_t *dev = NULL;
	int8_t err[PCAP_ERRBUF_SIZE];

	if(pcap_findalldevs(&devlist, err) == -1){
		fprintf(stderr, "error in pcap_findalldevs:%s", err);
		return -1;
	}

	for(dev = devlist; dev; dev = dev->next){
		struct pcap_addr *pcapaddr;
		for(pcapaddr = dev->addresses; pcapaddr; pcapaddr = pcapaddr->next){
			struct sockaddr *addr = pcapaddr->addr;

			printf("--->name : %s\n", dev->name);
#if 0
			if(dev->flags & PCAP_IF_LOOPBACK)
				continue;
			if(addr->sa_family == AF_INET)
				printf("name : %s\n", dev->name);
			else
				printf("Other device name : %s\n", dev->name);
#endif
#if 0
			if(addr->sa_family == AF_INET){
				ifaddrlist[i].device = strdup(dev->name);
				ifaddrlist[i].addr = ((struct sockaddr_in *)addr)->sin_addr.s_addr;
				++i;
				++nipaddr;
			}
#endif
		}
	
	}

	pcap_freealldevs(devlist);

//	*ipaddrp = ifaddrlist;

	return nipaddr;
}

int main()
{
	self_getaddrlist();

	return 0;
}
