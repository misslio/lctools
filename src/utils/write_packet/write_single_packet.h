/*
 * Copyright 2015‐2018 77Element Co,Ltd. All Rights Reserved
 * @file ‐ write_single_packet.c
 * @brief ‐ 将单独的数据包写入pcap文件，用于存储有问题的数据包
 * @author ‐ lc
 * @date ‐ 2018-06-10
 */

#ifndef UTILS_WRITE_SINGLE_PACKET_H
#define UTILS_WRITE_SINGLE_PACKET_H

/* 将一个数据包写入pcap文件 */
int write_this_packet(guint64 ts, const char *data_buf, int len);

#endif

