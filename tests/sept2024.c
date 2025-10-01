#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main () {
  int n;
    if ( n=(fork() && fork()) ) {
      // printf("%d\n", n);
      printf("hello 1\n");  
    } else {

      printf("world 2\n");
    }

    if((fork() * fork()) ){
        printf("hello\n");
    } else {
        printf("world\n");
    }

    return 0;
}

// #include <stdio.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <sys/types.h>

// int main() {
    
//     if (fork() != fork()) {
//         printf("hello1\n");
//     }
    
//     fork(); fork(); printf("world\n"); printf("hello\n");
//     return 0;
// }