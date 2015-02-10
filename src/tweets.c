#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashtable.h"
#include <ctype.h>

//NOTE: using 0 for all failiures and 1 for all success

int main(int argc, char **argv){
	//opens file via command line argument
	FILE *src_file = fopen(argv[1], "r");;
	if (src_file == NULL){
		fprintf(stderr, "There was an error opening the file.") ;
		return 0;
	}

	//define hashtable and malloc space
	hashtable* tweet_table = malloc(sizeof(hashtable));
	//read word by word and put any hashtags in hashtable
	char current_word[1000];
	int looper = 1;
	while (looper == 1){
		if (fscanf(src_file, "%s", current_word) == EOF){
			looper = 0;
		}
		else if (current_word[0] == '#'){
			int i;
			for (i = 1; i < strlen(current_word); i+=1){
				current_word[i] = tolower(current_word[i]);
			}
			assert (put(current_word, tweet_table) == 1);
		}
	}

	//call function to sort and return pointer to array of node pointers
	//print values and counts of top 10
	node** top_ten_list = find_frequent(tweet_table);
	if (top_ten_list == NULL){
		fprintf(stderr, "Could not generate top ten list.") ;
	}
	else {
		int z;
		for (z = 9; z >= 0; z -= 1){
			printf("%s: ", (*(top_ten_list + z))->value);
			printf("%d\n", (*(top_ten_list + z))->num_found);
		}
	}

	//free all malloc'd space
	assert(free_hashtable(tweet_table) == 1);
	//using valgrind this is legitimate and frees 100% of my malloc'd memory, it also works on mac
	//for some reason, it causes an error on linux so it is commented out
	free(top_ten_list);

	//close file when finished with operations
	fclose(src_file);

	return 1;
}
