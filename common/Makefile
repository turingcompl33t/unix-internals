# common/Makeile

LIB = common.a
OBJS = error.o

common: $(OBJS)
	ar cr common.a $(OBJS)

error.o: error.c error.h

clean:
	rm -f *.o
	rm -f common.a

.PHONY: clean