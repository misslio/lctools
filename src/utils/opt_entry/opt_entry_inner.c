#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "include/log.h"
#include "version.h"

#include "opt_entry_inner.h"

#include "config.h"

/* 测试使用 */
static const char *default_table_tag = "01234567890123456789012345678912";

/* 默认的配置文件名称 */
//static const char *default_config_file = "/iridium/iridium_all.conf";

/**
 * @desc - 检测,打印过滤参数
 * @param - f_opt : 过滤模块结构体
 * @author - lc
 * @date - created on 2018-01-25
 **/
static int check_filter_opts(filter_option_t *f_opt)
{	
#if 0
	printf("sip: %s, dip:%s, sport:%u, dport:%u\n"
		"smac:%s, dmac:%s\n"
		"port:%u, ip:%s, is_ipv6:%s, mac:%s, protocol:%s,\n"
		"is_only_check:%s, download_file_name:%s, key_name:%s\n\n",
		f_opt->sip?f_opt->sip:"N", f_opt->dip?f_opt->dip:"N", f_opt->sport, f_opt->dport,\
		f_opt->smac?f_opt->smac:"N", f_opt->dmac?f_opt->dmac:"N",\
		f_opt->port, f_opt->ip, f_opt->is_ipv6? "Y": "N",f_opt->mac?f_opt->mac:"N", f_opt->protocol?f_opt->protocol:"N", \
		f_opt->is_only_check?"Y":"N", f_opt->download_file_name?f_opt->download_file_name:"N",\
		f_opt->key_name?f_opt->key_name:"N");
#endif

	if(!f_opt->start_timestamp || !f_opt->end_timestamp){
		IRIDIUM_LOG_FATAL("-a and -z is needed\n");
		return -1;
	}
	if(f_opt->start_timestamp > f_opt->end_timestamp){
		IRIDIUM_LOG_FATAL("start_timestamp[%lu] is bigger than end_timestamp[%lu]\n", f_opt->start_timestamp, f_opt->end_timestamp);
		return -1;
	}

	if(f_opt->is_only_check && !f_opt->key_name){
		IRIDIUM_LOG_FATAL("-c and -k is needed\n");
		return -1;
	}

	if(f_opt->download_file_name && !f_opt->key_name){
		IRIDIUM_LOG_FATAL("-f and -k is needed\n");
		return -1;
	}

	/* 输入参数冲突检测 */
	if(f_opt->is_only_check && f_opt->download_file_name){
		IRIDIUM_LOG_FATAL("-c should not be with -f\n");
		return -1;
	}

	if((f_opt->sip != 0 || f_opt->dip != 0) && f_opt->ip != 0){
		IRIDIUM_LOG_FATAL("-ip should not be with --sip or/and --dip\n");
		return -1;
	}

	if((f_opt->sport != 0 || f_opt->dport != 0) && f_opt->port != 0){
		IRIDIUM_LOG_FATAL("-port should not be with --sport or/and --dport\n");
		return -1;
	}

	if((f_opt->smac != NULL|| f_opt->smac != NULL) && f_opt->mac != NULL){
		IRIDIUM_LOG_FATAL("-mac should not be with --smac or/and --dmac\n");
		return -1;
	}

	
	return 0;
}

/**
 * @desc - 检测,打印过滤参数
 * @param - f_opt : 深入模块结构体
 * @author - lc
 * @date - created on 2018-01-25
 **/
static int check_deep_opts(deep_dissector_option_t *d_opt)
{
	
#if 0
	printf("is_from_pcap: %s, pcap_file_name:%s\n"
			"d_table_tag:%s\n\n",d_opt->is_from_pcap?"Y":"N", d_opt->pcap_file_name?d_opt->pcap_file_name:"N",\
			d_opt->d_table_tag?d_opt->d_table_tag:"N");
#endif	

	/* 输入参数冲突检测 */
	if(d_opt->is_from_pcap && !d_opt->pcap_file_name){
		IRIDIUM_LOG_FATAL("No pcap file name\n");
		return -1;
	}
	if(!d_opt->is_from_pcap && d_opt->pcap_file_name){
		IRIDIUM_LOG_FATAL("-r should not be with -o\n");
		return -1;
	}
	
	if(d_opt->pkts_num <= 0){
		IRIDIUM_LOG_FATAL("-s error, pkts_num[%d] should be > 0\n", d_opt->pkts_num);
		return -1;
	}
	return 0;
}


