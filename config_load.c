
#include "config_load.h"

int CONFIG_PARAM1 = 1;
char *CONFIG_PARAM2 = NULL;


/*********************************************************************
 * zbx_module_load_config                                            *
 *********************************************************************/
void     zbx_module_load_config(void)
{

	char *MODULE_CONFIG_FILE = NULL;

	static struct cfg_line  module_cfg[] =
	{
	/* PARAMETER,			VAR,			TYPE,
                        MANDATORY,		MIN,			MAX */
		{"Param1",              &CONFIG_PARAM1,		TYPE_INT,
                        PARM_OPT,		1,			100},
		{"Param2",              &CONFIG_PARAM2,		TYPE_STRING,
                        PARM_MAND,		0,			0},
		{NULL, NULL, 0,0,0,0}
	};


	MODULE_CONFIG_FILE = zbx_dsprintf(MODULE_CONFIG_FILE, "%s/%s",
	                                  CONFIG_LOAD_MODULE_PATH, MODULE_CONFIG_FILE_NAME);

	zabbix_log(LOG_LEVEL_DEBUG, "[%s] load conf:%s", MODULE_NAME, MODULE_CONFIG_FILE);

	parse_cfg_file(MODULE_CONFIG_FILE, module_cfg, ZBX_CFG_FILE_REQUIRED, ZBX_CFG_STRICT);

	zbx_free(MODULE_CONFIG_FILE);

	zbx_module_set_defaults();


}


void     zbx_module_set_defaults(void)
{

        if (NULL == CONFIG_PARAM2){
                CONFIG_PARAM2 = zbx_strdup(CONFIG_PARAM2, "hello");
	}

}


