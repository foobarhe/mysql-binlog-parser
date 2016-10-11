LIB_DIR=./lib
BIN_DIR=.

INCLUDE = -I$(LIB_DIR)/mysqlclient/include \
		-I$(LIB_DIR)/mysql-binary-log-events-1.0.2/mysqlstream/include \
		-I$(LIB_DIR)/mysql-binary-log-events-1.0.2/binlogevents/include
LIBS = $(LIB_DIR)/mysql-binary-log-events-1.0.2/mysqlstream/lib/libmysqlstream.a \
		$(LIB_DIR)/mysql-binary-log-events-1.0.2/binlogevents/lib/libbinlogevents.a \
		$(LIB_DIR)/mysqlclient/libmysqlclient.a

LDLIBS = -lpthread -lz -ldl

GCC = gcc
GXX = g++

CFLAGS = -std=c++11 -g -DNDEBUG $(INCLUDE)

C_SOURCE = $(wildcard *.c)
LIB_OBJS = $(patsubst %.c, %.o,$(C_SOURCE))
CPP_SOURCE = $(wildcard *.cpp)
LIB_OBJS += $(patsubst %.cpp, %.o,$(CPP_SOURCE))

.cpp.o:
	$(GXX) $(CFLAGS) -Wno-strict-aliasing -c $<
.c.o:
	$(GCC) $(CFLAGS) -c $<

all: clean $(BIN_DIR)/binlog

$(BIN_DIR)/binlog: main.o
	$(GXX) -o $@ $^ $(CFLAGS) $(LIBS) $(LDLIBS)

clean:
	rm -f *.o
