TARGET = libtest.so
CC = g++
CFLAGS=-I../../ -fPIC -g -Wall -std=c++1y

.PHONY: default all clean

default: $(TARGET)
all: default

H = ../../pintest.hpp
OBJS = asserts.o fixtures.o exceptions.o

ODIR=obj

%.o: ../%.cpp $(H)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -fPIC -Wall -shared -dynamiclib -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
