# numeric/Makefile

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu11

LOCAL_LIBS = ../common
LOCAL_HDRS = ../common

abs-tmin:
	$(CC) $(CFLAGS) -o abs-tmin abs_tmin.c

truncation:
	$(CC) $(CFLAGS) -o truncation truncation.c

common:
	$(MAKE) -C $(LOCAL_LIBS)

clean:
	rm -f *.o
	rm -f abs-tmin
	rm -f truncation
	$(MAKE) -C $(LOCAL_LIBS) clean


.PHONY: clean