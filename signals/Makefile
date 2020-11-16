# signals/Makefile

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu11

LOCAL_LIBS = ../common
LOCAL_HDRS = ../common

stop1: common spin1
	$(CC) $(CFLAGS) -o stop1 -I$(LOCAL_HDRS) stop1.c $(LOCAL_LIBS)/common.a

spin1: common
	$(CC) $(CFLAGS) -o spin1 -I$(LOCAL_HDRS) spin1.c $(LOCAL_LIBS)/common.a

pending1: common
	$(CC) $(CFLAGS) -o pending1 -I$(LOCAL_HDRS) pending1.c $(LOCAL_LIBS)/common.a

suspend1: common
	$(CC) $(CFLAGS) -o suspend1 -I$(LOCAL_HDRS) suspend1.c $(LOCAL_LIBS)/common.a

signalfd1: common
	$(CC) $(CFLAGS) -o signalfd1 -I$(LOCAL_HDRS) signalfd1.c $(LOCAL_LIBS)/common.a

signalfd2: common
	$(CC) $(CFLAGS) -o signalfd2 -I$(LOCAL_HDRS) signalfd2.c $(LOCAL_LIBS)/common.a

signalfd3: common
	$(CC) $(CFLAGS) -o signalfd3 -I$(LOCAL_HDRS) signalfd3.c $(LOCAL_LIBS)/common.a

signalfd4: common
	$(CC) $(CFLAGS) -o signalfd4 -I$(LOCAL_HDRS) signalfd4.c $(LOCAL_LIBS)/common.a

common:
	$(MAKE) -C $(LOCAL_LIBS)

clean:
	rm -f *.o
	rm -f stop1
	rm -f spin1
	rm -f pending1
	rm -f suspend1
	rm -f signalfd1
	rm -f signalfd2
	rm -f signalfd3
	rm -f signalfd4
	$(MAKE) -C $(LOCAL_LIBS) clean


.PHONY: clean