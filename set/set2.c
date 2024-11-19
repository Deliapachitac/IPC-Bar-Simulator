#include <stdlib.h>
#include <stdio.h>

int global_var = 42;  // Initialized data segment

int main() {
    char *stack_v[10];// Stack
    char *heap_v = (char *)malloc(10);// Heap

    printf("Text segment  %p\n", &main);
    printf("Initialized data segment: %p\n", &global_var);
    printf("Stack: %p\n", &stack_v);
    printf("Heap: %p\n", &heap_v);

    free(heap_v); 
    return 0;
}
