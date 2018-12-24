/****
 * Copyright 2015‐2018 77Element Co,Ltd. All Rights Reserved
 * @file ‐ iridium_conf_core.c
 * @brief ‐ 读取配置文件的核心实现代码
 * @author ‐ lc
 * @date ‐ 2018‐04-21
 ****/
#include "include/log.h"

#include "iridium_conf_core.h"

#define INDEX_LOG_LEVEL	0
#define INDEX_LOG_FILE	1
#define INDEX_LOG_TERMINAL	2

#define LOG_SWITCH_STR		"CONF_SWITCH"

static void insert_new_node(iri_conf_manager_t *conf_manager, conf_pairs_t* conf_pairs);
static void init_manager(iri_conf_manager_t *conf_manager);
static conf_pairs_t * handle_conf_block(char *start, char *end);
static int iri_conf_line_handle(single_conf_pair_t *single_pair, char *buf, int len);
static int get_string(char *dst, char *src, int len);
static int is_level_true(char c);
static int check_comment_line(char *start, int len);



/* turn a char to digital */
static int is_level_true(char c)
{
	if(!isdigit(c))
		return -1;
	
	char tmp[2];
	tmp[0] = c;
	tmp[1] = '\0';
	int ret = atoi(tmp);
	
	return (ret == 1) ? 1 : ((ret == 0) ? 0 : -1); 
}


/**
 * @brief - insert a new key-value to hash table
 * @author - lc
 * @date - 2017-05-04
 * @ret - success 1， failed -1
 **/
static void insert_new_node(iri_conf_manager_t *conf_manager, conf_pairs_t* conf_pairs)
{
	if(conf_manager && conf_manager->iri_conf && conf_pairs)
		g_hash_table_insert(conf_manager->iri_conf, conf_pairs->mode_name, conf_pairs);
}


/**
 * @brief - for hash table
 * @author - lc
 * @date - 2017-05-04
 * @ret - success 1， failed -1
 **/
void key_value_destroy(gpointer data)
{
	if(data){
		free(data);
		data = NULL;
	}
}


/**
 * @brief - init configure mangager.
 * @author - lc
 * @date - 2017-05-04
 * @ret - success 1， failed -1
 * @TO DO - should exit when failure?
 **/
static void init_manager(iri_conf_manager_t *conf_manager)
{
	if(!conf_manager)
		return;
	/* The key will be freed when value is freed. */
	conf_manager->iri_conf = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, key_value_destroy);
	conf_manager->init_flag = IRI_CONF_FINISH_INIT;
	conf_manager->mode_count = 0;
}

/* destroy the hash table inside conf_manager */
void destroy_iri_conf(iri_conf_manager_t *conf_manager)
{
	if(conf_manager && conf_manager->iri_conf)
		g_hash_table_destroy(conf_manager->iri_conf);
}


/**
 * @brief - This will return all information of a mode which named mode_name.
 * @author - lc
 * @date - 2017-05-04
 **/
conf_pairs_t * get_conf_pairs(iri_conf_manager_t *conf_manager, char *mode_name)
{
	if(conf_manager && conf_manager->iri_conf)
		return (conf_pairs_t *)g_hash_table_lookup(conf_manager->iri_conf, mode_name);
	else 
		return NULL;
}

/* simple function. input your name of key, return the value */
char *get_value(conf_pairs_t * conf_pairs, char *name)
{
	int i = 0; 
	
	for(; i < conf_pairs->cur_pos; i++){
		if(strcmp(name, conf_pairs->pairs[i].name) == 0)
			return conf_pairs->pairs[i].value;
	}
	
	return NULL;
}


void print_key_value(gpointer key, gpointer value, gpointer user_data)
{
    int i; 
	conf_pairs_t * conf_pairs = (conf_pairs_t * )value;
	
	PRINT_DEBUG("----mode_name : %s\n", (char *)key);
	for(i = 0; i < conf_pairs->cur_pos; i++){
		PRINT_DEBUG("  ---name:%s, value:%s\n", conf_pairs->pairs[i].name, conf_pairs->pairs[i].value);
	}
}

/* This is a test function. */
void go_through_conf_manager(iri_conf_manager_t *conf_manager)
{
	g_hash_table_foreach(conf_manager->iri_conf, print_key_value, NULL);
}

/**
 * @brief - read configure info from iridiumlog.con
 * @ret - success : 0
 * 		  fail : < 0
 * @author - lc
 * @date - 2017-05-04
 * @TO DO - if our configuration file is missing, return 1, so code will continue working.
 *			because this shouldn't be an error.	-- 2016-10-25 by lc.
 **/
