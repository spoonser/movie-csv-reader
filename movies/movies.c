#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structs */
struct movie
{
	// Movie attributes
	char* title;
	char* languages[5];
	int year;
	double rating;

	// Pointer to next movie
	struct movie* next;
};

/* ----------------------------------------------------------------------------
| FILE PROCESSING FUNCTIONS
---------------------------------------------------------------------------- */
// Parse current line, which is comma delimited
// This is a struct-returning function
struct movie* createMovie(char *curLine)
{
	struct movie* curMovie = malloc(sizeof(struct movie));

	// strtok_r pointer
	char* saveptr;

	// Title is first token
	char* token = strtok_r(curLine, ",", &saveptr);
	curMovie->title = calloc(strlen(token) + 1, sizeof(char));
	strcpy(curMovie->title, token);

	// Second token is year
	token = strtok_r(NULL, ",", &saveptr);
	curMovie->year = atoi(token);

	// Third token is languages - requires some extra work
	char* langptr;
	token = strtok_r(NULL, ",", &saveptr);
	
	// Strip first and last character of the token ()
	token += 1;
	token[strlen(token)-1] = 0;

	// Loop through languages and add to the language array
	int langIdx = 0;
	char* langtoken;
	for (langtoken = strtok_r(token, ";", &langptr); 
		langtoken != NULL;
		langtoken = strtok_r(NULL, ";", &langptr))
	{
		curMovie->languages[langIdx] = langtoken;
		++langIdx;
	}

	// Final token is the rating
	token = strtok_r(NULL, "\n", &saveptr);
	curMovie->rating = strtod(token, NULL);

	// Next node set to NULL for this new movie
	curMovie->next = NULL;

	return curMovie;
}

// Get a linked list of the movies by parsing data from the specified file
struct movie* processFile(char* filePath)
{
	// Read file
	FILE* movieFile = fopen(filePath, "r");

	char* curLine = NULL;
	size_t len = 0;
	ssize_t nread;
	char* token;

	// Head and tail of linked list
	struct movie* head = NULL;
	struct movie* tail = NULL;
	
	// Skip first line of the file (header)
	nread = getline(&curLine, &len, movieFile);

	int movieCount = 0;
	// Read through rest of file line by line
	while ((nread = getline(&curLine, &len, movieFile)) != -1)
	{
		++movieCount;
		// Get new movie node at current line
		struct movie* newNode = createMovie(curLine);

		// Make head if first node
		if (head == NULL)
		{
			// Set head and tail of node to this node
			head = newNode;
			tail = newNode;
		}
		// Node is not first
		else
		{
			// Add node to list and advance tail
			tail->next = newNode;
			tail = newNode;
		}
	}
	free(curLine);
	fclose(movieFile);
	printf("Processed file %s and parsed data for %i movies\n", filePath, movieCount);
	return head;
}


// print stuff
void printMovie(struct movie* aMovie)
{
	printf("%s, %i, %s, %.1f\n", 
	aMovie->title, 
	aMovie->year, 
	aMovie->languages, 
	aMovie->rating);
}

void printMovieList(struct movie* list)
{
	while (list != NULL)
	{
		printMovie(list);
		list = list->next;
	}
}

/* ----------------------------------------------------------------------------
| USER INTERACTIVITY FUNCTIONS
---------------------------------------------------------------------------- */

// Print the movies released in the specified year (option 1)
void moviesInYear(struct movie* list, int inputYear)
{
	// Flag for checking if any films in the file were released in the input year
	int flag = 0;

	// Print out the films from the input year
	while (list != NULL)
	{
		if (list->year == inputYear)
		{
			flag = 1;
			printf("%s\n", list->title);
		}
		list  = list->next;
	}

	// Inform user if no movies were released in the specified year
	if (flag != 1)
	{
		printf("No movies in the input file were released in %d.\n", inputYear);
	}
}

// Show highest rated movie for each year (option 2)
void bestPerYear(struct movie* list)
{
	// Maintain the pointer to the first movie
	struct movie* curMovie = list;
	// Flag for if a movie is in this year
	int flag;
	// Struct for best movie in this year
	struct movie* bestInYear;


	// Loop through all possible years and print the best ones
	for (int i = 1900; i < 2022; ++i)
	{
		// Reset the flag and the best movie for the year
		flag = 0;
		bestInYear = NULL;

		// Get the best movie in current year (i)
		while (curMovie != NULL)
		{
			if (curMovie->year == i)
			{
				flag = 1;
				if (bestInYear != NULL)
				{
					// Update best in year if the current movie rating is higher
					if (curMovie->rating > bestInYear->rating)
					{
						bestInYear = curMovie;
					}
				}
				// First movie found released in the year
				else
				{
					bestInYear = curMovie;
				}
			}
			curMovie = curMovie->next;
		}

		// If there is a best movie of the current year (i), print it
		if (bestInYear != NULL)
		{
			printf("%d %.1f %s\n", bestInYear->year, bestInYear->rating, bestInYear->title);
		}

		// Reset current movie to the first in the list
		curMovie = list;
	}
}

// Show movies and their year of release for a specified language (option 3)
void moviesWithLang(struct movie* list, char* lang)
{

}

int main(int argc, char* argv[])
{
	// Inform user of failure to enter a filename as an argument
	if (argc < 2)
	{
		printf("You must provide the name of the file to process.\n");
		return EXIT_FAILURE;
	}

	// Process movie file 
	struct movie *list = processFile(argv[1]);
	
	// Ask for user input
	int input;
	while (1)
	{
		printf("\n");
		printf("1. Show movies released in the specified year\n");
		printf("2. Show highest rated movie for each year\n");
		printf("3. Show the title and year of release of all movies in a specified language\n");
		printf("4. Exit from the program\n");
		printf("\n");
		printf("Enter a choice from 1 to 4: ");
		
		// Get user input and take action
		scanf("%d", &input);
		if (input == 1)
		{ 
			int inputYear;
			printf("Enter the year that you want to see movies from: ");
			scanf("%d", &inputYear);
			moviesInYear(list, inputYear);
		}
		else if (input == 2)
		{ 
			bestPerYear(list);
		}
		else if (input == 3)
		{ 
			printf("3 entered\n");
		}
		else if (input == 4)
		{ 
			printf("4 entered\n");
			break;
		}
		else 
		{	
		// Invalid pick
		printf("You entered an invalid choice. Try again.\n");
		}
	}

	return 0;
};

