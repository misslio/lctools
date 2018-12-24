/****
 * @file - opt_entry.h
 * @desc - 输入参数处理。将深入参数格式化成特定应用场景下的结构体
 * @detail - 目前该模块在过滤库和深入代码中使用
 * @author - lc
 * @date - created on 2018-01-25
 ****/

#ifndef OPT_ENTRY_H
#define OPT_ENTRY_H

#include <stdio.h>

#define OPT_ENTRY_VERSION_STRING	"opt_entry-v1.0[2022]"

/**
 * @desc - 读包模块使用的格式结构体
 * @date - 2018-01-25
 * @author - lc
 **/
struct filter_option{
	const char 	*sip;
	const char	*dip;
	unsigned short sport;
	unsigned short dport;

	const char *smac;
	const char *dmac;

	const char *domain_id;
	const char *app_id;
	/* 用于不区分方向的过滤 */
	unsigned short port;
	const char	*ip;
	int is_ipv6;
	const char *mac;
	const char *protocol;

	int find_type;

	unsigned long start_timestamp;
	unsigned long end_timestamp;

	int is_only_check;
	const char *download_file_name;

	const char *key_name;
};


/**
 * @desc - 深入模块使用的格式结构体
 * @date - 2018-01-25
 * @author - lc
 **/
struct deep_dissector_option{	
	int is_from_pcap;	/* 是从过滤库读数据包，还是从pcap文件来读数据包 */
	const char *pcap_file_name;
	int is_non_deamon;	/* 是否不开启守护进程，0表示开启，1表示不开启，默认开启 */
	
	const char *d_table_tag;
	int pkts_num;	/* 深入要解析的总的数据包个数 */
};


struct format_opts{
	const char *conf_file;
	struct filter_option *f_opt;
	struct deep_dissector_option *d_opt;
	int is_only_version; /* 是否只是打印版本信息 */
};

int getopt_long_entry(int argc, char * const argv[], struct format_opts *format_opts);

#endif
