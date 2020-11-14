# io/Makefile

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu11

LOCAL_LIBS = ../common
LOCAL_HDRS = ../common

dup1: common
	$(CC) $(CFLAGS) -o dup1 -I$(LOCAL_HDRS) dup1.c $(LOCAL_LIBS)/common.a

share1: common
	$(CC) $(CFLAGS) -o share1 -I$(LOCAL_HDRS) share1.c $(LOCAL_LIBS)/common.a

hole: common
	$(CC) $(CFLAGS) -o hole -I$(LOCAL_HDRS) hole.c $(LOCAL_LIBS)/common.a

common:
	$(MAKE) -C $(LOCAL_LIBS)

clean:
	rm -f *.o
	rm -f dup1
	rm -f share1
	rm -f hole
	$(MAKE) -C $(LOCAL_LIBS) clean


.PHONY: clean