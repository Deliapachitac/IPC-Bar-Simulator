///////////////////////////////////////////////////////////
//
// Implementation of ADT List
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ADTList.h"

struct list {
    ListNode dummy;             // A dummy node, so that even an empty list has one node.
    ListNode last;              // Pointer to the last node, or to the dummy node
    int size;                   // Size of the list
    DestroyFunc destroy_value;  // Function pointer to destroy a list element if needed
    CompareFunc compare;        // Function pointer to compare two elements
};

struct list_node {
    ListNode next;      // Pointer to the next node
    ListNode previous;  // Pointer to the previous node
    Pointer value;      // The value of the node
};

List list_create(CompareFunc compare, DestroyFunc destroy_value ) {
    // Allocate memory for the list structure
    List list = malloc(sizeof(*list));
    list->size = 0;                
    list->compare = compare;
	list->destroy_value=destroy_value;

    // Create the dummy node
    list->dummy = malloc(sizeof(*list->dummy));
    list->dummy->next = NULL;      // The dummy's next is initially NULL
    list->dummy->previous = NULL;  // The dummy's previous is also NULL
    list->last = list->dummy;      // Last points to the dummy node

    return list;                  
}

int list_size(List list) {
    return list->size;          
}

void list_insert(List list, Pointer value) {
    // Create the new node
    ListNode new = malloc(sizeof(*new));
    new->value = value;            // Set the value of the new node

    // Link the new node into the list
    new->next = list->last->next;  // Point new's next to the current last's next
    list->last->next = new;        // Link last's next to new
    new->previous = list->last;    // Link new's previous to last
    list->last = new;              // Update last to point to the new node

    // Update the size of the list
    list->size++;
}


void list_remove(List list, ListNode node) {

    // If node is NULL do nothing
	if (node == NULL) {
        return;
    }

    // If the list only contains the dummy node
    if (list_size(list) == 1) {
        list->dummy->next = NULL; // Remove the link from the dummy to last
        list->last = NULL;        // Update last to NULL
    } else if (list_first(list) == node) {
        // If the node to remove is the first node
        list->dummy->next = node->next;   // Update dummy's next to the second node
        node->next->previous = list->dummy; // Update the second node's previous to dummy
    } else if (list_last(list) == node) {
        // If the node to remove is the last node
        node->previous->next = NULL;      // Update the previous node's next to NULL
        list->last = node->previous;      // Update last to the previous node
    } else {
        // If the node to remove is in the middle
        node->previous->next = node->next; // Link the previous node to the next node
        node->next->previous = node->previous; // Link the next node back to the previous node
    }

    // Call the destroy function 
    // if (list->destroy_value != NULL) {
    //     list->destroy_value(node->value);
    // }

    free(node);                        
    list->size--;                       // Decrease the size of the list
}

ListNode list_first(List list) {
    // The first node is the one after the dummy node.
    if (list->dummy->next == list->dummy)
        return NULL;  // Return NULL for an empty list
    else
        return list->dummy->next; 
}

ListNode list_last(List list) {
    // If the list is empty, return NULL instead of the dummy
    if (list->last == list->dummy)
        return NULL;  // Return NULL for an empty list
    else
        return list->last;  
}


ListNode list_next(List list, ListNode node) {
    assert(node != NULL);  // Ensure node is not null
    return node->next;    
}

ListNode list_previous(List list, ListNode node) {
    assert(node != NULL);  // Ensure node is not null
    return node->previous; 
}


Pointer list_node_value(List list, ListNode node) {
    assert(node != NULL);  // Ensure node is not null
    // if(node == NULL){
    //     return NULL;
    // }
    return node->value; 

}


ListNode list_find_node(List list, Pointer value) {
    // Traverse the entire list, calling compare until it returns 0
    for (ListNode node = list->dummy->next; node != NULL; node = node->next) {
        if (list->compare(value, node->value) == 0) {
            return node; 
        }
    }
	// Node not found
    return NULL;  
}


// int main(void) {
//     List list = list_create();

//     printf("The size: %d\n", list_size(list));

//     for (int i = 0; i < 10; i += 2) {
//         list_insert(list, i);
//         printf("The i is: %d    ", i);
//     }
    
//     printf("The size: %d\n", list_size(list));

//     ListNode new = list_find_node(list, 2, compare_objects);
//     if (new != NULL) {
//         printf("Node found\n");
//     } else {
//         printf("Node not found\n");
//     }
    
//     list_insert(list, 5);
//     list_remove(list, new);
    
//     ListNode node = list_first(list);
//     for (int i = 0; i < list_size(list); i++) {
//         printf("The %d element is: %d\n", i, node->value);
//         node = list_next(list, node);
//     }

//     return 0;
// }
