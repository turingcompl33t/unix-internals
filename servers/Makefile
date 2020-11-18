# servers/Makefile

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu11

LOCAL_LIBS = ../common
LOCAL_HDRS = ../common

client: common
	$(CC) $(CFLAGS) -o client -I$(LOCAL_HDRS) client.c util.c $(LOCAL_LIBS)/common.a

iterative: common
	$(CC) $(CFLAGS) -o iterative -I$(LOCAL_HDRS) iterative.c util.c $(LOCAL_LIBS)/common.a

forking: common
	$(CC) $(CFLAGS) -o forking -I$(LOCAL_HDRS) forking.c util.c $(LOCAL_LIBS)/common.a

common:
	$(MAKE) -C $(LOCAL_LIBS)

clean:
	rm -f *.o
	rm -f client
	rm -f iterative
	rm -f forking
	$(MAKE) -C $(LOCAL_LIBS) clean


.PHONY: clean