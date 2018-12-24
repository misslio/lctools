/*
 * Copyright 2015‐2018 77Element Co,Ltd. All Rights Reserved
 * @file ‐ write_single_packet.c
 * @brief ‐ 将单独的数据包写入pcap文件，用于存储有问题的数据包
 * @author ‐ lc
 * @date ‐ 2018-06-10
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pcap.h>
#include <glib.h>


static pcap_dumper_t* init_pcap_handler(const char *path)
{	
	pcap_t *handler;
	handler = pcap_open_dead(1, 65535); /* 不限制数据包的长度 */
	return pcap_dump_open(handler, path);

}

/**
 * @brief ‐ 将一个数据包写入pcap文件
 * @params ‐ pdumper ： pcap文件指针
 *			 ts	: 数据包时间戳，精度为10ns
 *			 data_buf : 数据包指针
 *			 len ： data_buf的长度
 * @ret ‐ None
 * @author ‐ lc
 * @date ‐ 2018-06-11
 **/
static void iridium_write_pcap_single(pcap_dumper_t *pdumper, guint64 ts, const char *data_buf, int len)
{
	struct pcap_pkthdr hdr;
	
	/* 数据包长度 */
	hdr.caplen = len;
	hdr.len = len;

	/* 数据包时间戳 */
	hdr.ts.tv_sec = ts/100000000;
	hdr.ts.tv_usec = ts/100;
	
	/* 将数据包写入pcap文件 */
	pcap_dump((guint8*)pdumper, &hdr, (const guint8 *)data_buf);
			
	/* 刷新缓冲区 */
	pcap_dump_flush(pdumper);

	return;
}

static void close_pcap_handler(pcap_dumper_t *pdumper)
{
	if(NULL == pdumper)
		return;
	
	pcap_dump_close(pdumper);
}

/**
 * @brief ‐ 将一个数据包写入pcap文件，原则上只写一个数据包到pcap文件，
 *			用于在程序退出时保存当前正在解析的
 * @params ‐ data_buf : 数据包指针
 *			 len ： data_buf的长度
 * @ret ‐ -1: 输入参数错误
 *		  -2: 打开文件错误
 *		  0 : 执行成功
 * @TO DO - 数据包写入目录是否规范为项目目录
 * @author ‐ lc
 * @date ‐ 2018-04-02
 **/
int write_this_packet(guint64 ts, const char *data_buf, int len)
{
	if((data_buf == NULL) || (len <= 0)){ 
		return -1;
	}
	
	pcap_dumper_t *pdumper = init_pcap_handler("/var/log/iridium/iridium_analysis_fatal.pcap");
	if(pdumper){
		iridium_write_pcap_single(pdumper, ts, data_buf, len);
		close_pcap_handler(pdumper);
	}else{
		return -2;
	}

	return 0;
}

