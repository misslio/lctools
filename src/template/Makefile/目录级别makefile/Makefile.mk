.PHONY: all

CC = gcc
#LFLAGS =  -L/usr/lib -lm -lglib-2.0 -liridiumlog
IFLAGS = -I/usr/include/glib-2.0 -I/usr/lib64/glib/include `pkg-config --cflags --libs glib-2.0`
#IFLAGS =  `pkg-config --clfags --libs glib-2.0`
CFLAGS = -g -Wall

CUR_PATH=$(shell 'pwd')
CUR_SRC=$(wildcard $(CUR_PATH)/*.c)
CUR_OBJS=$(patsubst %.c,%.o,$(CUR_SRC))

all:$(CUR_OBJS) 
	@echo "Finish in $(CUR_PATH)"


	
$(CUR_OBJS):%.o:%.c
	$(CC)  $(CFLAGS) $(LFLAGS) $(IFLAGS) -c $< -o $@ 


.PHONY:clean 
clean:
	-rm -rf $(CUR_OBJS) 
