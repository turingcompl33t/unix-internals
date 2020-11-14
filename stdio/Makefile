# stdio/Makefile

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu11

LOCAL_LIBS = ../common
LOCAL_HDRS = ../common

buffering1:
	$(CC) $(CFLAGS) -o buffering1 buffering1.c

buffering2:
	$(CC) $(CFLAGS) -o buffering2 buffering2.c

buffering3: common
	$(CC) $(CFLAGS) -o buffering3 -I$(LOCAL_HDRS) buffering3.c $(LOCAL_LIBS)/common.a

buffering4: common
	$(CC) $(CFLAGS) -o buffering4 -I$(LOCAL_HDRS) buffering4.c $(LOCAL_LIBS)/common.a

memstream1:
	$(CC) $(CFLAGS) -o memstream1 memstream1.c

common:
	$(MAKE) -C $(LOCAL_LIBS)

clean:
	rm -f *.o
	rm -f memstream1
	rm -f buffering1
	rm -f buffering2
	rm -f buffering3
	rm -f buffering4
	$(MAKE) -C $(LOCAL_LIBS) clean


.PHONY: clean