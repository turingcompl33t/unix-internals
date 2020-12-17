# nonlocal/Makefile

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu11

LOCAL_LIBS = ../common
LOCAL_HDRS = ../common

nonlocal1: common
	$(CC) $(CFLAGS) -o nonlocal1 -I$(LOCAL_HDRS) nonlocal1.c $(LOCAL_LIBS)/common.a

common:
	$(MAKE) -C $(LOCAL_LIBS)

clean:
	rm -f *.o
	rm -f nonlocal1
	$(MAKE) -C $(LOCAL_LIBS) clean


.PHONY: clean