///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Hash Table
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include "ADTHash.h"
#include <string.h>



// Οι κόμβοι του map στην υλοποίηση με hash table, μπορούν να είναι σε 3 διαφορετικές καταστάσεις,
// ώστε αν διαγράψουμε κάποιον κόμβο, αυτός να μην είναι empty, ώστε να μην επηρεάζεται η αναζήτηση
// αλλά ούτε occupied, ώστε η εισαγωγή να μπορεί να το κάνει overwrite.
typedef enum {
	EMPTY, OCCUPIED, DELETED
} State;


// Χρησιμοποιούμε open addressing, οπότε σύμφωνα με την θεωρία, πρέπει πάντα να διατηρούμε
// τον load factor του  hash table μικρότερο ή ίσο του 0.5, για να έχουμε αποδoτικές πράξεις
#define MAX_LOAD_FACTOR 0.5

// Δομή του κάθε κόμβου που έχει το hash table
struct hash_table_item{
	Pointer key;		// Το κλειδί που χρησιμοποιείται για να hash-αρουμε
	Pointer value;  	// Η τιμή που αντισοιχίζεται στο παραπάνω κλειδί
	State state;		// Μεταβλητή για να μαρκάρουμε την κατάσταση των κόμβων (βλέπε διαγραφή)
};


// Δομή του Map (περιέχει όλες τις πληροφορίες που χρεαζόμαστε για το HashTable)
struct hash_table {
	int size;					// Πόσα στοιχεία έχουμε προσθέσει
	int count;	
    HashTableItem* items;			
};


HashTableItem create_item(Pointer key, Pointer value){
    HashTableItem item = malloc(sizeof(*item));
    item->key=key;
    item->value=value;
    item->state= EMPTY;

    return item;
}

HashTable create_hash_table(int size){

    HashTable mytable= malloc(sizeof(*mytable));
    mytable->count=0;
    mytable->size=size;
    mytable->items=malloc(sizeof(*mytable->items)); //????

    return mytable;
}


void delete_item(HashTableItem i) {
    free(i->key);
    free(i->value);
    free(i->state);
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


int hash_integer(int number, int m) {

    return number % m;
}

