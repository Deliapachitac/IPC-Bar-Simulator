#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "Hash.h"


struct hash_table {
	int size;		//The variable size will be used to calculate the index
    HashNode* items; // An array of hashnode items			
};

struct hashnode{
	int key;		// The key we use to find the index of the array 
	Pointer value;  //We will save the graph nodesin this value 
    HashNode next;  //Because we try to implement a hash table with seperate chaining this means that some elements may have the same index so we need the next node to save the elements
};


HashNode create_node(int key,Pointer value){
    //Allocate memory for the hash node we create
    HashNode item = malloc(sizeof(*item));
    item->key=key;
    item->value = value;
    item->next = NULL;

    return item;
}

HashTable create_hash_table(int countline){

    //Allocate memory for the hash table
    HashTable mytable= malloc(sizeof(*mytable));
    mytable->size=countline*2; //Initialize  the size with the doublesize of the line in the file so we wont have to rehash
    mytable->items = malloc(mytable->size * sizeof(HashNode));  

    //Itialize all the items with NULL 
    for (int i = 0; i < mytable->size; i++) {
        mytable->items[i] = NULL;  
    }

    return mytable;
}

int hash_integer(HashTable table,int key) {

    return key % (table->size); //calculating the index of the array
}

void hash_add(HashTable table, int key, Pointer value) {

    //calculating the index
    int index = hash_integer(table, key);
    
    //Create the new hash node that we will add to the array
    HashNode new_node = create_node(key,value);

    //If the hash table at the index is empty insert the new node
    if (table->items[index] == NULL) {
        table->items[index] = new_node;
    } else {
        // Otherwise insert in the list (separate chaining)
        new_node->next = table->items[index];
        table->items[index] = new_node;
    }
}

Pointer hash_find(HashTable table, int key) {

    //Calculating the index to find the position of the array we will search
    int index = hash_integer(table,key);
    
    // Traverse the list at the index to find the value
    HashNode temp = table->items[index];
    while (temp != NULL) {
        if (temp->key == key) {
            return temp->value;  
        }
        temp = temp->next;
    }

    // Value not found
    return NULL;  
}

void delete_item(HashTable table, int id ) {
    //Find the item we want to delete and delete it 
    Pointer node = hash_find(table,id);
    free(node);
}


void delete_hash_table(HashTable table) {
    //Traverse every item in the table 
    for (int i = 0; i < table->size; i++) {
        if (table->items[i] != NULL) {
            while (table->items[i] != NULL) {
                HashNode next = table->items[i]->next;
                free(table->items[i]);  //Free the current node
                table->items[i] = next;  
            };  
        }
    }
    free(table->items);  // Free the array of items
    free(table);  // Free the hash table itself
}