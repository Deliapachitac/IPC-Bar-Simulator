////////////////////////////////////////////////////////////////////////
//
// ADT Hash table
//
////////////////////////////////////////////////////////////////////////

#pragma once 
#include <stdbool.h>
#include <assert.h>
#include <stdbool.h> 


// Pointer προς ένα αντικείμενο οποιουδήποτε τύπου.
typedef void* Pointer;
typedef struct node* Node;
typedef struct hash_table* HashTable;


typedef int (*HashFunc)(Pointer);

// Creating Functions
Node create_node(Pointer key);
HashTable create_hash_table(int countline);


// Υλοποιημένες συναρτήσεις κατακερματισμού 
int hash_integer(HashTable table,int key);

void hash_add(HashTable table, int key);
Node hash_find(HashTable table, int key);

// Deleting Functions
void delete_item(Node i);
void delete_hash_table(HashTable ht);

