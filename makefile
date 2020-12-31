CC     = gcc
CFLAGS = -Wall -g -std=c99
CCLINK =$(CC)
OBJS   = ransom.o
EXEC   = prog.exe
RM     = rm -f

$(EXEC): $(OBJS)
	$(CCLINK) $(OBJS) -o $(EXEC)

ransom.o: ransom.c
  
clean:
	$(RM) $(EXEC) $(OBJS)