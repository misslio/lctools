.PHONY: all

#CC = gcc
#CFLAGS = -g -Wall `pkg-config --libs --cflags glib-2.0`

#SRC_BASE=../

include $(SRC_BASE)/makefile.rule

CUR_PATH=$(shell 'pwd')
CUR_SRC=$(wildcard $(CUR_PATH)/*.c)
CUR_OBJS=$(patsubst %.c,%.o,$(CUR_SRC))


ALL_SUB_DIRS = $(shell find . -maxdepth 1 -type d)
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

all:$(CUR_OBJS) 
	$(MAKE_SUBDIR)
	@echo "Finish in $(CUR_PATH)"

$(CUR_OBJS):%.o:%.c
	$(CC)  $(CFLAGS) $(LFLAGS) $(IFLAGS) -c $< -o $@ 
	cp -f $@ $(ALL_OBJS_DIR)

.PHONY:clean 
clean:
	-rm -rf $(CUR_OBJS) 
	$(MAKE_SUBDIR_CLEAN)


