#ifndef OPT_ENTRY_INNER_H
#define OPT_ENTRY_INNER_H

#include "opt_entry.h"

/* 长选项中对应的val值 */
#define ENTRY_FILTER_IP	'A'
#define ENTRY_FILTER_SIP	'B'
#define ENTRY_FILTER_DIP	'C'
#define ENTRY_FILTER_MAC	'D'
#define ENTRY_FILTER_SMAC	'E'
#define ENTRY_FILTER_DMAC	'F'
#define ENTRY_FILTER_PORT	'G'
#define ENTRY_FILTER_SPORT	'H'
#define ENTRY_FILTER_DPORT	'I'
#define ENTRY_FILTER_PROTOCOL	'J'
#define ENTRY_FILTER_FIND_TYPE	'K'
#define ENTRY_FILTER_IPV6		'L'
#define ENTRY_FILTER_DOMAIN		'M'
#define ENTRY_FILTER_APPID		'N'



typedef struct filter_option filter_option_t;
typedef struct deep_dissector_option deep_dissector_option_t;

typedef struct format_opts format_opts_t;

#endif
