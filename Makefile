LIB_DIR=./lib
BIN_DIR=.

INCLUDE = -I$(LIB_DIR)/mysqlclient/include \
		-I$(LIB_DIR)/replication/include

LIBS = $(LIB_DIR)/replication/libreplication.a \
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

$(BIN_DIR)/binlog: main.o Handler_IncidentEvent.o Handler_TableMapEvent.o Handler_RowEvent.o
	$(GXX) -o $@ $^ $(CFLAGS) $(LIBS) $(LDLIBS)

clean:
	rm -f *.o
