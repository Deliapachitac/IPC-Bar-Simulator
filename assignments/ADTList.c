///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT List 
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ADTList.h"


struct list {
	ListNode dummy;				// χρησιμοποιούμε dummy κόμβο, ώστε ακόμα και η κενή λίστα να έχει έναν κόμβο.
	ListNode last;				// δείκτης στον τελευταίο κόμβο, ή στον dummy
	int size;					// μέγεθος, ώστε η list_size να είναι Ο(1)
	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο της λίστας.
	CompareFunc compare;
};

struct list_node {
	ListNode next;		// Δείκτης στον επόμενο
	ListNode previous;	// Δείκτης στον προηγουμενο
	Pointer value;		// Η τιμή που αποθηκεύουμε στον κόμβο
};



List list_create(CompareFunc compare) {//DestroyFunc destroy_value
	// Πρώτα δημιουργούμε το stuct
	List list = malloc(sizeof(*list));
	list->size = 0;
	list->compare = compare;
	//list->destroy_value = destroy_value;

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

void list_insert(List list,  Pointer value) {
	// Αν το node είναι NULL απλά εισάγουμε μετά τον dummy κόμβο!
	// Αυτή ακριβώς είναι η αξία του dummy, δε χρειαζόμαστε ξεχωριστή υλοποίηση.
	if (list->last == NULL){
		list->last = list->dummy;
		return ;
	}

	// Δημιουργία του νέου κόμβου
	ListNode new = malloc(sizeof(*new));
	new->value = value;

	// Σύνδεση του new ανάμεσα στο node και το node->next
	new->next = list->last->next;
	list->last->next = new;
	new->previous = list->last;
	list->last = new;

	// Ενημέρωση των size 
	list->size++;

		
}


void list_remove(List list, ListNode node) {
	
	if (node == NULL) {
		node = list->dummy;
		return;
	}

	if( list_size(list) == 1){
		list->dummy->next =NULL;
		list->last = NULL;
		
	}else if( list_first(list) == node){

		list->dummy->next=node->next;	
		node->next->previous=list->dummy;
	}
	else if(list_last(list) == node){

		node->previous->next=NULL;
		list->last=node->previous;
	}else{

		node->previous->next=node->next;
		node->next->previous=node->previous;
	}

	if (list->destroy_value != NULL)
		list->destroy_value(node->value);

	
	free(node);
	list->size--;
	
}


// Διάσχιση της λίστας

ListNode list_first(List list) {
	// Ο πρώτος κόμβος είναι ο επόμενος του dummy.
	if (list->dummy->next == list->dummy)
		return list->dummy;		// κενή λίστα
	else
		return list->dummy->next;
}

ListNode list_last(List list) {
	// Προσοχή, αν η λίστα είναι κενή το last δείχνει στον dummy, εμείς όμως θέλουμε να επιστρέψουμε NULL, όχι τον dummy!

	if (list->last == list->dummy)
		return list->dummy;		// κενή λίστα
	else
		return list->last;
}

ListNode list_next(List list, ListNode node) {
	assert(node != NULL);	// Ensure node is not null
	return node->next;
}

ListNode list_previous(List list, ListNode node) {
	assert(node != NULL);	 // Ensure node is not null
	return node->previous;
}

Pointer list_node_value(List list, ListNode node) {
	assert(node != NULL);	// Ensure node is not null
	return node->value;
}

ListNode list_find_node(List list, Pointer value) {
	// διάσχιση όλης της λίστας, καλούμε την compare μέχρι να επιστρέψει 0
	printf("mphke");
	for (ListNode node = list->dummy->next; node != NULL; node = node->next){
		if (list->compare(value, node->value) == 0){
			return node;		// βρέθηκε
			
		}
		
	}
	return NULL;	// δεν υπάρχει
}


// int main(void){

// 	List list= list_create();

// 	printf("The size:%d\n", list_size(list));

// 	for (int i = 0; i < 10; i=i+2)
// 	{
// 		list_insert(list, i);
// 		printf("the i is : %d    ",i);
// 	}
	
// 	printf("The size:%d\n", list_size(list));


// 	if(list_find_node(list,2,compare_objects)!=NULL){
// 		printf("pame ligo\n");
// 	}
// 	// int tryfind= list_find_node(list,2,compare_objects)->value;
// 	// printf("%d",tryfind);
// 	ListNode new=list_find_node(list,2,compare_objects);
// 	if(new ==NULL){
// 		printf("pame ligo\n");
// 	}
	
// 	list_insert(list, 5);
// 	list_remove(list,new);
	
// 	ListNode node= list_first(list);
// 	for(int i=0;i<list_size(list);i++){
// 		printf("The %d element is:%d \n",i,node->value);
// 		node=list_next(list,node);
// 	}

// 	// list_insert_previous(list,list_find_node(list,4,compare_objects), 3);
// 	// ListNode node1= list_first(list);
// 	// for(int i=0;i<list_size(list);i++){
// 	// 	printf("The %d element is:%d \n",i,node1->value);
// 	// 	node1=list_next(list,node1);
// 	// }

// 	// list_insert_previous(list,list_find_node(list,5,compare_objects), 100);
// 	// ListNode node1= list_first(list);
// 	// for(int i=0;i<list_size(list);i++){
// 	// 	printf("The %d element is:%d \n",i,node1->value);
// 	// 	node1=list_next(list,node1);
// 	// }

// 	// printf("the last node %d",list_node_value(list,list_last(list)));

// 	return 0;
// }




