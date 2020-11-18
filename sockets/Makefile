# sockets/Makefile

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu11

LOCAL_LIBS = ../common
LOCAL_HDRS = ../common

unix-stream-client: common
	$(CC) $(CFLAGS) -o unix-stream-client -I$(LOCAL_HDRS) unix_stream_client.c $(LOCAL_LIBS)/common.a

unix-stream-server: common
	$(CC) $(CFLAGS) -o unix-stream-server -I$(LOCAL_HDRS) unix_stream_server.c $(LOCAL_LIBS)/common.a

common:
	$(MAKE) -C $(LOCAL_LIBS)

clean:
	rm -f *.o
	rm -f unix-stream-client
	rm -f unix-stream-server
	$(MAKE) -C $(LOCAL_LIBS) clean


.PHONY: clean