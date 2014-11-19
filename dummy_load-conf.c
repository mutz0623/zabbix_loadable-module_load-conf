/*
 * zabbix loadable module sample
 *
 * -load config
 *  same path of  module
 *  file name describe on this code.
 * -determine daemon type(server, agent or proxy )
 *
 * mutsumi Iwata
 *
 *
 */

/*
** Zabbix
** Copyright (C) 2001-2014 Zabbix SIA
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#include "sysinc.h"
#include "module.h"
#include "log.h"
#include "common.h"
#include "cfg.h"


extern unsigned char daemon_type ;
extern char *CONFIG_LOAD_MODULE_PATH ;


#define MODULE_CONFIG_FILE_NAME "dummy_load-conf.conf"
#define MODULE_NAME "zbx_module_load_config.so"

static void	zbx_module_load_config();
static void	zbx_module_set_defaults(void);

int CONFIG_PARAM1 = 1; 
char *CONFIG_PARAM2 = NULL; 


/* the variable keeps timeout setting for item processing */
static int	item_timeout = 0;

int	zbx_module_dummy_param1(AGENT_REQUEST *request, AGENT_RESULT *result);
int	zbx_module_dummy_param2(AGENT_REQUEST *request, AGENT_RESULT *result);

static ZBX_METRIC keys[] =
/*      KEY                     FLAG		FUNCTION        	TEST PARAMETERS */
{
	{"dummy.param1",		0,		zbx_module_dummy_param1,	NULL},
	{"dummy.param2",		0,		zbx_module_dummy_param2,	"hello"},
	{NULL}
};

/******************************************************************************
 *                                                                            *
 * Function: zbx_module_api_version                                           *
 *                                                                            *
 * Purpose: returns version number of the module interface                    *
 *                                                                            *
 * Return value: ZBX_MODULE_API_VERSION_ONE - the only version supported by   *
 *               Zabbix currently                                             *
 *                                                                            *
 ******************************************************************************/
int	zbx_module_api_version()
{
	return ZBX_MODULE_API_VERSION_ONE;
}

/******************************************************************************
 *                                                                            *
 * Function: zbx_module_item_timeout                                          *
 *                                                                            *
 * Purpose: set timeout value for processing of items                         *
 *                                                                            *
 * Parameters: timeout - timeout in seconds, 0 - no timeout set               *
 *                                                                            *
 ******************************************************************************/
void	zbx_module_item_timeout(int timeout)
{
	item_timeout = timeout;
}

/******************************************************************************
 *                                                                            *
 * Function: zbx_module_item_list                                             *
 *                                                                            *
 * Purpose: returns list of item keys supported by the module                 *
 *                                                                            *
 * Return value: list of item keys                                            *
 *                                                                            *
 ******************************************************************************/
ZBX_METRIC	*zbx_module_item_list()
{
	return keys;
}

int	zbx_module_dummy_param1(AGENT_REQUEST *request, AGENT_RESULT *result)
{

	SET_UI64_RESULT(result, CONFIG_PARAM1);

	return SYSINFO_RET_OK;
}

int	zbx_module_dummy_param2(AGENT_REQUEST *request, AGENT_RESULT *result)
{

        SET_STR_RESULT(result, strdup(CONFIG_PARAM2));

	return SYSINFO_RET_OK;
}


/******************************************************************************
 *                                                                            *
 * Function: zbx_module_init                                                  *
 *                                                                            *
 * Purpose: the function is called on agent startup                           *
 *          It should be used to call any initialization routines             *
 *                                                                            *
 * Return value: ZBX_MODULE_OK - success                                      *
 *               ZBX_MODULE_FAIL - module initialization failed               *
 *                                                                            *
 * Comment: the module won't be loaded in case of ZBX_MODULE_FAIL             *
 *                                                                            *
 ******************************************************************************/
int	zbx_module_init()
{
	int ret = ZBX_MODULE_FAIL;

	/* determine daemon process */
	switch (daemon_type){
		case ZBX_DAEMON_TYPE_SERVER:
			zabbix_log(LOG_LEVEL_WARNING, "[%s] module loaded by server process. [%d]",MODULE_NAME, daemon_type);
			ret = ZBX_MODULE_OK;
			break;
		case ZBX_DAEMON_TYPE_PROXY_ACTIVE:
		case ZBX_DAEMON_TYPE_PROXY_PASSIVE:
		case ZBX_DAEMON_TYPE_PROXY:
			zabbix_log(LOG_LEVEL_WARNING, "[%s] module loaded by proxy process. [%d]", MODULE_NAME, daemon_type);
			ret = ZBX_MODULE_OK;
			break;
		case ZBX_DAEMON_TYPE_AGENT:
			zabbix_log(LOG_LEVEL_WARNING, "[%s] module loaded by agent process. [%d]", MODULE_NAME, daemon_type);
			ret = ZBX_MODULE_OK;
			break;
		default:
			zabbix_log(LOG_LEVEL_WARNING, "[%s] unknown pram daemon_type [%d]", MODULE_NAME, daemon_type );
	}


	// load config file
	zbx_module_load_config();


	return ret;
}

/******************************************************************************
 *                                                                            *
 * Function: zbx_module_uninit                                                *
 *                                                                            *
 * Purpose: the function is called on agent shutdown                          *
 *          It should be used to cleanup used resources if there are any      *
 *                                                                            *
 * Return value: ZBX_MODULE_OK - success                                      *
 *               ZBX_MODULE_FAIL - function failed                            *
 *                                                                            *
 ******************************************************************************/
int	zbx_module_uninit()
{
	return ZBX_MODULE_OK;
}




/*********************************************************************
 * zbx_module_load_config                                            *
 *********************************************************************/
static void     zbx_module_load_config(void)
{

	char MODULE_CONFIG_FILE[256];

	static struct cfg_line  module_cfg[] =
	{
	/* PARAMETER,                   VAR,				TYPE,
                        MANDATORY,      MIN,			MAX */
		{"Param1",              &CONFIG_PARAM1,			TYPE_INT,
                        PARM_OPT,	1,			100},
		{"Param2",              &CONFIG_PARAM2,			TYPE_STRING,
                        PARM_MAND,	0,			0},
		{NULL}
	};


	zbx_snprintf( MODULE_CONFIG_FILE, sizeof(MODULE_CONFIG_FILE), "%s/%s", CONFIG_LOAD_MODULE_PATH, MODULE_CONFIG_FILE_NAME);
	zabbix_log(LOG_LEVEL_WARNING, "[%s] load conf:%s", MODULE_NAME, MODULE_CONFIG_FILE);

	parse_cfg_file(MODULE_CONFIG_FILE, module_cfg, ZBX_CFG_FILE_REQUIRED, ZBX_CFG_STRICT);
	zbx_module_set_defaults();


}


static void     zbx_module_set_defaults(void)
{
        if (NULL == &CONFIG_PARAM1){
                CONFIG_PARAM1 = 1;
	}

        if (NULL == CONFIG_PARAM2){
                CONFIG_PARAM2 = zbx_strdup(CONFIG_PARAM2, "hello");
	}

}


