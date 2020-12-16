# io/Makefile

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu11

LOCAL_LIBS = ../common
LOCAL_HDRS = ../common

dup1: common
	$(CC) $(CFLAGS) -o dup1 -I$(LOCAL_HDRS) dup1.c $(LOCAL_LIBS)/common.a

dup2: common
	$(CC) $(CFLAGS) -o dup2 -I$(LOCAL_HDRS) dup2.c $(LOCAL_LIBS)/common.a

share1: common
	$(CC) $(CFLAGS) -o share1 -I$(LOCAL_HDRS) share1.c $(LOCAL_LIBS)/common.a

open1: common
	$(CC) $(CFLAGS) -o open1 -I$(LOCAL_HDRS) open1.c $(LOCAL_LIBS)/common.a

open2: common
	$(CC) $(CFLAGS) -o open2 -I$(LOCAL_HDRS) open2.c $(LOCAL_LIBS)/common.a

open3: common
	$(CC) $(CFLAGS) -o open3 -I$(LOCAL_HDRS) open3.c $(LOCAL_LIBS)/common.a

open4: common
	$(CC) $(CFLAGS) -o open4 -I$(LOCAL_HDRS) open4.c $(LOCAL_LIBS)/common.a

hole: common
	$(CC) $(CFLAGS) -o hole -I$(LOCAL_HDRS) hole.c $(LOCAL_LIBS)/common.a

common:
	$(MAKE) -C $(LOCAL_LIBS)

clean:
	rm -f *.o
	rm -f dup1
	rm -f dup2
	rm -f share1
	rm -f open1
	rm -f open2
	rm -f open3
	rm -f open4
	rm -f hole
	$(MAKE) -C $(LOCAL_LIBS) clean


.PHONY: clean