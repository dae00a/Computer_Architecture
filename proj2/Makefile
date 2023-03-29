CC=gcc
CFLAGS=-Wall
OBJS=main.o if.o id.o ex.o mem.o wb.o
TARGET=singleCycleMIPS

$(TARGET): $(OBJS)
	$(CC) -Wall -Wextra -o $@ $(OBJS)

main.o: Resource.h main.c
if.o: if.h if.c
id.o: id.h id.c
ex.o: ex.h ex.c
mem.o: mem.h mem.c
wb.o: wb.h wb.c

clean:
	rm -f $(OBJS) $(TARGET)