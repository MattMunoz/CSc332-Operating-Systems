// include directive for ceratin function usage
#include <string.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <stdio.h>
#include <stdlib.h>

// function deficntion
int search(char* file, char* dir);

int main(int argc, char* argv[]){

	if (argc != 3){
		perror("Wrong number of arguments...exiting program");
		return -1;
	}

	// function to check for file existance in a ceratin directory
	int exists = search(argv[1], argv[2]);

	
	return 0;
}

int search(char* file, char* dir){
	
	// opens a directory and returns a pointer to said directory if it exists
	DIR *dirptr = opendir(dir);
	
	// if the directory does not exist it prints an appropriate message and exits the program
	if (dirptr == NULL) {
		perror("The directory called does NOT exist");
		exit (1);
	}
	
	// allows for access of each file in the directory via a struct
	struct dirent *entry = readdir(dirptr);
	
	// loops through the directory checking each file until 
	// the file is found or it reaches the end of the directory
	while (entry != NULL) {
	
		// checks if the current file being looked at is the one we are searching for 
		// and prints the appropriate message, then the funciton call returns to main
		if (strlen(entry->d_name) == strlen(file) && strcmp(entry->d_name, file) == 0) {
			printf("\n\"%s\" does exisit in the \"%s\" directory\n", file, dir);
			return 0;
		}
			
		entry = readdir(dirptr);
	}

	// if the file is not found a messagfe is displayed and the function returns
	printf("\n\"%s\" does NOT exisit in the \"%s\" directory\n", file, dir);
	return 1;
}

