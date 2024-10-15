///////////////////////////////////////
//
// ADT List
//
//////////////////////////////////////

#pragma once 
#include <stdbool.h>
#include <assert.h>
#include "common_types.h"

// Οι σταθερές αυτές συμβολίζουν κόμβους _πριν_ τον πρώτο και _μετά_ τον τελευταίο
#define LIST_BOF (ListNode)0
#define LIST_EOF (ListNode)0


// Λίστες και κόμβοι αναπαριστώνται από τους τύπους List και ListNode. Ο χρήστης δε χρειάζεται να γνωρίζει το περιεχόμενο
// των τύπων αυτών, απλά χρησιμοποιεί τις συναρτήσεις list_<foo> που δέχονται και επιστρέφουν List / ListNode.
//
// Οι τύποι αυτοί ορίζινται ως pointers στα "struct list" και "struct list_node" των οποίων το
// περιεχόμενο είναι άγνωστο (incomplete structs), και εξαρτάται από την υλοποίηση του ADT List.
//
typedef struct list* List;
typedef struct list_node* ListNode;



// Δημιουργεί και επιστρέφει μια νέα λίστα.
List list_create();

// Επιστρέφει τον αριθμό στοιχείων που περιέχει η λίστα.
int list_size(List list);

// Προσθέτει έναν νέο κόμβο μετά τον node, ή στην αρχή αν node == LIST_BOF, με περιεχόμενο value.
void list_insert_next(List list, ListNode node, Pointer value);

// Προσθετει εναν νεο κομβο πριν τον node 
void list_insert_previous(List list, ListNode node, Pointer value);

// Αφαιρεί τον επόμενο κόμβο από τον node, ή τον πρώτο κόμβο αν node == LIST_BOF.
void list_remove_next(List list, ListNode node);

// Επιστρέφει την πρώτη τιμή που είναι ισοδύναμη με value
// (με βάση τη συνάρτηση compare), ή NULL αν δεν υπάρχει
Pointer list_find(List list, Pointer value, CompareFunc compare);




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
