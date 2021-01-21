CC=gcc
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -pthreads

# the build target executable:
# SOURCES = $(wildcard *.c)
# OBJECTS = $(SOURCES:%.c=%.o)

# all: mm

# %.o: %.c
# 	$(CC) $(CFLAGS) -c $^ -o $@ -g

# mm:$(OBJECTS)
# 	$(CC) -o mm $(OBJECTS)

mm: matrix_multiplication.c lab1_IO.c
	$(CC) $(CFLAGS) -o mm matrix_multiplication.c lab1_IO.c -I

clean:
	@rm -f *.o mm