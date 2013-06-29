TARGET = libtest.so
CC = gcc
CFLAGS=-I../../ -g -Wall -std=c++11

.PHONY: default all clean

default: $(TARGET)
all: default

H = ../../test.hpp
OBJS = simple.o simple2.o 

ODIR=obj

%.o: ../%.cpp $(H)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -Wall -shared -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
