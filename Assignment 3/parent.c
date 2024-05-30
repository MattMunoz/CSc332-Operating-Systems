// include directive to allow for ceratin necessary function calls
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>


int main(int argc, char *argv[]) {

	// Checks if the right number of arguments are passed, if not it exits the rpogram
	if (argc != 3) {
		printf("Not the right number of args\n");
		return -1;
	}

	// creates the pipe for one-way child parent communication
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		printf("Pipe creeation error");
		return 1;	
	}

	// loops to create the 4 children and produce their appropriate action
	for (int i = 1; i <= 4; i++) {	
	
		// creates a child process
		pid_t pid = fork();
		
		// if we are in the parent process skip this iteration of the 
		// loop since the rest of the code does not pertain to it and 
		// it can just move on to create the other children
		if (pid < 0) {
			fprintf(stderr, "fork failied\n");
			exit(1);
		} else if (pid != 0) 
			continue;
			
		
		// child 1 process executes
		if (i == 1) {
			// replaces the current process image with the childs process image,
			// in this case the data transfer program called "child1" is executed
			// with the args passed to this program
			execl("./child1", "child1" ,argv[1], argv[2], NULL);
			exit(EXIT_FAILURE);
			
		// child 2 process executes
		} else if (i == 2) {
			close(pipefd[0]);					//closes the reading end of pipe for child
			srand(time(NULL));					//generaties a random seed
			int random_num = rand() % 11;				// gets a number between 0 and 10
			printf("Child 2: %d\n", random_num);
			write(pipefd[1], &random_num, sizeof(random_num));	//writes the random number to the pipe
			close(pipefd[1]);					//closes the writing end of the pipe
			exit(EXIT_SUCCESS);		
		} else if (i == 3) {
			close(pipefd[0]);					//closes the reading end of pipe for child
			srand(time(NULL) + 1);					//generaties a random seed
			int random_num = rand() % 11;				// gets a number between 0 and 10
			printf("Child 3: %d\n", random_num);
			write(pipefd[1], &random_num, sizeof(random_num));	//writes the random number to the pipe
			close(pipefd[1]);					//closes the writing end of the pipe
			exit(EXIT_SUCCESS);
		} else if (i == 4) {
			// replaces the current process image with the childs process image,
			// in this case the process to get the date from the system
			execlp("date", "date", NULL);				
		} 
	}
	
	close(pipefd[1]);			// close the writing end of parent side of pipe
	
	int r2, r3;				// define varables to hold the random numbers received from child process
	
	wait(NULL);				// set parent to wait for child 2 to finish 
	
	read(pipefd[0], &r2, sizeof(r2));	// read the random number sent by child 2
	printf("In parent, the second childs random number is %d\n", r2);
	
	wait(NULL);				// set parent to wait for child 3 to finish 
	
	read(pipefd[0], &r3, sizeof(r3));	// read the random number sent by child 3
	printf("In parent, the third childs random number is %d\n", r3);
	
	close(pipefd[0]);			//close the reading side of the pipe for parent
	
	// compare the two random numbers and display the child with the larger number
	if (r2 > r3) 
		printf("\nThe winner is child 2\n\n");
	else if (r3 > r2)
		printf("The winner is child 3\n\n");
	else
		printf("Neither child wins\n\n");
		
	// set parent to wait for child 4 to finish 
	wait(NULL);			
	
	
	return 0;
}
