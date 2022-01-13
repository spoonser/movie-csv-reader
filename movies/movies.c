#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structs */
struct movie
{
	char title[20]; // change for different allocation
	char languages[2][4]; //change
	int year;
	double rating;

	// pointer to next movie goes here

};

/* Function prototypes */
char readCSV(char);


/* Function definitions */


int main(int argc, char* argv[])
{
	printf("Hello world!\n");

	return 0;
};

