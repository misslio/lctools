.PHONY: all

#CC = gcc
#CFLAGS = -g -Wall `pkg-config --libs --cflags glib-2.0`


#export ROOT_DIR = $(shell pwd)

export SRC_BASE = $(shell 'pwd')

include $(SRC_BASE)/makefile.rule


export ALL_OBJS_DIR = $(shell 'pwd')/OBJS

EXE = trust

CUR_PATH=$(shell 'pwd')
CUR_SRC=$(wildcard $(CUR_PATH)/*.c)
CUR_OBJS=$(patsubst %.c,%.o,$(CUR_SRC))

ifeq ($(restore), TRUE)
ALL_SUB_DIRS = $(shell find . -type d | grep -v 'OBJS' | grep -v 'read_pcap_file')
else
ALL_SUB_DIRS = $(shell find . -type d | grep -v 'OBJS' | grep -v 'restore_data')
endif
SUB_DIRS = $(basename $(patsubst ./%,%, $(ALL_SUB_DIRS)))


MAKE_SUBDIR = @for subdir in $(SUB_DIRS);\
	do\
	echo "making in $$subdir";\
	(cd $$subdir && make -f Makefile.mk) || exit 1;\
	done;


MAKE_SUBDIR_CLEAN = @for subdir in $(SUB_DIRS);\
	do\
	echo "making in $$subdir";\
	(cd $$subdir && make -f Makefile.mk clean) || exit 1;\
	done;


$(EXE):$(CUR_OBJS) $(wildcard $(ALL_OBJS_DIR)/*.o)
	$(MAKE_SUBDIR)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^
#all:$(CUR_OBJS) $(ALL_OBJS) 
#	$(MAKE_SUBDIR)
#	@echo "Finish in $(CUR_PATH)"


$(CUR_OBJS):%.o:%.c	
#$(MAKE_SUBDIR)
	$(CC)  $(CFLAGS) $(LFLAGS) $(IFLAGS) -c $< -o $@


.PHONY:clean 
clean:
	-rm -rf $(CUR_OBJS) 
	$(MAKE_SUBDIR_CLEAN)


