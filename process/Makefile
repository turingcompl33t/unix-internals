# process/Makefile

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu11

LOCAL_LIBS = ../common
LOCAL_HDRS = ../common

fork1:
	$(CC) $(CFLAGS) -o fork1 fork1.c

fork2:
	$(CC) $(CFLAGS) -o fork2 fork2.c

redirect1:
	$(CC) $(CFLAGS) -o redirect1 redirect1.c

fork-file-sharing: common
	$(CC) $(CFLAGS) -o fork-file-sharing -I$(LOCAL_HDRS) fork_file_sharing.c $(LOCAL_LIBS)/common.a

common:
	$(MAKE) -C $(LOCAL_LIBS)

clean:
	rm -f *.o
	rm -f fork1
	rm -f fork2
	rm -f redirect1
	rm -f fork-file-sharing
	$(MAKE) -C $(LOCAL_LIBS) clean


.PHONY: clean