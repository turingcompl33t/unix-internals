# signals/Makefile

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu11

LOCAL_LIBS = ../common
LOCAL_HDRS = ../common

signalfd1: common
	$(CC) $(CFLAGS) -o signalfd1 -I$(LOCAL_HDRS) signalfd1.c $(LOCAL_LIBS)/common.a

signalfd2: common
	$(CC) $(CFLAGS) -o signalfd2 -I$(LOCAL_HDRS) signalfd2.c $(LOCAL_LIBS)/common.a

common:
	$(MAKE) -C $(LOCAL_LIBS)

clean:
	rm -f *.o
	rm -f signalfd1
	rm -f signalfd2
	$(MAKE) -C $(LOCAL_LIBS) clean


.PHONY: clean