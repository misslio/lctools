#include <stdio.h> 
#include <fcntl.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
 
#include <sys/ioctl.h> 


#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <linux/if.h> 
#include <errno.h>
#include <linux/hdreg.h>

#include <scsi/sg.h>
#include <sys/ioctl.h>


static void add_interface_name(char *ifnames, const char * name, int *count) 
{ 
    int i; 
    for (i=0;i<*count;i++) 
    { 
        if (!strcmp(ifnames+i*IFNAMSIZ, name)) 
            return; 
    } 
    strncpy(ifnames+((*count)++)*IFNAMSIZ, name, IFNAMSIZ-1); 
} 
 
static char * get_name(char *name, char *p) 
{ 
    while (isspace(*p)) 
    p++; 
    while (*p) { 
	    if (isspace(*p)) 
	        break; 
	    if (*p == ':') {   
	        char *dot = p, *dotname = name; 
	        *name++ = *p++; 
	        while (isdigit(*p)) 
	        *name++ = *p++; 
	        if (*p != ':') {  
	        p = dot; 
	        name = dotname; 
	        } 
	        if (*p == '\0') 
	        return NULL; 
	        p++; 
	        break; 
	    } 
	    *name++ = *p++; 
    } 
    *name++ = '\0'; 
    return p; 
} 

/* Get adapter name for file /proc/net/dev  */ 
static int get_procnet_list(char *ifnames, int *count) 
{ 
    FILE *fh; 
    char buf[512]; 
    fh = fopen("/proc/net/dev", "r"); 
    if (!fh) 
        return -1; 
 	
	char name[IFNAMSIZ]; 
    fgets(buf, sizeof buf, fh); /* eat title lines */ 
    fgets(buf, sizeof buf, fh); 
    while (fgets(buf, sizeof buf, fh)) 
    { 
    	memset(name, 0, IFNAMSIZ);
        get_name(name, buf); 
        add_interface_name(ifnames, name, count); 
    } 
    fclose(fh); 
    return 0; 
} 

/*
 * func - mac_addr_sys
 * des - User interface. Get all mac from device.
 * para - @addr: A buf to store all the mac address. See the func main below.
 *		- @adapter-num : A value-result parameter. When as value, it means the max numbers of adapter we set.
 *						 When as result, means the actual adapter numbers from device.
 * ret - success : 0
 *       fail : -1
 */
static int mac_addr_sys ( u_char (*addr)[SINGLE_MAC_LEN], int *adapter_num) 
{ 
    struct ifreq ifr; 
    struct ifreq *IFR; 
    char buf[1024]; 
    int s, i,j, count = 0, ok =0; 

 	
	char ifname_buf[2048]; 
    memset(ifname_buf, 0, sizeof(ifname_buf)); 
 
	
    struct ifconf ifc; 

    s = socket(AF_INET, SOCK_DGRAM, 0); 
    if (s==-1) { 
        return -1; 
    } 
#ifdef IOCTL_GET_NAME	
	/* This method will get the same mac address, such as "eth0:1" which is same with "eth0". 
	 * So we don't use.
	 */
    ifc.ifc_len = sizeof(buf); 
    ifc.ifc_buf = buf; 
    ioctl(s, SIOCGIFCONF, &ifc); 
 
    IFR = ifc.ifc_req; 
    for (i = ifc.ifc_len / sizeof(struct ifreq); --i >= 0; IFR++) { 
            add_interface_name(ifname_buf ,IFR->ifr_name, &count); 
    }
#else
    if (get_procnet_list(ifname_buf, &count)) 
        return -1; 
#endif 
    int got_count = 0;
	char tmp_mac[SINGLE_MAC_LEN];
    for (i = 0; i < count; i++) { 
        strcpy(ifr.ifr_name, ifname_buf + i*IFNAMSIZ); 
        if (ioctl(s, SIOCGIFFLAGS, &ifr) == 0) { 
            if (!(ifr.ifr_flags & IFF_LOOPBACK)) {  	
                if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {  	
                	snprintf (tmp_mac, SINGLE_MAC_LEN, "%02X:%02X:%02X:%02X:%02X:%02X", (u_char) ifr.ifr_hwaddr.sa_data[0], \
						(u_char) ifr.ifr_hwaddr.sa_data[1], (u_char) ifr.ifr_hwaddr.sa_data[2], \
						(u_char) ifr.ifr_hwaddr.sa_data[3], (u_char) ifr.ifr_hwaddr.sa_data[4], \
						(u_char) ifr.ifr_hwaddr.sa_data[5]);
					tmp_mac[SINGLE_MAC_LEN] = '\0';					
					ok = 1;
					if(got_count > 0){
						for(j = 0; j < got_count ; j++){
							if(strncmp(addr[j], tmp_mac, SINGLE_MAC_LEN) == 0){
								ok = 1;
								#ifdef GOT_MAC_TEST
								printf("Same mac %s , current net name = %s\n", tmp_mac, ifr.ifr_name);
								#endif
								break;
							} else 
								continue;
						}
					}

					if(ok){
						if(got_count >= NET_ADAPTER_NUM) 	/* 网卡数量大于设定的数量时返回错误 */
							return -1;
						memcpy(addr[got_count], tmp_mac, SINGLE_MAC_LEN);
						addr[got_count][SINGLE_MAC_LEN] = '\0';
						got_count++;
					}
                } 
            } 
        } 
    } 
	
    close(s); 

	*adapter_num = got_count;
    return 0; 
} 

