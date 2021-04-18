# perf/Makefile

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu11

LOCAL_LIBS = ../common
LOCAL_HDRS = ../common

hello:
	$(CC) $(CFLAGS) -o hello hello.c

common:
	$(MAKE) -C $(LOCAL_LIBS)

clean:
	rm -f *.o
	rm -f hello
	$(MAKE) -C $(LOCAL_LIBS) clean


.PHONY: clean