#include <stdbool.h>
#include <stdbool.h> 

// Pointer to an object of any type
typedef void* Pointer;

//Function pointer that compares two elements a and b
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Function pointer that destroys a value
typedef void (*DestroyFunc)(Pointer value);

// Pointers to the structs
typedef struct list* List;
typedef struct list_node* ListNode;


// Creates and returns a new list
List list_create(CompareFunc compare,DestroyFunc destroy_value);

//Returns the number of elements in the list
int list_size(List list);

// Adds a new node after the last node with the value
void list_insert(List list, Pointer value);

//Removes the given value
void list_remove(List list, Pointer value);

//Functions for the first and last node of the list
ListNode list_first(List list);
ListNode list_last(List list);

// Functions  to access a node
ListNode list_next(List list, ListNode node);
ListNode list_previous(List list, ListNode node);

//Returns the value of the given node
Pointer list_node_value(List list, ListNode node);

// Finds the node that is equivalent to value
ListNode list_find_node(List list, Pointer value);

//Destroy the list
void list_destroy(List list);