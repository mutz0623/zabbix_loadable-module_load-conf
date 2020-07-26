#ifndef __ZABBIX_CONFIG_LOAD_H
#define __ZABBIX_CONFIG_LOAD_H


#include "sysinc.h"
#include "module.h"
#include "common.h"
#include "log.h"
#include "cfg.h"

#define MODULE_NAME "dummy_load-conf.so"
#define MODULE_CONFIG_FILE_NAME "dummy_load-conf.conf"

extern char *CONFIG_LOAD_MODULE_PATH ;


extern void zbx_module_load_config(void);
extern void zbx_module_set_defaults(void);

extern int CONFIG_PARAM1;
extern char *CONFIG_PARAM2;

#endif /* __ZABBIX_CONFIG_LOAD_H */

