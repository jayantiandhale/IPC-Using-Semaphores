CS 570 - Assignment 1, Due by Date: 09/22/2014
Assignment solution by-
Student Id: 12022254
Name: Jayanti Andhale
-----------------------------------------------------------------------------------------------------------------------------------------
Summary:
1. The proposed solution to the problem makes use of the shared memory operations to Load, retrieve and update data to/from shared memory.
2. This assignment helped me resolve the concepts for interprocess communication through shared memory and synchronization using semaphores.
3. Programs implemented in the solution use following system calls for accessing shared memmory
	-shmget, shmctl, shmat, semget, semctl, semop
4. Following files are included as part of the assignment solution:
	-Load.c, Change.c, Print.c, Query.c, Clean.c
	-Makefile
	-README file (this file)
5. Assumptions before running the program files:
	-A user will not run Print, Query or Change before the Load program is executed
	or after Clean program has been executed
	-At most 20 entries will be in the database at any given time
 	-The password for an advisor is "000"
	-Input file name is "Student_info.dat" and output file name is "Saved_Student_Info.dat" 
-------------------------------------------------------------------------------------------------------------------------------------------

Instructions to Compile and to Run the program files:
1. All compilation rules are in Make file, so run Makefile ->
	make -f Makefile
2. Run Load to load data from input file to shared memory
	./Load
3. Run Print to print data from shared memory to standard ouptut
	./Print
4. Run Query to print information for specific student
	./Query
5. Run Change to change the information for specific student
	./Change
6. Run Clean to save shared memory data to output file and delete shared memory segment and semaphores
	./Clean
7. Run Makefile->CleanAll title to clean all object files,etc from folder.
	make -f Makefile CleanAll
--------------------------------------------------------------------------------------------------------------------------------------------
