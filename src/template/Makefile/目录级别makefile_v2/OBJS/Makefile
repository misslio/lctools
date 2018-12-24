
include $(SRC_BASE)/makefile.rule

CUR_OBJS=$(wildcard *.o)


$(EXE):$(CUR_OBJS) 
	$(CC) $^ -o ../$@  $(CFLAGS) $(LFLAGS) $(IFLAGS)

.PHONY:clean 
clean:
	-rm -rf $(CUR_OBJS) 


