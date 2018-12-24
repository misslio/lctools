#include "iridiumlog_conf.h"

static int lci_add_info(log_conf_infos_t *log_conf_infos, single_log_conf_info_t *info, int is_total_switch);
static int irilog_conf_line_handle(single_log_conf_info_t *info, char *name, char *value);
static log_conf_infos_t * log_conf_infos_init();
static void set_default_single_log_conf_info(single_log_conf_info_t *info);
static void fill_log_conf_infos(gpointer key, gpointer value, gpointer user_data);
static void get_info_from_conf_pairs(log_conf_pairs_t *conf_pairs, single_log_conf_info_t *info);



static value_string info_head[] = {
	{INDEX_LOG_LEVEL, "DEBUG.INFO.ERR.FATAL"},
	{INDEX_LOG_FILE, "LOG_FILE.FILE.FUNC.LINE.DATE"},
	{INDEX_LOG_TERMINAL, "LOG_TERMINAL.FILE.FUNC.LINE.DATE"},
	{INDEX_LOG_ROLLBAKC_NUM, "ROLLBACK_NUMBER"},
	{INDEX_LOG_SINGLE_MAX_SIZE, "SINGLE_MAX_SIZE"},
	{INDEX_LOG_IS_PROCESS_LOCK, "IS_PROCESS_LOCK"},
	{INDEX_LOG_END, NULL}
};

/* turn a char to digital */
static int is_level_true_inner(char c)
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
 * @des - handle a line context in the configure file
 * @date - 2017-12-15 updated by lc
 **/
static int irilog_conf_line_handle(single_log_conf_info_t *info, char *name, char *value)
{
	int i = 0, j = 0, ret = -1;
	int remember_seq = 0;
	for(i = 0; info_head[i].strptr != NULL; i++){
		if(strcmp(name, info_head[i].strptr) == 0){
			j = 0;

			remember_seq = 0;
			switch(i){
				case INDEX_LOG_LEVEL:
					for(; value[j] != '\0'; j++){
						if(value[j] == '.') 
							continue;
						
						if(remember_seq > 3)
							break;

						ret = is_level_true_inner(value[j]);
						if(ret < 0)
							goto irilog_conf_line_handle_err_exit;
						switch(remember_seq){
							case 0:
								info->irilog_switch.debug = ret;
								break;
							case 1:
								info->irilog_switch.info = ret;
								break;
							case 2:
								info->irilog_switch.err = ret;
								break;
							case 3:
								info->irilog_switch.fatal = ret;
								break;
							default:
								break;
						}

						remember_seq++;
					}
					break;

				case INDEX_LOG_FILE:
					for(; value[j] != '\0'; j++){
						if(value[j] == '.')
							continue;
						if(remember_seq > 4)
							break;

						ret = is_level_true_inner(value[j]);
						if(ret < 0)
							goto irilog_conf_line_handle_err_exit;
						switch(remember_seq){
							case 0:
								info->irilog_switch.log_file = ret;
								break;
							case 1:
								info->irilog_switch.f_file = ret;
								break;
							case 2:
								info->irilog_switch.f_func = ret;
								break;
							case 3:
								info->irilog_switch.f_line = ret;
								break;
							case 4:
								info->irilog_switch.f_date = ret;
								break;
							default:
								break;
						}

						remember_seq++;
					}
					break;
				case INDEX_LOG_TERMINAL:
					for(; value[j] != '\0'; j++){
						if((value[j] == '.'))
							continue;
						if(remember_seq > 4)
							break;

						ret = is_level_true_inner(value[j]);
						if(ret < 0)
							goto irilog_conf_line_handle_err_exit;
						switch(remember_seq){
							case 0:
								info->irilog_switch.log_terminal = ret;
								break;
							case 1:
								info->irilog_switch.p_file = ret;
								break;
							case 2:
								info->irilog_switch.p_func = ret;
								break;
							case 3:
								info->irilog_switch.p_line = ret;
								break;
							case 4:
								info->irilog_switch.p_date = ret;
								break;
							default:
								break;
						}

						remember_seq++;
					}
					break;
				case INDEX_LOG_ROLLBAKC_NUM:
				{
					sscanf(value+j, "%d", &info->rollback_num);
				}
				break;
				case INDEX_LOG_SINGLE_MAX_SIZE:
				{
					sscanf(value+j, "%u", &info->single_max_size);
				}
				break;
				case INDEX_LOG_IS_PROCESS_LOCK:
				{
					sscanf(value+j, "%d", &info->is_process_lock);
				}
				break;
				default:
					break;
			}

			
			break;
		}
	}
	return 0;
irilog_conf_line_handle_err_exit:
	return -1;
}

