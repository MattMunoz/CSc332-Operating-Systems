// include directives to allow ceratin necessary functions
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

// funcion defintion
void readAndWrite(char* filepath); 

int main(int argc, char* argv[]){
	char* filepath = argv[1];
	int returnval;
	
	// check file existance and print out an appropriate message
	returnval = access(filepath, F_OK);
	
	if (returnval == 0) 
		printf("\n%s exists\n", filepath);
	else {
		if (errno == ENOENT)
			printf("%s does not exists\n", filepath);
		else if (errno == EACCES)
			printf("%s is not accessible\n", filepath);
		return 0;
	}
	
	// function call to check if a file has read and write access
	readAndWrite(filepath);
	
	return 0;

}

void readAndWrite(char* filepath) {

	// chech readability, returns 0 if the file is readable, otherwise return -1
	int read_acc = access(filepath, R_OK); 
	
	// prints appropriate message on readaility
	if (read_acc == 0)
		printf("%s is readable\n", filepath);
	else
		printf("%s is NOT readable\n", filepath);
		
	// check writeability, returns a 0 if the file is writable, otherwise return -1
	int write_acc = access(filepath, W_OK); 
	 
	 // prints appropriate message on writability
	if (write_acc == 0)
		printf("%s is writable\n", filepath);
	else
		printf("%s is NOT writable\n", filepath);
}
