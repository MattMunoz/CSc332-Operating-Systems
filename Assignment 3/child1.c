// include directive to allow for ceratin necessary function calls
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

// function definitions
int exist(int s, int d, char* argv[]);
void transfer(int s, int d);

int main(int argc, char* argv[]){
		
	// opens the files necessary for the data transfer
	int fds = open(argv[1], O_RDONLY);
	int fdd = open(argv[2], O_WRONLY);
	
	// check esistence of the files and exits if one does not exist
	if (exist(fds, fdd, argv) == -5)
		return -2;
		
	// transfers the contents of one file to the other
	transfer(fds, fdd);
	
	// closes the opened files after transfer completion
	close(fds);
	close(fdd);
	
	return 0;
}

int exist(int fds, int fdd, char* argv[]){

	// provides the approptiate message if one or both of the files do not exist
	if (fds == -1 || fdd == -1) {
		if (fds == -1)
			printf("\"%s\" does NOT exist, please try again\n", argv[1]);
		
		if (fdd == -1)
			printf("\"%s\" does NOT exist, please try agin\n", argv[2]);
			
		return -5;
	}
	
	return 0;

}

void transfer(int fds, int fdd){
	// allocates 100 charachter memory spce to for buffer useage
	char* c = (char*)calloc(100, sizeof(char));
	
	// arbitrary variable greater than 100 to ensure while loop execution
	int sz = 1000;
	
	// while loop checks the content 100 charachters at a time and pass it from one file to the other
	while (sz >= 100){
		//reads 100 charachters from the source file
		sz = read(fds, c, 100);
		
		// writes the buffer content to the destionation file
		write(fdd, c, sz);
	}
	
	return;
	
}

