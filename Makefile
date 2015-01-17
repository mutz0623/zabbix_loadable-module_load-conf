
SRC = dummy_load-conf.c
OBJ = $(SRC:%.c=%.so)

$(OBJ): $(SRC)
	gcc -shared -o $(OBJ) $(SRC) -I../zabbix-src/include/ -fPIC

clean:
	rm -f $(OBJ)

install: $(OBJ)
	service zabbix-agent stop
	install $(OBJ) /etc/zabbix/modules/
	service zabbix-agent start

