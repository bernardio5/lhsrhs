
TARGET = lhsrhs
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
CC   = g++
CFLAGS = -Wall -D__LITTLE_ENDIAN__ -D__LINUX_ALSA__
CINC = -Iinclude/stk
LIBS = -lncurses -lstk -lpthread -lasound

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(CINC) -o $@ $^ $(LIBS)

clean:
	rm src/*.o
	