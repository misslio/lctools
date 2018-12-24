
EXE = db_test

CC = gcc
LFLAGS = -lpcap -liridiumlog -lpthread -lmysqlclient
LFLAGS +=  -L/usr/lib -lm -lglib-2.0 -liridiumlog
CFLAGS = -g -Wall 

BIN_DIR = ./lc_bin

CUR_PATH=$(shell 'pwd')
#SUB_DIRS =  `ls -d */ | grep -v 'bin' | grep -v 'objs'`
ALL_SUB_DIRS =  $(shell find . -type d | grep -v 'lc_bin')
SUB_DIRS = $(basename $(patsubst ./%,%, $(ALL_SUB_DIRS)))

SRCS = $(foreach SRC,$(SUB_DIRS), $(wildcard $(CUR_PATH)/$(SRC)/*.c))
OBJS = $(patsubst %.c,%.o,$(SRCS)) 
	

MAKE_SUBDIR = @for subdir in $(SUB_DIRS);\
	do\
	echo "making in $$subdir";\
	(cd $$subdir && make -f Makefile.mk clean;make -f Makefile.mk) || exit 1;\
	done;

all:$(OBJS) $(EXE)
.PHONY:all

$(OBJS):$(SRCS)
	$(MAKE_SUBDIR)

$(EXE):$(OBJS)
	$(CC) $(CFLGAS) -o $@ $^ $(LFLAGS)
	cp $(EXE) $(BIN_DIR)



.PHONY:clean 
clean:
	-rm -rf $(OBJS) $(EXE)	$(BIN_DIR)/$(EXE)