/*
 * func - get_all_mac
 * des - Read all mac address in device.
 * para - @mac_buf: A buf to store all the macs.
 *		  @buf_len: The length of mac_buf.
 * ret - success : 0
 *       fail : -1
 */
int get_all_mac(u_char *mac_buf, int buf_len)

{
	int stat,  ret = 0; 

    int i, len =0; 
    u_char addr[NET_ADAPTER_NUM][SINGLE_MAC_LEN]; 

	memset(addr, 0, NET_ADAPTER_NUM * SINGLE_MAC_LEN * sizeof(u_char));
	int act_adapter_num = NET_ADAPTER_NUM;
    stat = mac_addr_sys(addr, &act_adapter_num); 

    if (0 == stat){ 
		if(buf_len <= (act_adapter_num * SINGLE_MAC_LEN))
			return -1;
        for(i = 0; i < act_adapter_num; i++){
			memcpy(mac_buf+len, addr[i], SINGLE_MAC_LEN-1);
			len += SINGLE_MAC_LEN-1;		
			mac_buf[len] = '-';
			len += 1;
		}
		len--;
		mac_buf[len] = '\0';
    }
    else
		ret = -1;
    
	return ret; 
}

/***********************************************  Get CPU ID  *******************************************************/


/* 23 + 1 */
#define CPU_ID_LEN 24
static int get_cpu_id(u_char *cpu_buf, int cpu_len)
{
	if (0 != getuid())
		return -1;
	if(cpu_len < CPU_ID_LEN)
		return -1;
	
	int ret = 0;
	char command[100] = {0};
	snprintf(command, sizeof(command), "dmidecode -t processor | grep ID | awk -F: '{print $2}' | tail -1 > %s", dmi_result);
	
	if(0 == system(command)){
		FILE* fp = fopen(dmi_result, "r");

		if(fp){
			fseek(fp, 1, SEEK_SET);
			fread(cpu_buf, 1, CPU_ID_LEN-1 ,fp);
			fclose(fp);

			cpu_buf[CPU_ID_LEN-1] = '\0'; 
		}
	} else 
		ret = -1;
	
	unlink(dmi_result);
	
	return ret;
}


static int get_baseboard_id(u_char *baseboard_buf, int baseboard_len)
{
	if (0 != getuid())
		return -1;

	int ret = 0;
	char command[100] = {0};
	snprintf(command, sizeof(command), "dmidecode -s system-serial-number > %s", dmi_result);
	
	if(0 == system(command)){
		FILE* fp = fopen(dmi_result, "r");
		
		if(fp){	
			u_char buf[512];
			int len = 0;
			memset(buf, 0, 512);
			fread(buf, 1, 512 ,fp);
			fclose(fp);
			len = strlen(buf);
			if(len > baseboard_len)
				return -1;
			memcpy(baseboard_buf, buf, len);
			baseboard_buf[len-1] = '\0';
		}
	} else 
		ret = -1;
	
	unlink(dmi_result);
	
	return ret;
}


static int get_disk_id (u_char *disk_buf, u_int disk_len)
{
    int fd;
    struct hd_driveid hid;
	
    fd = open ("/dev/sda", O_RDONLY);
    if (fd < 0)
    {
        return -1;
    }
	int ret ;
	
    if ((ret = ioctl (fd, HDIO_GET_IDENTITY, &hid)) < 0)	
//    if ((ret = ioctl (fd, BLKGETSIZE, &hid)) < 0)
    {
        return -1;
    }
    close (fd);
	
	if(disk_len < strlen(hid.serial_no))
		return -1;
	
    sprintf(disk_buf, "%s",  hid.serial_no);
    return 0;
}

/*
 * des - 获取网卡的主板序列号
 * ret - seccess : 0
 *		 fail : -1
 * date - 2016-05-11
 */
int get_mboard_sin(u_char *sin_buf, int buf_len)
{
	if (0 != getuid())
		return -1;
	if(buf_len < 128)
		return -1;
	
	int ret = -1;
	char command[150] = {0};
	snprintf(command, sizeof(command), "/opt/napatech3/bin/productinfo | grep \"Main Board Serial No\" | awk -F: '{print $2}' | sed 's/^[ \t]*//g' > %s", dmi_result);
	
	if(0 == system(command)){
		FILE* fp = fopen(dmi_result, "r");
		if(fp){
			ret = fread(sin_buf, 1, 128 ,fp);
			fclose(fp);

			if(ret < 11){
				unlink(dmi_result);
				return -1;
			}
			u_int i = 0;	
			for(; i < strlen(sin_buf); i++){
				if(sin_buf[i] == '\n'){
					sin_buf[i] = '\0';
					ret = 0;
				}
			}
		}
	} else 
		ret = -1;

	unlink(dmi_result);
	
	return ret;
}





