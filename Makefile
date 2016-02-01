#all title to indicate all main files in a program
all: Load Print Change Query Clean

#Format for writing make
#first line: title: dependent files
#second line: after pressing tab, rule for compiling file in title
Load: Load.c sem.h
	gcc -o Load Load.c sem.c
Print: Print.c sem.h
	gcc -o Print Print.c sem.c
Change: Change.c sem.h
	gcc -o Change Change.c sem.c
Query: Query.c sem.h
	gcc -o Query Query.c sem.c
Clean: Clean.c sem.h
	gcc -o Clean Clean.c sem.c
CleanAll: 
	rm -f Load Print Change Query Clean *~core