int get_conf_info_from_file(const char *conf_file, iri_conf_manager_t *conf_manager)
{
	g_assert(conf_file);
	g_assert(conf_manager);
	
	if(conf_manager->init_flag != IRI_CONF_FINISH_INIT)
		init_manager(conf_manager);
	else 
		return REPEAT_INIT_ERR;

	FILE *fp = NULL;
	int len = 0, ret = 0;
	char *p, *end, *block_start = NULL;

	char log_switch_buf[128];
	/* open configure file */
	fp = fopen(conf_file, "r");
	if(NULL == fp){
		return OPEN_FILE_ERR;
	}

	char *rd_buf = NULL;
	int block_start_flag = 1;
	conf_pairs_t *new_pairs = NULL;
	
	rd_buf = (char *)malloc(MAX_CONF_FILE_SIZE);
	if(NULL == rd_buf)
		return MALLOC_BUF_ERR;
	
	/* The size of iridiumlog.conf should at most 1K
	 * This is not correct.
	 * We shouldn't limit the file size.
	 */
	
	memset(rd_buf, 0,MAX_CONF_FILE_SIZE);
	if((len = fread(rd_buf, 1,MAX_CONF_FILE_SIZE, fp)) < 0){
		ret = READ_FILE_ERR;
		goto get_conf_info_from_file_err_exit;
	}

	p = rd_buf;
	end = p + len;

	while(p < end){
		/* ignore the space and tab at the beginning of line */
		while((p < end) && (*p != '\n') && ((*p == ' ') || (*p == '\t'))) p++;
		/* start our context handling */
		if(*p == '#'){ 	/* comment */
			while((p != end) && (*p != '\n'))
				p++;
		} if(*p == '@') {	/* configure switch */
			memset(log_switch_buf, 0, 128);
			int i = 0, j = 0;
			p++;
			while((p != end) && (*p != '\n')){
				if(i < 125)
					log_switch_buf[i++] = *p++;
				else
					p++;
			}
			log_switch_buf[i] = '\0';
			if(strstr(log_switch_buf, LOG_SWITCH_STR) != NULL){
				while(j < i) if (log_switch_buf[j++] == '=') break;
				for(; j < i; j++){
					if ((log_switch_buf[j] == ' ') || (log_switch_buf[j] == '\t')) 
						continue;
					else
						break;
				}

				if(j < i){
					if(!is_level_true(log_switch_buf[j])){
						ret = CONF_CLOSE_ALL;
						goto get_conf_info_from_file_err_exit;
					}	
				}
			}
		} else {
			if((*p == '-') && (!block_start_flag)){	
				/* get block */
				if(strncmp(p, "-end", 4) != 0){
					PRINT_ERR("end error, need destroy!\n");
					goto get_conf_info_from_file_err_exit;
				}
				block_start_flag = 0;					
				new_pairs = handle_conf_block(block_start, p);
				
				if(NULL != new_pairs){
					insert_new_node(conf_manager, new_pairs);
					conf_manager->mode_count++;
				}
				block_start_flag = 1;
			}else if(*p == '-'){
				block_start = ++p;
				block_start_flag = 0;
			}	


			while((p != end) && (*p != '\n'))
				p++;
			
			if(p == end)
				break;
		}

		p++;
	}
	ret = 0;
get_conf_info_from_file_err_exit:
	fclose(fp);
	LC_FREE_MALLOC(rd_buf);	
	if(ret != 0)
		destroy_iri_conf(conf_manager);
	return ret;
}


/**
 * @brief -  handle the single block from file.
 * @ret : success - a malloc point.
 * 		  fail - NULL
 * @author - lc
 * @date - 2017-05-04
 **/