static void get_info_from_conf_pairs(log_conf_pairs_t *conf_pairs, single_log_conf_info_t *info)
{
	
	int i = 0;
	for(; i < conf_pairs->cur_pos; i++){
		irilog_conf_line_handle(info, conf_pairs->pairs[i].name, conf_pairs->pairs[i].value);
	}	
}

static void fill_log_conf_infos(gpointer key, gpointer value, gpointer user_data)
{
	char *log_file_name = (char *)key;
	log_conf_pairs_t *conf_pairs = (log_conf_pairs_t *)value;
	log_conf_infos_t *log_conf_infos = (log_conf_infos_t *)user_data;
	single_log_conf_info_t *info = NULL;
	int id = -1;
	int *int_value;

	info = (single_log_conf_info_t *)g_malloc(sizeof(single_log_conf_info_t));
	memset(info, 0, sizeof(single_log_conf_info_t));
	strncpy(info->name, log_file_name, FILE_NAME_LEN - 1);
	
	get_info_from_conf_pairs(conf_pairs, info);
	if(strcmp(info->name, TOTAL_SWITCH_MODE_NAME) != 0){
		id = lci_add_info(log_conf_infos, info, INFOS_ID_START);
	}else{
		char *value = log_get_value(conf_pairs, TOTAL_SWITCH_IS_VALID);
		if(!value){
			PRINT_ERR_FATAL("[%s] is missing", TOTAL_SWITCH_IS_VALID);
		}
		
		info->is_valid = is_level_true_inner(*value);
		if(info->is_valid < 0){
			PRINT_ERR_FATAL("value of [%s] is invalid\n", TOTAL_SWITCH_IS_VALID);
		}

		value = log_get_value(conf_pairs, TOTAL_SWITCH_TIMER);
		if(!value){
			PRINT_ERR_FATAL("[%s] is missing", TOTAL_SWITCH_TIMER);
		}
		
		info->timer = atoi(value);
		if(info->timer < 0){
			PRINT_ERR_FATAL("value of [%s] is invalid\n", TOTAL_SWITCH_TIMER);
		}
		id = lci_add_info(log_conf_infos, info, INFOS_ID_TOTAL_SWITCH);
	}
	int_value = (int *)g_malloc(sizeof(int));
	*int_value = id;
	g_hash_table_insert(log_conf_infos->names, info->name, int_value);	
}


/**
 * @desc - add a info to log_conf_infos
 * @date - 2017-12-15 created by lc
 **/
