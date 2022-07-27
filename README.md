# Parallel and Serial Maze Generator
 Generates a maze from CI using parallelization

#Limitations:
- Cannot have even sized mazes
- Maze seeds must be 0 or higher
- Maze size must be greater than 0
- Works on Linux OS

#Instructions:
- requires OpenMP to be installed into system for compilation
- program includes compiled files if OpenMP is not available
- type "make" to run the makefile
- type "./maze" to run the serial version
- type "./mazep" to run the parallel version
- add -n {NUMBER} to command line execution to customize maze size
	example:
		./maze -n 21
- maze size is 11 by default
- add -s {NUMBER} to command line execution to customize maze seed
	example:
		./maze -s 12345
- maze seed is 0 by default

#Removing Executables:
- type "make rm"
- WARNING: this will also remove included compiled files

#Remarks:
- Parallelism is more noticeable at larger map sizes