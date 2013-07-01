TARGET = libtest.so
CC = g++
CFLAGS=-I../../ -fPIC -g -Wall -std=c++11

.PHONY: default all clean

default: $(TARGET)
all: default

H = ../../test.hpp
OBJS = simple.o simple2.o asserts.o

ODIR=obj

%.o: ../%.cpp $(H)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -fPIC -Wall -shared -dynamiclib -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
