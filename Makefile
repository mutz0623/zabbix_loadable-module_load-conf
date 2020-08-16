CC = gcc
CFLAG = -MMD -MP -Wall -Wextra -O3 -fPIC
LDFLAG = -shared
INCLUDE = -I../zabbix-src/include
SRCDIR = ./src
SRC = $(wildcard $(SRCDIR)/*.c)
OBJDIR = ./obj
OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRC:.c=.o)))
BINDIR = ./bin
BIN = dummy_load-conf.so
TARGET = $(BINDIR)/$(BIN)
DEPENDS = $(OBJ:.o=.d)
MODULEPATH = /etc/zabbix/modules
MODULECONF = dummy_load-conf.conf
CONFDIR = ./conf

$(TARGET): $(OBJ)
	-mkdir -p $(BINDIR)
	$(CC) $(LDFLAG) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	-mkdir -p $(OBJDIR)
	$(CC) $(CFLAG) $(INCLUDE) -o $@ -c $<

-include $(DEPENDS)

.PHONY: all clean
all: clean $(TARGET)

clean:
	-rm -f $(TARGET) $(OBJ) $(DEPENDS)

.PHONY: install test
install:$(TARGET)
	test $$(whoami) == "root"
	[ -d $(MODULEPATH) ] || mkdir $(MODULEPATH)
	install -C $(TARGET) $(MODULEPATH)
	ls $(MODULEPATH)/$(MODULECONF) || cp $(CONFIDR)/$(MODULECONF) $(MODULEPATH)
	service zabbix-agent restart
	service zabbix-agent status
	tail /var/log/zabbix/zabbix_agentd.log

test:
	md5sum  $(MODULEPATH)/$(TARGET) ./$(TARGET) || :
	zabbix_agentd --print |grep -e dummy.param1 -e dummy.param2 -e dummy.LLD

.PHONY :doc doc-clean
doc:
	doxygen ./doc/Doxyfile

doc-clean:
	-rm -rf ./doc/html/

.PHONY : scan-build scan-build-clean
scan-build:clean
	-mkdir -p ./doc/scan-build/
	scan-build -o ./doc/scan-build/ $(MAKE)

scan-build-clean:
	-rm -rf ./doc/scan-build/

.PHONY: clean-all
clean-all: clean doc-clean scan-build-clean

