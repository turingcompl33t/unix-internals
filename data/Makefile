# data/Makefile

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu11

LOCAL_LIBS = ../common
LOCAL_HDRS = ../common

local-static1:
	$(CC) $(CFLAGS) -o local-static1 -I$(LOCAL_HDRS) local_static1.c

local-static2:
	$(CC) $(CFLAGS) -pthread -o local-static2 -I$(LOCAL_HDRS) local_static2.c

clean:
	rm -f *.o
	rm -f local-static1
	rm -f local-static2

.PHONY: clean