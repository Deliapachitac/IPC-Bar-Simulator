#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main(){
    // int r1=fork();
    // int r2=fork();
    // int r3=fork();

    // printf("not loop : r1 %d\n ",r1);

    // printf("not loop : r1 %d r2 %d,r3 %d\n",r1,r2,r3);
    // if( fork() && fork() || fork()){
    //     printf("loop \n");
    // }

    // if(fork()*fork()){
    //     printf("word\n");
    // }else{
    //     printf("delia\n");
    // }
    int i;
    for( i=5;i>-1;i--){
printf("the value of i %d",i);
    //code
    }
    printf("the value of i %d",i);
}