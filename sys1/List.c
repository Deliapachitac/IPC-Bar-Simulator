#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "List.h"

struct list {
    ListNode empy;             // An empy node always exists when the list is empty 
    ListNode last;              
    int size;       
    DestroyFunc destroy_value;  //A pointer to a function that destroys an element
    CompareFunc compare;        // Function pointer to compare two elements
};

struct list_node {
    ListNode next;      // Pointer to the next node
    ListNode previous;  //Pointer to the previous node
    Pointer value;      // The value of the node
};

List list_create(CompareFunc compare, DestroyFunc destroy_value ) {
    //Allocate memory for the list
    List list = malloc(sizeof(*list));
    list->size = 0;                
    list->compare = compare;
	list->destroy_value=destroy_value;

    // Create the empy node
    list->empy = malloc(sizeof(*list->empy));
    list->empy->next = NULL;      
    list->empy->previous = NULL;  //The previous of the empy will always be NULL

    //The last points to the empy node
    list->last = list->empy;      

    return list;                  
}

int list_size(List list) {
    return list->size;          
}

void list_insert(List list, Pointer value) {
    // Create the new node that we will add to the list 
    ListNode new = malloc(sizeof(*new));
    new->value = value;            

    // Connect the new node with the last's next node 
    new->next = list->last->next;  
    list->last->next = new;      
    
    //Connect the last node with the new
    new->previous = list->last;    // The previous of the new node is the last 
    list->last = new;             //The last node will point to the new

    // Increase the size of the list
    list->size++;
}


void list_remove(List list, Pointer value) {

    ListNode node= list_find_node(list, value);

    //If the node is NULL do nothing
	if (node == NULL) {
        return;
    }

    // If the list contains only one node
    if (list_size(list) == 1) {
        list->empy->next = NULL; // The next of the empy node will be NULL because we will erase the node of the list 
        list->last = NULL;        //The last will no longer point to the node of the list 
    } else if (list_first(list) == node) {
        //If the node we will remove is the first node
        list->empy->next = node->next;   //The empy next has to point to the second node
        node->next->previous = list->empy; //The second node has to point to the empy 
    } else if (list_last(list) == node) {
        // If the node to be removed is the last node
        node->previous->next = NULL;     //The previous node of the node we want to remove has to point to Null because it is the last node
        list->last = node->previous;      // The last node has to point to the previous of the node we will remove
    } else {
        // If the node is in the middle
        node->previous->next = node->next; //The previous node of the node we want to remove has to point to the next of the node we will remove  
        node->next->previous = node->previous; // The next node of the node we want to remove has to point to the previous of the node we will remove  
    }

    // Call the destroy function and free the node
    if (list->destroy_value != NULL) {
        list->destroy_value(node);
    }
    free(node);

    // Decrease the size of the list                        
    list->size--;  
}

ListNode list_first(List list) {
    
    if (list->empy->next == list->empy)
        return NULL;  // Return NULL if list is empty
    else
        return list->empy->next; //The first node is the one after the empy node
}

ListNode list_last(List list) {
    
    if (list->last == list->empy)
        return NULL;  // Return NULL if list is empty
    else
        return list->last;  
}


ListNode list_next(List list, ListNode node) {
    if(node == NULL) //if node is null then the next doesnt exist
        exit(1);
    return node->next;    
}

ListNode list_previous(List list, ListNode node) {
    if(node == NULL) //if node is null then the previous node doesnt exist
        exit(1);
    return node->previous; 
}


Pointer list_node_value(List list, ListNode node) {
    if(node == NULL) //if node is null it cant have a value
        exit(1);
    return node->value; 

}


ListNode list_find_node(List list, Pointer value) {
    //Traverse the entire list calling the compare function until it returns 0
    for (ListNode node = list->empy->next; node != NULL; node = node->next) {
        if (list->compare(value, node->value) == 0) {
            return node; 
        }
    }
	// Node not found
    return NULL;  
}

void list_destroy(List list) {
    // Iterate over the nodes of the list and free each one
    ListNode node = list->empy;
	while (node != NULL) {				
		ListNode next = node->next;		// Get the next node before freeing the node

		// Call the destroy function if it exists
		if (node != list->empy && list->destroy_value != NULL)
			list->destroy_value(node->value);

        // free the node and initialize the next node that we saved before
		free(node); 
		node = next;
	}

	//free the list struct itself
	free(list);
}