static void deep_simple_usage(void)
{
	printf("Version: %s\n"
		   "	\033[32mGet version information\033[0m\n"
		   "./deep_analysis -o -a start_timestamp -z end_timestamp -t table_name -s pkts_num filter_string\n"
		   "	\033[32mRead packet from filter lib\033[0m\n"
		   "./deep_anlaysis -m -r pcap_file -s 100000\n"
		   "	\033[32mRead packet from pcap file\033[0m\n" 
		   "	\033[32m-m : run not as a daemon\033[0m\n"
		   "	\033[32m-r : path of pcap file\033[0m\n"
		   "	\033[32m-s : packets number\033[0m\n", get_version());	
}

/**
 * @desc - 格式化输入参数为C程序内部结构体
 * @param - argc : main函数中传入的参数个数
 *			argv : main函数中传入的参数内容
 *			opts ： 格式好后的结构体
 * @author - lc
 * @date - created on 2018-01-25 
 **/
int getopt_long_entry(int argc, char * const argv[], format_opts_t *opts)
{
	IRIDIUM_LOG_TRACE_MC("Enter this func");
	int c;
	int option_index = 0;
	int f_ret = 0, d_ret = 0, ret = 0;
	int is_only_filter_lib = 0, is_only_deep = 0;
	
	filter_option_t *f_opt = NULL;
	deep_dissector_option_t *d_opt = NULL;

	if(!opts || !opts->f_opt || !opts->d_opt){
		IRIDIUM_LOG_FATAL("opts is invalid\n");
	}

	if(argc < 2){
		printf("Invalid input\n");
		deep_simple_usage();
		IRIDIUM_LOG_FATAL("Invalid input");
		return -1;
	}
	
	f_opt = opts->f_opt;
	d_opt = opts->d_opt;
	opts->is_only_version = 0;
	
	memset(f_opt, 0, sizeof(struct filter_option));
	memset(d_opt, 0, sizeof(struct deep_dissector_option));

	/* 过滤库使用的过滤选项 */
	struct option long_filter_options[] = {
		{"ip", required_argument, NULL, ENTRY_FILTER_IP},
		{"sip",  required_argument, NULL, ENTRY_FILTER_SIP},
		{"dip", required_argument, NULL, ENTRY_FILTER_DIP},
		{"mac", required_argument, NULL, ENTRY_FILTER_MAC},
		{"smac", required_argument, NULL, ENTRY_FILTER_SMAC},
		{"dmac", required_argument, NULL, ENTRY_FILTER_DMAC},
		{"port", required_argument, NULL, ENTRY_FILTER_PORT},
		{"sport", required_argument, NULL, ENTRY_FILTER_SPORT},
		{"dport", required_argument, NULL, ENTRY_FILTER_DPORT},
		{"protocol", required_argument, NULL, ENTRY_FILTER_PROTOCOL},
		{"find_type", required_argument, NULL, ENTRY_FILTER_FIND_TYPE}, 
		{"ipv6", no_argument, NULL, ENTRY_FILTER_IPV6},
		{"domain", required_argument, NULL, ENTRY_FILTER_DOMAIN}, 
		{"application", required_argument, NULL, ENTRY_FILTER_APPID},
		{0, 0, 0, 0}
	};

	/**
	 * -v, -V 打印版本号
	 * 
	 * -o 深入模块从过滤库中读取数据包，不加则从pcap文件中读取
	 * -r 深入模块读取数据包的pcap文件名
	 * -t 深入模块要写入的数据表名称中的标识值，MD5字符串
	 * -p 深入模块配置文件路径
	 * -s 深入模块要解析的总的数据包个数
	 * -m 深入模块自测使用，不开启守护进程
	 * 
	 * -a 过滤库的用于接收读取数据包开始时间
	 * -z 过滤库的用接收读取数据包的结束时间
	 * -f 过滤库用于判断下载保存文件名字
	 * -c 过滤库用于判断是否执行程序用于检测时间区间内的数据包大小，不执行写文件操作
	 * -k 过滤库用于接收web端传进来的key值
	 **/
	const char *opt_short_string = "vVhonmr:s:a:z:t:f:ck:";
	d_opt->is_from_pcap = 1;	/* 默认从pcap文件中读入数据包 */
	d_opt->d_table_tag = default_table_tag;		/* 设置默认的表名标签 */
	
	opts->conf_file = DEFAULT_ANALYSIS_CONFIG_FILE;	/* 默认配置文件路径 */
	
	while((c = getopt_long(argc, argv, opt_short_string, long_filter_options, &option_index)) != -1){
		switch(c){
		case 'v':
		case 'V':	
			printf("%s\n", get_version());
			exit(0);
			break;
		case 'o':
			d_opt->is_from_pcap = 0;
			is_only_filter_lib = 1;
			break;
		case 'r':	/* 深入不依赖过滤库 */
			d_opt->pcap_file_name = optarg;
			is_only_deep = 1;
			break;
		case 'a':
			f_opt->start_timestamp = atol(optarg);
			break;
		case 'z':
			f_opt->end_timestamp = atol(optarg);
			break;
		case 't':	/* 深入数据库表名中的md5字符串 */
			d_opt->d_table_tag = optarg;
			break;
		case 's':
			d_opt->pkts_num = atoi(optarg);
			break;
		case 'p':
			opts->conf_file = optarg;
			break;	
		case 'f':	/* 过滤库只是下载 */
			f_opt->download_file_name = optarg;
			is_only_filter_lib = 1;
			break;
		case 'c':	/* 过滤库只是检测 */
			f_opt->is_only_check = 1;
			is_only_filter_lib = 1;
			break;
		case 'm':	/* 不开启守护进程运行 */
			d_opt->is_non_deamon = 1;
			break;
		case 'k':
			f_opt->key_name = optarg;
			break;
		case ENTRY_FILTER_IP:
			f_opt->ip = optarg;
			break;
		case ENTRY_FILTER_SIP:
			f_opt->sip = optarg;
			break;
		case ENTRY_FILTER_DIP:
			f_opt->dip = optarg;
			break;
		case ENTRY_FILTER_MAC:
			f_opt->mac = optarg;
			break;
		case ENTRY_FILTER_SMAC:
			f_opt->smac = optarg;
			break;
		case ENTRY_FILTER_DMAC:
			f_opt->dmac = optarg;
			break;
		case ENTRY_FILTER_PORT:
			f_opt->port = atoi(optarg);
			break;
		case ENTRY_FILTER_SPORT:
			f_opt->sport = atoi(optarg);
			break;
		case ENTRY_FILTER_DPORT:
			f_opt->dport = atoi(optarg);
			break;
		case ENTRY_FILTER_PROTOCOL:
			f_opt->protocol = optarg;
			break;
		case ENTRY_FILTER_FIND_TYPE:
			f_opt->find_type = atoi(optarg);
			break;
		case ENTRY_FILTER_IPV6:
			f_opt->is_ipv6 = 1;
			break;
		case ENTRY_FILTER_DOMAIN:
			f_opt->domain_id = optarg;
			break;
		case ENTRY_FILTER_APPID:
			f_opt->app_id = optarg;
			break;
		case 'h':
			deep_simple_usage();
			exit(0);
			break;
		case '?':
			printf("[?] invalid input parameter[%d]\n", c);
			deep_simple_usage();
			IRIDIUM_LOG_FATAL("[?] invalid input parameter[%d]\n", c);
			break;
		default:
			printf("[default] invalid input parameter[%d]\n", c);
			deep_simple_usage();
			IRIDIUM_LOG_FATAL("[default] invalid input parameter[%d]\n", c);
			break;
		}
	}

	/**
	 * 1 过滤库只做检测和下载时，不检测深入的参数 
	 * 2 深入从pcap文件读取时，不检测过滤库参数
	 **/

	/* 以下判断顺序根据项目实际需要排序 */
	if(!is_only_filter_lib && !is_only_deep){	/* 检测两个模块 */
		f_ret = check_filter_opts(f_opt);
		d_ret = check_deep_opts(d_opt);
	}else if(is_only_filter_lib){	/* 只检测过滤库 */
		f_ret = check_filter_opts(f_opt);
	}else{	/* 只检测深入 */
		d_ret = check_deep_opts(d_opt);
	}
	
	ret = (!f_ret && !d_ret)? 0:-1;
	if(ret){
		deep_simple_usage();
		IRIDIUM_LOG_FATAL("input parameter check fail.\n");
	}

	IRIDIUM_LOG_TRACE_MC("Leave this func");
	return ret;
}

