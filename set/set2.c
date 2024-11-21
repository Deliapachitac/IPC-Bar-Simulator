#include <stdlib.h>
#include <stdio.h>

//initialized data segment must be or a global variable or a static one
int global_var = 42;


void print_fun(){// a text segment can be a function address
    printf("Delia\n");
}

int main() {
    char *stack_v[10];// Stack
    char *heap_v = (char *)malloc(10);// Heap

    printf("text segment %p\n", print_fun);
    printf("initialized data  %p\n", &global_var);
    printf("stack %p\n", &stack_v);
    printf("heap %p\n", &heap_v);

    free(heap_v); 
    return 0;
}
