#include <stdbool.h>
#include <assert.h>
#include <stdbool.h> 

// Pointer to an object of any type.
typedef void* Pointer;

//Pointers to the structs
typedef struct hashnode* HashNode;
typedef struct hash_table* HashTable;


// Creating functions
HashNode create_node(Pointer value);
HashTable create_hash_table(int countline);

//Function to find the index of the array 
int hash_string(HashTable table,Pointer value);

//Function that inserts the value into the array
void hash_add(HashTable table, Pointer value);

//Finds the value in O(1) complexity 
Pointer hash_find(HashTable table, Pointer value);

// Deleting functions
void delete_item(HashTable table, Pointer value);
void delete_hash_table(HashTable table);