static int lci_add_info(log_conf_infos_t *log_conf_infos, single_log_conf_info_t *info, int special_id)
{
	int id;
	
	if(log_conf_infos->len >= log_conf_infos->allocated_len){
		if(!log_conf_infos->sinfo){
			log_conf_infos->allocated_len = DEFAULT_LOG_CONF_FILES_MEM > 3 ? DEFAULT_LOG_CONF_FILES_MEM: 3;
			log_conf_infos->len = INFOS_ID_START;	/* 自动增长的初始值 */
			log_conf_infos->sinfo = (single_log_conf_info_t **)g_malloc(log_conf_infos->allocated_len * sizeof(single_log_conf_info_t *));
			log_conf_infos->sinfo[INFOS_ID_CTL] = NULL;
			log_conf_infos->sinfo[INFOS_ID_DEFAULT] = NULL;
			log_conf_infos->sinfo[INFOS_ID_TOTAL_SWITCH] = NULL;
		}else{
			log_conf_infos->allocated_len += 10;
			log_conf_infos->sinfo = (single_log_conf_info_t **)g_realloc(log_conf_infos->sinfo, log_conf_infos->allocated_len * sizeof(single_log_conf_info_t *));
		}
	}

	if(special_id >= INFOS_ID_START){	/* 自增长处理 */
		log_conf_infos->sinfo[log_conf_infos->len] = info;
		log_conf_infos->len++;

		id = log_conf_infos->len - 1;
	}else if(special_id >= 0){
		if(log_conf_infos->sinfo[special_id]){	/* 特殊处理 */
			g_free(log_conf_infos->sinfo[special_id]);
			log_conf_infos->sinfo[special_id] = NULL;
		}
		
		log_conf_infos->sinfo[special_id] = info;
		id = special_id;
	}


	return id;
}


static void set_default_single_log_conf_info(single_log_conf_info_t *info)
{
	info->rollback_num = DEFAULT_LOG_ROLLBACK_NUMBER;
	info->single_max_size = DEFAULT_LOG_SINGLE_MAX_SIZE;
	info->is_valid = 0;
	info->irilog_switch.p_file = 0;
	info->irilog_switch.p_func = 0;
	info->irilog_switch.p_line = 0;
	info->irilog_switch.p_date = 0;
	info->irilog_switch.f_file = 1;
	info->irilog_switch.f_func = 1;
	info->irilog_switch.f_line = 1;
	info->irilog_switch.f_date = 1;
	info->irilog_switch.log_terminal = 0;
	info->irilog_switch.log_file = 1;

	info->irilog_switch.debug = 0;
	info->irilog_switch.info = 1;
	info->irilog_switch.err = 1;
	info->irilog_switch.fatal = 1;
}


/**
 * @desc - 申请并初始化一个infos
 * @date - 2017-12-15 created by lc
 **/
static log_conf_infos_t * log_conf_infos_init()
{
	log_conf_infos_t *log_conf_infos = (log_conf_infos_t *)g_malloc(sizeof(log_conf_infos_t));
	int id;
	int *value = NULL;

	memset(log_conf_infos, 0, sizeof(log_conf_infos_t));
	log_conf_infos->names = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, g_free);

	single_log_conf_info_t *info = NULL;
	
	info = (single_log_conf_info_t *)g_malloc(sizeof(single_log_conf_info_t));
	memset(info, 0, sizeof(single_log_conf_info_t));
	info->log_switch = 1;
	info->is_valid = 0;
	id = lci_add_info(log_conf_infos, info, INFOS_ID_CTL);

	info = (single_log_conf_info_t *)g_malloc(sizeof(single_log_conf_info_t));
	memset(info, 0, sizeof(single_log_conf_info_t));
	set_default_single_log_conf_info(info);
	id = lci_add_info(log_conf_infos, info, INFOS_ID_TOTAL_SWITCH);
	
	info = (single_log_conf_info_t *)g_malloc(sizeof(single_log_conf_info_t));
	memset(info, 0, sizeof(single_log_conf_info_t));
	strncpy(info->name, DEFAULT_LOG_FILE_NAME, FILE_NAME_LEN - 1);	
	set_default_single_log_conf_info(info);
	id = lci_add_info(log_conf_infos, info, INFOS_ID_DEFAULT);
	value = (int *)g_malloc(sizeof(int));
	*value = id;
	g_hash_table_insert(log_conf_infos->names, info->name, value);
	
	return log_conf_infos;
}

/**
 * @desc - destroy infos. In most cases, this function is for memory test.
 * @date - 2017-12-15 created by lc
 **/
