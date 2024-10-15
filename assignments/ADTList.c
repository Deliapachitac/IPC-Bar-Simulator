///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT List 
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ADTList.h"


// Ενα List είναι pointer σε αυτό το struct
struct list {
	ListNode dummy;				// χρησιμοποιούμε dummy κόμβο, ώστε ακόμα και η κενή λίστα να έχει έναν κόμβο.
	ListNode last;				// δείκτης στον τελευταίο κόμβο, ή στον dummy (αν η λίστα είναι κενή)
	int size;					// μέγεθος, ώστε η list_size να είναι Ο(1)
	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο της λίστας.
};

struct list_node {
	ListNode next;		// Δείκτης στον επόμενο
	ListNode previous;	// Δείκτης στον προηγουμενο
	Pointer value;		// Η τιμή που αποθηκεύουμε στον κόμβο
};



List list_create() {
	// Πρώτα δημιουργούμε το stuct
	List list = malloc(sizeof(*list));
	list->size = 0;

	// Χρησιμοποιούμε dummy κόμβο, ώστε ακόμα και μια άδεια λίστα να έχει ένα κόμβο
	// (απλοποιεί τους αλγορίθμους). Οπότε πρέπει να τον δημιουργήσουμε.
	list->dummy = malloc(sizeof(*list->dummy));
	list->dummy->next = NULL;		
	list->dummy->previous = NULL;
	list->last = list->dummy;

	return list;
}

int list_size(List list) {
	return list->size;
}

void list_insert_next(List list, ListNode node, Pointer value) {
	// Αν το node είναι NULL απλά εισάγουμε μετά τον dummy κόμβο!
	// Αυτή ακριβώς είναι η αξία του dummy, δε χρειαζόμαστε ξεχωριστή υλοποίηση.
	if (node == NULL)
		node = list->dummy;

	// Δημιουργία του νέου κόμβου
	ListNode new = malloc(sizeof(*new));
	new->value = value;

	// Σύνδεση του new ανάμεσα στο node και το node->next
	new->next = node->next;
	node->next = new;
	new->previous = node;

	// Ενημέρωση των size & last
	list->size++;
	if (list->last == node)
		list->last = new;
}

void list_insert_previous(List list, ListNode node, Pointer value) {

	list_insert_next(list,node->previous,value);
}

void list_remove_next(List list, ListNode node) {
	// Αν το node είναι NULL απλά διαγράφουμε μετά τον dummy κόμβο!
	// Αυτή ακριβώς είναι η αξία του dummy, δε χρειαζόμαστε ξεχωριστή υλοποίηση.
	if (node == NULL)
		node = list->dummy;

	// Ο κόμβος προς διαγραφή είναι ο επόμενος του node, ο οποίος πρέπει να υπάρχει
	ListNode removed = node->next;
	assert(removed != NULL);		// LCOV_EXCL_LINE

	if (list->destroy_value != NULL)
		list->destroy_value(removed->value);

	// Σύνδεση του node με τον επόμενο του removed
	node->next = removed->next;		// πριν το free!

	free(removed);

	// Ενημέρωση των size & last
	list->size--;
	if (list->last == removed)
		list->last = node;
}


// Διάσχιση της λίστας

ListNode list_first(List list) {
	// Ο πρώτος κόμβος είναι ο επόμενος του dummy.
	if (list->dummy->next == list->dummy)
		return LIST_EOF;		// κενή λίστα
	else
		return list->dummy->next;
}

ListNode list_last(List list) {
	// Προσοχή, αν η λίστα είναι κενή το last δείχνει στον dummy, εμείς όμως θέλουμε να επιστρέψουμε NULL, όχι τον dummy!

	if (list->last == list->dummy)
		return LIST_EOF;		// κενή λίστα
	else
		return list->last;
}

ListNode list_next(List list, ListNode node) {
	assert(node != NULL);	// LCOV_EXCL_LINE 
	return node->next;
}

ListNode list_previous(List list, ListNode node) {
	assert(node != NULL);	 
	return node->previous;
}

Pointer list_node_value(List list, ListNode node) {
	assert(node != NULL);	
	return node->value;
}

ListNode list_find_node(List list, Pointer value, CompareFunc compare) {
	// διάσχιση όλης της λίστας, καλούμε την compare μέχρι να επιστρέψει 0

	for (ListNode node = list->dummy->next; node != NULL; node = node->next)
		if (compare(value, node->value) == 0)
			return node;		// βρέθηκε

	return NULL;	// δεν υπάρχει
}

int compare_objects(Pointer a, Pointer b){
    int obj1 =(uintptr_t)a;
    int obj2 =(uintptr_t)b;

    if(obj1 > obj2 ){
        return 1;
    }else if(obj1 < obj2){
        return -1;
    }else{
        return 0;
    }
}

int main(void){

	List list= list_create();

	printf("The size:%d\n", list_size(list));

	for (int i = 0; i < 10; i=i+2)
	{
		list_insert_next(list,list->last, i);
		printf("the i is : %d    ",i);
	}
	
	printf("The size:%d\n", list_size(list));


	if(list_find_node(list,3,compare_objects)!=NULL){
		printf("pame ligo\n");
	}
	int tryfind= list_find_node(list,6,compare_objects)->value;
	printf("%d",tryfind);

	list_insert_next(list,list_find_node(list,4,compare_objects), 5);
	
	// ListNode node= list_first(list);
	// for(int i=0;i<list_size(list);i++){
	// 	printf("The %d element is:%d \n",i,node->value);
	// 	node=list_next(list,node);
	// }

	list_insert_previous(list,list_find_node(list,4,compare_objects), 3);
	// ListNode node1= list_first(list);
	// for(int i=0;i<list_size(list);i++){
	// 	printf("The %d element is:%d \n",i,node1->value);
	// 	node1=list_next(list,node1);
	// }

	list_insert_previous(list,list_find_node(list,5,compare_objects), 100);
	ListNode node1= list_first(list);
	for(int i=0;i<list_size(list);i++){
		printf("The %d element is:%d \n",i,node1->value);
		node1=list_next(list,node1);
	}

	printf("the last node %d",list_node_value(list,list_last(list)));

	return 0;
}




