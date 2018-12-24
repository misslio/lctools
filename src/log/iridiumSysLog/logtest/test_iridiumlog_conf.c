#include "iridiumlog_conf/iridiumlog_conf.h"

void xxx()
{	
#if 0
	log_iri_conf_manager_t conf_manager;
	memset(&conf_manager, 0, sizeof(log_iri_conf_manager_t));

	log_get_conf_info_from_file(&conf_manager);
	
	log_conf_pairs_t *conf_pairs;
	if((conf_pairs = log_get_conf_pairs(&conf_manager, "example2")) != NULL){
		printf("value = %s\n", log_get_value(conf_pairs, "example2_key4"));
	}

	log_go_through_conf_manager(&conf_manager);
	log_destroy_iri_conf(&conf_manager);
#if 0
	char *str = (char *)malloc(256);
	snprintf(str, 256, "mei#you#");
	PRINT_DEBUG("ret = %d\n", check_comment_line(str, strlen(str)));
	LC_FREE_MALLOC(str);
#endif
#endif
}

int main(int argc, char *argv[])
{
	if(argc != 2){
		PRINT_ERR_FATAL("./test xxx.conf\n");
	}
	
	log_conf_infos_t *infos= NULL;

	infos = get_log_conf_infos(argv[1]);
	
	go_through_log_conf_infos(infos);

	log_conf_infos_destroy(infos);

	return 0;
}