void log_conf_infos_destroy(log_conf_infos_t *infos)
{
	int i = 0;
	if(infos){
		if(infos->sinfo){
			for(; i < infos->len; i++){
				if(infos->sinfo[i]){
					g_free(infos->sinfo[i]);
					infos->sinfo[i] = NULL;
				}
			}
			g_free(infos->sinfo);
			infos->sinfo = NULL;
		}
		if(infos->names){
			g_hash_table_destroy(infos->names);
			infos->names = NULL;
		}
		g_free(infos);
	}
}
/**
 * @desc - get log files configure infos from conf_file
 * @param - conf_file : configure file name
 * @date - 2017-12-15 created by lc
 **/
log_conf_infos_t * get_log_conf_infos(char *conf_file)
{	
	log_iri_conf_manager_t conf_manager;
	log_conf_infos_t * log_conf_infos = NULL;
	int ret = 0;
	memset(&conf_manager, 0, sizeof(log_iri_conf_manager_t));

	log_conf_infos = log_conf_infos_init();
	
	ret = log_get_conf_info_from_file(conf_file, &conf_manager);
	if(ret == 0){
		log_conf_infos->sinfo[INFOS_ID_CTL]->log_switch = conf_manager.log_switch;
		log_conf_manager_foreach(&conf_manager, fill_log_conf_infos, log_conf_infos);
	}

	log_conf_infos->sinfo[INFOS_ID_CTL]->is_valid = log_conf_infos->sinfo[INFOS_ID_TOTAL_SWITCH]->is_valid;
	
	log_destroy_iri_conf(&conf_manager);
	
	return log_conf_infos;	
}

single_log_conf_info_t * get_log_conf_info(log_conf_infos_t *infos, int id)
{
	if(!infos || id < 0 || id >= infos->len)	
		return NULL;

	return infos->sinfo[id];
}

void go_through_log_conf_infos(log_conf_infos_t *infos)
{
	if(!infos){
		PRINT_ERR_FATAL("input error!\n");
	}

	printf("************ count : %d\n\n", infos->len);
	int i;
	for(i = 0; i < infos->len; i++){
		if(INFOS_ID_CTL == i){
			printf("---LOG_SWITCH: %d, IS_VALID:%d\n", infos->sinfo[i]->log_switch, infos->sinfo[i]->is_valid);
			continue;
		}else if(INFOS_ID_TOTAL_SWITCH == i){
			printf("---timer:%d,", infos->sinfo[i]->timer);
		}
		
		printf("%sfile_name: %s\n", 
			i > INFOS_ID_TOTAL_SWITCH ? "---":"  ", i == INFOS_ID_TOTAL_SWITCH ? "TOTAL_SWITCH[no file name]" : infos->sinfo[i]->name);
		printf("	rollback_num: %d\n", infos->sinfo[i]->rollback_num);
		printf("	single_max_size: %u\n", infos->sinfo[i]->single_max_size);
		printf("	is_process_lock: %s\n", infos->sinfo[i]->is_process_lock ? "Y":"N");
		printf("		debug:%u, info:%u, err:%u, fatal:%u\n", infos->sinfo[i]->irilog_switch.debug, infos->sinfo[i]->irilog_switch.info, infos->sinfo[i]->irilog_switch.err, infos->sinfo[i]->irilog_switch.fatal);
		printf("		log_terminal:%u, log_file:%u\n", infos->sinfo[i]->irilog_switch.log_terminal, infos->sinfo[i]->irilog_switch.log_file);
		printf("		f_file:%u, f_func:%u, f_line:%u, f_date:%u\n", 
			infos->sinfo[i]->irilog_switch.f_file, infos->sinfo[i]->irilog_switch.f_func, infos->sinfo[i]->irilog_switch.f_line, infos->sinfo[i]->irilog_switch.f_date);
		printf("		p_file:%u, p_func:%u, p_line:%u, p_date:%u\n\n", 
			infos->sinfo[i]->irilog_switch.p_file, infos->sinfo[i]->irilog_switch.p_func, infos->sinfo[i]->irilog_switch.p_line, infos->sinfo[i]->irilog_switch.p_date);
	}
}

