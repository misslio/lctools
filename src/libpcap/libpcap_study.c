#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pcap.h>

int main()
{
	char err_buf[PCAP_ERRBUF_SIZE];
	pcap_if_t *all_dev;

	memset(err_buf, 0, PCAP_ERRBUF_SIZE);

//	pcap_datalink();

	pcap_findalldevs(&all_dev, err_buf);

	pcap_if_t *ptr = all_dev;

	while(ptr != NULL){
		printf("%s %d ", ptr->name, ptr->flags);
		if(ptr->description)
			printf("%s\n", ptr->description);
		printf("\n");

		ptr = ptr->next;
	}

	pcap_freealldevs(all_dev);

	return 0;
}
