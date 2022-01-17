#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structs */
struct movie
{
	// Movie attributes
	char* title;
	int year;
	double rating;

	// Pointer to next movie
	struct movie* next;

	// Flexible languages array
	char* languages[];
};

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

	// Third is languages
	char* langptr;
	token = strtok_r(NULL, ",", &saveptr);
	
	// Loop through languages and add to the language array
	int langIdx = 0;
	char* langtoken = strtok_r(token, "[", &langptr);	
	while (langtoken != NULL)
	{
		char *langtoken = strtok_r(NULL, ";", &langptr);
		curMovie->languages[langIdx] = calloc(strlen(token) + 1, sizeof(char));
		strcpy(curMovie->languages[langIdx], langtoken);
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


/* Function prototypes */
char readCSV(char);
char moviesInYear(struct movie);
char bestInYear(struct movie);
char moviesWithLang(struct movie);



int main(int argc, char* argv[])
{
	struct movie *list = processFile("./movies_sample_1.csv");
	printMovieList(list);

	return 0;
};

