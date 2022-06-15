PROGRAM = mem_one
cpp_FILES = main.cpp gene.cpp gene2.cpp
c_FILES =
CC = g++

CFLAGS = -O -g
LIBs = 

OBJECTS = $(c_FILES:.c=.o) $(cpp_FILES:.cpp=.o)

.SUFFIXES: .o .cpp .c

.cpp.o:
	$(CC) $(FLAGS) $(CFLAGS) $< -c $(INCLUDES)

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) $< -c $(INCLUDES)

$(PROGRAM): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) -o $@ $(INCLUDES) $(OBJECTS) $(LIBS)

clean:
	rm *.o

rebuild: clean
	make
