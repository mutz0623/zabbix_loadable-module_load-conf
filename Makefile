
SRC = dummy_load-conf.c
OBJ = $(SRC:%.c=%.so)

dummy-agent.so: $(SRC)
	gcc -shared -o dummy_load-conf.so $(SRC) -I../zabbix-src/include/ -fPIC
#	gcc -shared -o dummy_load-conf.so $(SRC) -I../zabbix-src/include/ -fPIC

clean:
	rm -f dummy_load-conf.so

install:dummy_load-conf.so
	service zabbix-agent stop
	\cp -p dummy_load-conf.so /etc/zabbix/modules/
	service zabbix-agent start

