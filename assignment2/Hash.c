#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "Hash.h"


struct hash_table {
	int size;		//The variable size will be used to calculate the index
    HashNode* items; // An array of hashnode items			
};

struct hashnode{
	Pointer value;
    HashNode next;  //Because we try to implement a hash table with seperate chaining this means that some elements may have the same index so we need the next node to save the elements
};


HashNode create_node(Pointer value){
    //Allocate memory for the hash node we create
    HashNode item = malloc(sizeof(*item));
    item->value = value;
    item->next = NULL;

    return item;
}

HashTable create_hash_table(int countline){

    //Allocate memory for the hash table
    HashTable mytable= malloc(sizeof(*mytable));
    mytable->size=countline; 
    mytable->items = malloc(mytable->size * sizeof(HashNode));  

    //Itialize all the items with NULL 
    for (int i = 0; i < mytable->size; i++) {
        mytable->items[i] = NULL;  
    }

    return mytable;
}

int hash_string(HashTable table, Pointer value) { 
    long long p = 31; // Base for the polynomial hash
    long long m = table->size; // Table size (should be prime number)
    unsigned long long hash = 0; // Use unsigned long long to prevent overflow
    unsigned long long p_pow = 1; // p^i (power of p)

    for (char* s = value; *s != '\0'; s++) {
        // Use unsigned char to handle all characters properly
        hash = (hash + ((unsigned char)*s) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }

    return (int) hash; // Return the final hash value, cast to int
}

void hash_add(HashTable table, Pointer value) {

    //calculating the index
    int index = hash_string(table, value);
    
    //Create the new hash node that we will add to the array
    HashNode new_node = create_node(value);

    //If the hash table at the index is empty insert the new node
    if (table->items[index] == NULL) {
        table->items[index] = new_node;
    } else {
        // Otherwise insert in the list (separate chaining)
        new_node->next = table->items[index];
        table->items[index] = new_node;
    }
}

Pointer hash_find(HashTable table, Pointer value) {

    //Calculating the index to find the position of the array we will search
    int index = hash_string(table,value);
    
    // Traverse the list at the index to find the value
    HashNode temp = table->items[index];
    while (temp != NULL) {
        if (strcmp(temp->value,value)== 0) {
            return temp->value;  
        }
        temp = temp->next;
    }

    // Value not found
    return NULL;  
}

void delete_item(HashTable table, Pointer value ) {
    //Find the item we want to delete and delete it 
    Pointer node = hash_find(table,value);
    free(node);
}


void delete_hash_table(HashTable table) {
    //Traverse every item in the table 
    for (int i = 0; i < table->size; i++) {
        if (table->items[i] != NULL) {
            while (table->items[i] != NULL) {
                HashNode next = table->items[i]->next;
                free(table->items[i]);        
                table->items[i] = next;  
            };  
        }
    }
    free(table->items);  // Free the array of items
    free(table);  // Free the hash table itself
}