
CC  = gcc
CFLAGS = -Wall -pthread

# no names here
SRCS = 
OBJS = $(SRCS:.c=.o)
TARGET = 

all:  $(TARGET)

$(TARGET):  $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o  $(TARGET)

%.o:  %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
