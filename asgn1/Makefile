
#httpserver
#Took the template from my cse101 class makefile: https://github.com/jakearmendariz/cse101
#Jake Armendariz
#Makefile-- creates an executable: httpserver
SOURCES	 = server.c	requests.c	library.c
OBJECTS	 = server.o	requests.o	library.o
HEADERS	 = server.h
EXEBIN	 = httpserver
FLAGS	 = -std=c99	-Wall	-Wextra	-Wpedantic	-Wshadow	-lpthread

all: $(EXEBIN)

$(EXEBIN) : $(OBJECTS) $(HEADERS)
	gcc -o $(EXEBIN) $(OBJECTS)

$(OBJECTS) : $(SOURCES) $(HEADERS)
	gcc -c $(FLAGS) $(SOURCES)

clean :
	rm -f $(EXEBIN) $(OBJECTS)

check:
	valgrind --leak-check=full $(EXEBIN)
