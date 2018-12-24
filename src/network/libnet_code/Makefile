#Makefile for lc 2016-03-27

EXE=test
CC=gcc

CFLAGS += -g
LFLAGS += -lnet

CUR_PATH = $(shell pwd)

CUR_SRC = $(wildcard $(CUR_PATH)/*.c)

OBJS = $(patsubst %.c,%.o,$(CUR_SRC))

$(EXE):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

$(OBJS):%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LFLAGS)

.PHONY:clean

clean:
	-rm -rf $(OBJS) $(EXE)