static conf_pairs_t * handle_conf_block(char *start, char *end)
{
	if((NULL == start) || (NULL == end))
		return NULL;

	conf_pairs_t *conf_pairs = NULL;
	char *p = start, *line_start;
	int len = 0;
	
	while(p != end){
		line_start = p;
		while((p != end) && (*p != '\n'))
			p++;

		if(p == end)
			break;
		/* Check whether this line is a comment line */

		if((p - line_start) > (MAX_LINE_LEN - 1))
			break;
		
		len = check_comment_line(line_start, p - line_start);
		
		if(len > 0){ 	/* valid line, we will handle it. */
			if(NULL == conf_pairs){	/* first line, we just need a mode name easily */
				/* Get a new node */
				conf_pairs = (conf_pairs_t *)malloc(sizeof(conf_pairs_t));
				if(NULL == conf_pairs)
					return NULL;
				
				memset(conf_pairs, 0, sizeof(conf_pairs_t));
				conf_pairs->cur_pos = 0;
				if((get_string(conf_pairs->mode_name, line_start,len)) < 0){
					goto handle_conf_block_err_exit;
				}
			} else { /* Here, we get our key-value pairs */
				if(iri_conf_line_handle(&(conf_pairs->pairs[conf_pairs->cur_pos]), line_start, len) < 0){
					goto handle_conf_block_err_exit;
				}else{
					conf_pairs->cur_pos++;
					if(conf_pairs->cur_pos >= CONF_MAX_NODE_NUM)
						break;
				} 

			}
		}

		p++; /* Jump the beginning of next line */
	
	}
	
	goto handle_conf_block_exit;
handle_conf_block_err_exit:	
	LC_FREE_MALLOC(conf_pairs);
handle_conf_block_exit:
	return conf_pairs;
}


#define CONF_MAX_LEN	128


/**
 * @brief -  Check comment in a line context.
 * @params -	start : line start pointer
 * 			len : length of this line.
 * @ret	- 0: This is a total comment line.
 * 		  > 0 : valid length of this line. This value <= len.
 * @author - lc
 * @date - 2017-05-04
 **/
static int check_comment_line(char *start, int len)
{
	char buf[MAX_LINE_LEN];
	char *comment_ptr = NULL;
	int i;
	memset(buf, 0, MAX_LINE_LEN);

	memcpy(buf, start, len);
	buf[len] = '\0';

	if((comment_ptr = strchr(buf, '#')) != NULL){
		for(i = 0; i < comment_ptr - buf; i++){
			if((buf[i] == ' ') || (buf[i] == '\t')){
				continue;
			}else{
				return (comment_ptr - buf);
			}
		}
	}else
		return len;

	return 0;
}


static int get_string(char *dst, char *src, int len)
{
	int i = 0,string_len = 0;
	char line[256];
	char *start;
	memset(line, 0, 256);
	if(-1 == len)
		len = strlen(src);
	memcpy(line, src, len);
	line[len] = '\0';
	while((line[i] != '\0') && ((line[i] == ' ') || (line[i] == '\t'))) i++;
	
	start = &(line[i]);
	while((line[i] != '\0') && (line[i] != ' ') && (line[i] != '\t')) i++;
	string_len = &(line[i]) - start;
	if(string_len < (CONF_STRING_MAX_LEN - 1)){
		memcpy(dst, start, string_len);
		dst[string_len] = '\0';
		if(dst[string_len -1] == '\r') { (dst[string_len -1] = '\0'); }
	}else{
		IRIDIUM_LOG_ERROR_MC("get value fail:[%s]", src);
		return -1;
	}
	
	return 0;
}

/* handle a line context in the configure file */
static int iri_conf_line_handle(single_conf_pair_t *single_pair, char *buf, int len)
{
	if((NULL == single_pair) || (NULL == buf) || (len > 255))
		return -1;

	char line[256];
	memset(line, 0, 256);
	
	memcpy(line, buf, len);
	line[len] = '\0';

	int ret = -1;
	char *mid = NULL;
	mid = strchr(line, '=');
	if(NULL != mid){
		if(!get_string(single_pair->name, line, mid - line))
			if(!get_string(single_pair->value, mid + 1, -1))
				ret = 0;
	}else{
		IRIDIUM_LOG_ERROR_MC("line fail:[%s]", line);
	}
	
	return ret;
}

#if 0
int main()
{
	iri_conf_manager_t conf_manager;
	memset(&conf_manager, 0, sizeof(iri_conf_manager_t));

	get_conf_info_from_file(&conf_manager);
	
	conf_pairs_t *conf_pairs;
	if((conf_pairs = get_conf_pairs(&conf_manager, "example2")) != NULL){
		printf("value = %s\n", get_value(conf_pairs, "example2_key4"));
	}

	go_through_conf_manager(&conf_manager);
	destroy_iri_conf(&conf_manager);
#if 0
	char *str = (char *)malloc(256);
	snprintf(str, 256, "mei#you#");
	PRINT_DEBUG("ret = %d\n", check_comment_line(str, strlen(str)));
	LC_FREE_MALLOC(str);
#endif

	return 0;
}
#endif
