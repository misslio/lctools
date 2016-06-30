.PHONY: all

CC = gcc
LFLAGS = -lssl -lcrypto -lpthread
CFLAGS = -g 

CUR_PATH=$(shell 'pwd')
CUR_SRC=$(wildcard $(CUR_PATH)/*.c)
CUR_OBJS=$(patsubst %.c,%.o,$(CUR_SRC))

all:$(CUR_OBJS) 
	@echo "Finish in $(CUR_PATH)"

$(CUR_OBJS):%.o:%.c
	$(CC)  -c $< -o $@ $(CFLAGS) $(LFLAGS)



.PHONY:clean 
clean:
	-rm -rf $(CUR_OBJS) 
