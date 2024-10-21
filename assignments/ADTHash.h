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
typedef struct hash_table_item* HashTableItem;
typedef struct hash_table* HashTable;


typedef int (*HashFunc)(Pointer);

// Creating Functions
HashTableItem create_item(Pointer key, Pointer value);
HashTable create_hash_table(int size);

// Deleting Functions
void delete_item(HashTableItem i);
void delete_hash_table(HashTable ht);

// Υλοποιημένες συναρτήσεις κατακερματισμού για συχνούς τύπους δεδομένων
int hash_string(char* stringg, int a, int m);		// Χρήση όταν το key είναι char*
int hash_integer(int number, int m);
