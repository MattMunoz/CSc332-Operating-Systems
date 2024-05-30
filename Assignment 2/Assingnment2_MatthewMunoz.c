// import to allow for ceratin functionality
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// function defintions for use in the program
void insertatEnd(int);
void insertAfter(int, int);
void delete(int);
void printList();

// structure of the node, which includes the data and two pointers
struct node {
	int data;
	struct node *next;
	struct node *prev;
};

// global head and tail pinter for use in any functino without having to call
struct node *head = NULL;
struct node *tail = NULL;	// tail was only implemented for insertion efficency

int main(int argc, char* argv[]) {

	// open a file and set it to rewad only
	FILE *fd = fopen(argv[1], "r");
	
	// check if the file exists, exits  if it doesnt
	if (fd == NULL) {
		printf("\"%s\" does NOT exist, exiting...\n", argv[1]);
		return -2;
	}
	
	// inserts the numbers in the file one int at a time
	int num;
	while (fscanf(fd, "%d", &num) == 1){
		insertatEnd(num);
	}
	
	//closes the file after all ints have been inserted
	fclose(fd);
	
	// info for the user along with the current state of the linked list
	printf("All numbers were inserted into the linked list\n");
	printList();

	// varous variables that will be used for the functions
	int selection; 	// selector for the list
	int dataInsert; // int to be inserted
	int dataDelete; // int to be deleted
	int dataPrev;	// int that will preceed the inserted int
	
	// do..while loop for easy of use and ensure menu display
	do {
		printf("\nSelect an option form the folowing list:\n\t1. Insert new node at the end\n\t2. Insert new node after a specific node\n\t3. Delete a node\n\t0. Quit\n\nSelection: ");
		
		scanf("%d", &selection);  // takes the user input
		
		// swich stamtnet for easy of implemenation instead of a long if...else
		switch (selection) 
		{
		// selection of 1 allows the user to insert a int at the end of the linked list
		case 1: 
			printf("What number would you like to insert: ");
			scanf("%d", &dataInsert);
			insertatEnd(dataInsert);
			printList();
			break;
		// selection of 2 allows the user to insert a int after any int the user wants, if previous exists
		case 2: 
			printf("What number would you like to insert: ");
			scanf("%d", &dataInsert);
			printf("What number would you like it to follow: ");
			scanf("%d", &dataPrev);
			insertAfter(dataInsert, dataPrev);
			printList();
			break;
		// selection of 3 allows for the user to delete an int from the linked list, if it exists
		case 3: 
			printf("What number would you like to delete: ");
			scanf("%d", &dataDelete);
			delete(dataDelete);
			printList();
			break;
		case 0: 
			//quits the program and displays the linked list
			printf("Quiting, here is the lists forwards and backwards\n");
			printList();
			break;
		default: 
			// notifies the user it selected an invalid value and before redisplaying the menu again
			printf("Not a valid input, please select again\n");
		}
		
		
	} while (selection != 0);


	return 0;
}

// inserts a given int at the end of the linked list
void insertatEnd(int d) {
	//creates a new node, allocates memory for it and assins proper values to the structure
	struct node *newNode;
	newNode = malloc(sizeof(struct node));
	newNode->data = d;
	newNode->next = NULL;
	newNode->prev = NULL;
	
	// checks if the list is empty to set head, node, and tail appropriately
	if (head == NULL){
		head = newNode;
		tail = newNode;
		return;
	}
	
	// inserts the newnode at the end by inserting it after the current tail pinter and then moving the tail pointer to the new node
	tail->next = newNode;
	newNode->prev = tail;
	tail = newNode;

	return;
}

// insrts a new node with given data after another given node
void insertAfter(int d, int p) {
	
	// checks if the list is empty, therefore we cant insert after any node. Then the function exits
	if (head == NULL) {
		printf("\nList is empty, cant insert after a node\n");
		return;
	}

	// creates a temp variable to iterate through the list
	struct node *temp = head;

	// looks for the node that will proceed the node that is wanted to be inserted
	while (temp->data != p)
	{
		// if the user wants to place it after a node that does not exist a prompt is displayed and the functino returns
		if (temp->next == NULL) {
			printf("\n%d is not in the list\n", p);
			return;
		}
		
		temp = temp->next;
	}
	
	// if the user want to place it ath the end of the list simply call the insert at end function for simplicity
	if (temp->next == NULL) {
		insertatEnd(d);
		return;
	}
	
	// selection of 1 allows the user to insert a number at the end of the linked list
	struct node *newNode;
	newNode = malloc(sizeof(struct node));
	newNode->data = d;
	newNode->next = NULL;
	newNode->prev = NULL;
	
	//sets the appropriate pointers for the previous, next and curent nodes to allow for appropriate conectivity
	newNode->next = temp->next;
	newNode->prev = temp;
	temp->next->prev = newNode;
	temp->next = newNode;
	return;
}

// function to delete a given int from the list, if it exists
void delete(int d)
{
	// if the list is empty nothing can be deleted, therefore infrom the user and exit
	if (head == NULL) {
		printf("\nList is empty, their is nothing to delete\n");
		return;
	}
	 
	// create a temporary pinter to iterat thrtough the list 
	struct node *temp = head;
	
	//iterate through the list untill the values is either found or not
	while (temp->data != d)
	{
		// if its not found prompt the user and return from the function
		if (temp->next == NULL) {
			printf("\n%d is not in the list\n", d);
			return;
		}
		
		temp = temp->next;
	}
	
	// if their is only one node in the list and that the one to be deleted, delte it iand set the tail and head pinter to NULL
	if ((tail == temp) && (head == temp)){
		tail = NULL;
		head = NULL;
		return;
	}
	
	// if the first node is to be deleted then move the head one over and delete the first node
	if (temp == head) {
		head = temp->next;
		temp->next->prev = NULL;
		return;
	}
	
	if (temp == tail) {
		tail = temp->prev;
		temp->prev->next = NULL;
		return;
	}
	
	//delete the node and set the previous and next nodes pinter appropriately
	temp->prev->next = temp->next;
	temp->next->prev = temp->prev;
}

//print the current state of the linked list forwards and backwards
void printList()
{
	// set a temp pointer to iterate through the list
	printf("\n----------------------------------------------\nCurrent List:\n");
	printf("\t\nForward: head");
	struct node *temp = head;
	
	// iterate through the list forward and print the current values until the end is reached
	while (temp != NULL)
	{
		printf(" <|%d|> ", temp->data);
		temp = temp->next;
	}
	
	printf("tail\n");
	
	printf("\t\nBackward: tail ");
	
	temp = tail;
	
	// iterate through the list backwards and print the current value untill the front is reached
	while (temp != NULL)
	{
		printf(" <|%d|> ", temp->data);
		temp = temp->prev;
	}
	
	printf("head\n----------------------------------------------\n");
	
}
