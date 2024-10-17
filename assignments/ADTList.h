///////////////////////////////////////
//
// ADT List
//
//////////////////////////////////////

#pragma once 
#include <stdbool.h>
#include <assert.h>
#include <stdbool.h> 

// Pointer προς ένα αντικείμενο οποιουδήποτε τύπου.
typedef void* Pointer;

// Δείκτης σε συνάρτηση που συγκρίνει 2 στοιχεία a και b 
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Δείκτης σε συνάρτηση που καταστρέφει ένα στοιχείο value
typedef void (*DestroyFunc)(Pointer value);


typedef struct list* List;
typedef struct list_node* ListNode;



// Δημιουργεί και επιστρέφει μια νέα λίστα.
List list_create();

// Επιστρέφει τον αριθμό στοιχείων που περιέχει η λίστα.
int list_size(List list);

// Προσθέτει έναν νέο κόμβο μετά τον node, ή στην αρχή αν node == LIST_BOF, με περιεχόμενο value.
void list_insert(List list, Pointer value);

// Αφαιρεί τον επόμενο κόμβο από τον node, ή τον πρώτο κόμβο αν node == LIST_BOF.
void list_remove(List list, ListNode node);



// Διάσχιση της λίστας 

// Επιστρέφουν τον πρώτο και τον τελευταίο κομβο της λίστας
ListNode list_first(List list);
ListNode list_last(List list);

// Επιστρέφει τον κόμβο μετά από τον node και τον προηγουμενο
ListNode list_next(List list, ListNode node);
ListNode list_previous(List list, ListNode node);

// Επιστρέφει το περιεχόμενο του κόμβου node
Pointer list_node_value(List list, ListNode node);

// Βρίσκει τo πρώτo στοιχείο που είναι ισοδύναμο με value (με βάση τη συνάρτηση compare).
// Επιστρέφει τον κόμβο του στοιχείου, ή LIST_EOF αν δεν βρεθεί.
ListNode list_find_node(List list, Pointer value, CompareFunc compare);
