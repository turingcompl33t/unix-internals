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

preforking: common
	$(CC) $(CFLAGS) -o preforking -I$(LOCAL_HDRS) preforking.c util.c $(LOCAL_LIBS)/common.a

threaded: common
	$(CC) $(CFLAGS) -o threaded -pthread -I$(LOCAL_HDRS) threaded.c util.c $(LOCAL_LIBS)/common.a

prethreaded1: common
	$(CC) $(CFLAGS) -o prethreaded1 -pthread -I$(LOCAL_HDRS) prethreaded1.c util.c $(LOCAL_LIBS)/common.a

common:
	$(MAKE) -C $(LOCAL_LIBS)

clean:
	rm -f *.o
	rm -f client
	rm -f iterative
	rm -f forking
	rm -f preforking
	rm -f threaded
	rm -f prethreaded1
	$(MAKE) -C $(LOCAL_LIBS) clean

.PHONY: clean