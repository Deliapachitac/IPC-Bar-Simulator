///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Hash Table
//
///////////////////////////////////////////////////////////
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ADTHash.h"

// Δομή του κάθε κόμβου που έχει το hash table
struct node{
	int key;		// Το κλειδί που χρησιμοποιείται για να hash-αρουμε
	Node next;
};

struct hash_table {
	int size;					
    Node* items;			
};


Node create_node(Pointer key){
    Node item = malloc(sizeof(*item));
    item->key=key;
    item->next = NULL;

    return item;
}

HashTable create_hash_table(int countline){

    HashTable mytable= malloc(sizeof(*mytable));
    mytable->size=countline*2;

    return mytable;
}

int hash_integer(HashTable table,int key) {

    return key % (table->size);
}

void hash_add(HashTable table, int key) {
    int index = hash_integer(table, key);
    
    // Create a new node
    Node new_node = create_node(key);

    // If the hash table entry at the index is empty, insert the new node
    if (table->items[index] == NULL) {
        table->items[index] = new_node;
    } else {
        // Otherwise, insert at the beginning of the linked list (separate chaining)
        new_node->next = table->items[index];
        table->items[index] = new_node;
    }
}

Node hash_find(HashTable table, int key) {
    int index = hash_integer(table,key);
    Node temp = table->items[index];

    // Traverse the linked list at the index to find the value
    while (temp != NULL) {
        if (temp->key == key) {
            return temp;  // Value found
        }
        temp = temp->next;
    }
    return NULL;  // Value not found
}

void delete_item(Node i) {
    free(i->key);
    free(i->next);
    free(i);
}


void delete_hash_table(HashTable ht) {
    for (int i = 0; i < ht->size; i++) {
        HashTable item = ht->items[i];
        if (item != NULL) {
            delete_item(item);
        }
    }
    free(ht->items);
    free(ht);
}


// int hash_string(char* stringg, int a, int m) {
// 	long hash = 0;
//     const int len_string = strlen(stringg);
//     for (int i = 0; i < len_string; i++) {
//         hash += (long)pow(a, len_string - (i+1)) * stringg[i];
//         hash = hash % m;
//     }
//     return (int)hash;
// }



