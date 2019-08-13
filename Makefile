
CC = gcc
CFLAG = -Wall -Wextra -O3
SRC = dummy_load-conf.c config_load.c
HDR = config_load.h
OBJ = $(SRC:%.c=%.o)
TARGET = dummy_load-conf.so
MODULEPATH = /etc/zabbix/modules

.SUFFIXES: .c .o

$(TARGET): $(OBJ)
	$(CC) -shared -o $@ $(OBJ) -fPIC

$(OBJ): $(HDR)

.c.o:
	$(CC) -c $< -I../zabbix-src/include/ -fPIC -c $(CFLAG)

clean:
	rm -f $(TARGET) $(OBJ)

install:$(TARGET)
	install -C $(TARGET) $(MODULEPATH)

restart:$(TARGET)
	service zabbix-agent restart
	service zabbix-agent status
	tail /var/log/zabbix/zabbix_agentd.log

test:
	md5sum  $(MODULEPATH)/$(TARGET) ./$(TARGET) || :
	zabbix_agentd --print |grep -e dummy.param1 -e dummy.param2 -e dummy.LLD


