BIN:=tuya-reporterd
LDFLAGS:=-llink_core -lmiddleware_implementation -lplatform_port -lutils_modules -lblobmsg_json -lubox -lubus -largp
CPPFLAGS:=-I$(CURDIR)/include -I$(INSTALL_DIR)/include
SRC:=$(wildcard *.c)
OBJS:=$(SRC:.c=.o)

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(BIN) $(OBJS)
