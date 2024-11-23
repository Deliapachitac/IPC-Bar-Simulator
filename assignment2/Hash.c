#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Hash.h"


struct hash_table {
	int size;		//The variable size will be used to calculate the index
    HashNode* items; // An array of hashnode items			
};

struct hashnode{
    int counter;
	Pointer value;
    HashNode next;  //Because we try to implement a hash table with seperate chaining this means that some elements may have the same index so we need the next node to save the elements
};


HashNode create_node(Pointer value){
    //Allocate memory for the hash node we create
    HashNode item = malloc(sizeof(*item));
    item->value = value;
    item->counter= 1 ;
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

    if (value == NULL) {
        printf( "Error: Null value \n");
        exit(1);
    }

    //calculating the index
    int index = hash_string(table, value);
    
    // Search for the value in the linked list at the given index
    HashNode current = table->items[index];
    while (current != NULL) {
        if (strcmp(current->value, value) == 0) { 
            current->counter++;
            return; // Value already exists, no need to insert
        }
        current = current->next;
    }

    //Create the new hash node that we will add to the array
    HashNode new_node = create_node(value);

    // Insert the new node at the end of the linked list
    if (table->items[index] == NULL) {
        // If the bucket is empty, the new node is the first (and only) node
        table->items[index] = new_node;
    } else {
        // If the bucket is not empty, traverse to the end of the list
        HashNode last = table->items[index];
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = new_node;
    }


}

HashNode hash_find(HashTable table, Pointer value) {

    //Calculating the index to find the position of the array we will search
    int index = hash_string(table,value);
    
    // Traverse the list at the index to find the value
    HashNode temp = table->items[index];
    while (temp != NULL) {
        if (strcmp(temp->value,value)== 0) {
            return temp;  
        }
        temp = temp->next;
    }

    // Value not found
    return NULL;  
}

int get_counter(HashTable table,Pointer value){
    HashNode node = hash_find(table, value);
    if (node != NULL) {
        return node->counter; // Return the counter if the node exists
    }
    return 0; // Return 0 if the value is not found

}

HashNode hash_first(HashTable table) {
	
	for (int i = 0; i < table->size; i++){
        if (table->items[i]!= NULL){
            return table->items[i];
        }
    }	

	return NULL;
}


HashNode hash_next(HashTable table, HashNode node) {
    if (node == NULL) {
        return NULL; // No current node, nothing to iterate over
    }

    // If there's a next node in the current chain, return it
    if (node->next != NULL) {
        return node->next;
    }

    // Otherwise, move to the next bucket
    int index = hash_string(table, node->value); // Find the bucket of the current node
    for (int i = index + 1; i < table->size; i++) {
        if (table->items[i] != NULL) {
            return table->items[i]; // Return the first non-NULL bucket
        }
    }

    // If no further nodes are found, return NULL
    return NULL;
}

Pointer hash_find_value(HashTable table, HashNode node){
    if (node->value !=NULL)
    {
        return node->value;
    }
    return NULL;
    
}

void delete_item(HashTable table, Pointer value ) {
    //Find the item we want to delete and delete it
    int index = hash_string(table, value);
    HashNode current = table->items[index];
    HashNode prev = NULL;

    while (current != NULL) {
        if (strcmp(current->value, value) == 0) {
            if (prev == NULL) {
                table->items[index] = current->next;  // Remove from head
            } else {
                prev->next = current->next;  // Remove from middle or end
            }
            free(current);  // Free the node
            return;
        }
        prev = current;
        current = current->next;
    }
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