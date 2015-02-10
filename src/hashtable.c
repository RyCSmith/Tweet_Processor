#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

/*
 *This calculates the hashcode of a string by adding character values
 * Returns 1 if successful, and 0 if an error occurred 
 */
int hash(char* string, unsigned long* value)
{
  if (string == NULL || value == NULL){
  	return 0;
  }
  int i; //for loop counter
  int total = 0; //counter to hold total ascii value of string
  for (i = 0; i < strlen(string); i += 1){
  	int temp_value = (int) string[i];
  	total += temp_value;
  }
  if (total <= 0){
	  total = total * -1;
  }
  *value = total;
  return 1;
}


/*
 * Adds the string to the hashtable in the appropriate bucket.
 * Returns 1 if successful, and 0 if not
 */
int put(char* string, hashtable* h)
{
  if (string == NULL || h == NULL)
	  return 0;
  //create a long on the stack
  //get its hashCode
  unsigned long returned_code = 0;
  unsigned long* returned_code_ptr = &returned_code;
  hash(string, returned_code_ptr);
  returned_code = returned_code % CAPACITY;


  //hashCode ready at this point
  //check to see if the string exists in the corresponding bucket's linked list
  //if so increment the tags num_found and return
  node* search_head = h->list[returned_code];
  while (search_head != NULL){
  	if (strcmp(search_head->value, string) == 0){
  		search_head->num_found = search_head->num_found + 1;
  		return 1;
  	}
  	search_head = search_head->next;
  }

  //create new node and new name space
  node* new = malloc(sizeof(node));
  if (new == NULL)
  	return 0;
  //malloc space for fields, set num_found to 1
  new->value = malloc(strlen(string)+1);
  if(new->value == NULL)
  	return 0;

  new->num_found = 1;
  
  //copy string into new namespace
  strcpy(new->value, string);

  //hold onto current head in that bucket
  node* head = h->list[returned_code];
  //fill in next pointer in new node
  new->next = head;
  //update pointer from hashtable array
  h->list[returned_code] = new;

  return 1;

}


/*
 * Determines whether the string is in the hashtable.
 * Return 1 if it is found, 0 if not
 * Note: not currently used in tweets program.
 */
int get(char* string, hashtable* h)
{
  if (string == NULL || h == NULL)
	  return 0;
  //create a long on the stack
  //get its hashCode
  unsigned long returned_code = 0;
  unsigned long* returned_code_ptr = &returned_code;
  hash(string, returned_code_ptr);
  returned_code = returned_code % CAPACITY;


  //hashCode ready at this point
  //check to see if the string exists in the corresponding bucket's linked list
  node* search_head = h->list[returned_code];
  while (search_head != NULL){
  	if (strcmp(search_head->value, string) == 0)
  		return 1;
  	search_head = search_head->next;
  }
  return 0;
}

/*
* Find the 10 tweets that appear most frequently in the dataset and creates an array of node* to them on the stack.
* Copies node* array to heap and returns it.
*/
node** find_frequent(hashtable* h){
	if (h == NULL)
		return 0;
	node* top_ten_array[10];
	int clear;
	for (clear = 0; clear < 10; clear += 1){
		top_ten_array[clear] = NULL;
	}

	//for some reason this needs to be declared outside of the loop on eniac
	//produces weird results when node* is moved to within for loop
	node* search_head;

	int i;
	for (i = 0; i < CAPACITY; i+=1){
		search_head = h->list[i];
		while (search_head != NULL){
			int j;
			for (j = 9; j >= 0; j -= 1 ){
				if (top_ten_array[j] == NULL){
					top_ten_array[j] = search_head;
				}
				else if (top_ten_array[j]->num_found == search_head->num_found){
					int k;
					for (k = 0; k < j - 1; k += 1){
						top_ten_array[k] = top_ten_array[k + 1];
					}
					top_ten_array[j - 1] = search_head;
					break;
				}
				else if (top_ten_array[j]->num_found < search_head->num_found){
					int x;
					for (x = 0; x < j; x += 1){
						top_ten_array[x] = top_ten_array[x + 1];
					}
					top_ten_array[j] = search_head;
					break;
				}
			}
			search_head = search_head->next;
		}
	}
	//nodes are already in heap, but array of pointers to top 10 is local so malloc space and assign
	node** heap_ten_list = malloc(sizeof(node*)*10);
	if(heap_ten_list == NULL){
		return NULL;
	}
	else {
		int t;
		for (t = 0; t < 10; t += 1){
			*(heap_ten_list + t) = top_ten_array[t];
		}
	}
	return heap_ten_list;
}

/*
* Walks through the linked lists in each bucket of the hashtable forwards and frees all malloc'd space
* Returns 1 if successful, 0 for error
*/
int free_hashtable(hashtable* h){
	if (h == NULL){
		return 0;
	}
	node* search_head;
	node* temp;
	int i;
	for (i = 0; i < CAPACITY; i+=1){
		search_head = h->list[i];
		while (search_head != NULL){
			temp = search_head->next;
			free(search_head->value);
			free(search_head);
			search_head = temp;
		}
	}
	free(h);
	return 1;
}
  
