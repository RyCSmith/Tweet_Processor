//quantity of buckets in the hashtable
#define CAPACITY 100

typedef struct Node node;
struct Node {
  char* value;
  unsigned int num_found;
  node* next; 
};

typedef struct Hashtable hashtable;
struct Hashtable {
  node* list[CAPACITY];
};

/* Function declarations*/
int hash(char*, unsigned long*);
int put(char*, hashtable*);
int get(char*, hashtable*);
node** find_frequent(hashtable*);
int free_hashtable(hashtable*);
