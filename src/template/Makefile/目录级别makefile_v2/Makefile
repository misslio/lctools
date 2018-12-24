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

ifeq ($(MARIADB), TRUE)
ALL_SUB_DIRS = $(shell find . -maxdepth 1 -type d | grep -v 'OBJS')
else
ALL_SUB_DIRS = $(shell find . -maxdepth 1 -type d | grep -v 'OBJS' | grep -v 'restore_data')
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

all : ALL_SUB_OBJS $(CUR_OBJS) $(EXE)

$(EXE):$(CUR_OBJS) ALL_SUB_OBJS 
	make -C OBJS

ALL_SUB_OBJS:
	$(MAKE_SUBDIR)

$(CUR_OBJS):%.o:%.c	
#$(MAKE_SUBDIR)
	$(CC)  $(CFLAGS) $(LFLAGS) $(IFLAGS) -c $< -o $@
	cp -r $@ $(ALL_OBJS_DIR) 

.PHONY:clean 
clean:
	@-rm -rf $(CUR_OBJS)
	$(MAKE_SUBDIR_CLEAN)
	make -C OBJS clean
	@rm -rf core.*


