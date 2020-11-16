# async-io/Makefile

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu11

LOCAL_LIBS = ../common
LOCAL_HDRS = ../common

epoll1: common
	$(CC) $(CFLAGS) -pthread -o epoll1 -I$(LOCAL_HDRS) epoll1.c $(LOCAL_LIBS)/common.a

epoll2: common
	$(CC) $(CFLAGS) -o epoll2 -I$(LOCAL_HDRS) epoll2.c $(LOCAL_LIBS)/common.a

eventfd: common
	$(CC) $(CFLAGS) -o eventfd -I$(LOCAL_HDRS) eventfd.c $(LOCAL_LIBS)/common.a

linux-aio: common
	$(CC) $(CFLAGS) -o linux-aio -I$(LOCAL_HDRS) linux_aio.c $(LOCAL_LIBS)/common.a

common:
	$(MAKE) -C $(LOCAL_LIBS)

clean:
	rm -f *.o
	rm -f epoll1
	rm -f epoll2
	rm -f eventfd
	rm -f linux-aio
	$(MAKE) -C $(LOCAL_LIBS) clean


.PHONY: clean