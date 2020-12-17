# sync/Makefile

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu11

LOCAL_LIBS = ../common
LOCAL_HDRS = ../common

sem1: common
	$(CC) $(CFLAGS) -pthread -o sem1 -I$(LOCAL_HDRS) sem1.c $(LOCAL_LIBS)/common.a

common:
	$(MAKE) -C $(LOCAL_LIBS)

clean:
	rm -f *.o
	rm -f sem1
	$(MAKE) -C $(LOCAL_LIBS) clean


.PHONY: clean