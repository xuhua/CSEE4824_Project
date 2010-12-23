#Makefile for SESC simulation
#Young Jin Yoon <youngjin@cs.columbia.edu>

#type "make sesc" to generate an executable file for sesc simulator
#type "make gcc" to generate an executable file for linux
#type "make" to generate executable files for both linux and sesc simulator 
#type "make clean" to clean up the directory

CC =../utils/bin/mipseb-linux-gcc
CFLAGS = -g -mips2 -mabi=32 -Wa,-non_shared -mno-abicalls -DSESC -I../utils/lib/gcc/mipseb-linux/3.4.4/include -I../utils/mipseb-linux/sys-include -I../utils/mipseb-linux/include -L../utils/lib/gcc/mipseb-linux/3.4.4 -nostdinc
CLINKS = -static -Wl,--script=../utils/mipseb-linux/lib/ldscripts/mint.x
TARGET = programy
TARGET2 = matmul
EXEC = $(TARGET).sesc
SRCS = $(TARGET).c 
THREAD = sesc_thread
THREAD_OBJS = $(THREAD).o
THREAD_SRCS = $(THREAD).c
FAST = sesc_events
FAST_OBJS = $(FAST).o
FAST_SRCS = $(FAST).c
TARGET2_OBJS = $(TARGET2).o
TARGET2_SRCS = $(TARGET2).c

all: sesc gcc
sesc: 
	$(CC) -c -o $(THREAD_OBJS) $(CFLAGS) $(THREAD_SRCS) 
	$(CC) -c -o $(FAST_OBJS) $(CFLAGS) $(FAST_SRCS) 
	$(CC) -c -o $(TARGET2_OBJS) $(CFLAGS) $(TARGET2_SRCS) 
	$(CC) -o $(EXEC) $(CFLAGS) $(CLINKS) $(SRCS) $(THREAD_OBJS) $(FAST_OBJS) $(TARGET2_OBJS)

gcc:
	gcc -c -o $(TARGET2_OBJS) $(TARGET2_SRCS)
	gcc -o $(TARGET) $(SRCS) $(TARGET2_OBJS)

clean:
	rm -rf *.o
	rm -rf *.sesc
	rm -rf $(TARGET)
