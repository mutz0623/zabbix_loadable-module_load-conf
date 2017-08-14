
CC = gcc
CFLAG = -Wall -O3
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
	service zabbix-agent stop
	install -C $(TARGET) $(MODULEPATH)
	service zabbix-agent start
	service zabbix-agent status
	tail /var/log/zabbix/zabbix_agentd.log

test:
	md5sum  $(MODULEPATH)/$(TARGET) ./$(TARGET) || :
	zabbix_agentd --test  dummy.param1
	zabbix_agentd --test  dummy.param2


