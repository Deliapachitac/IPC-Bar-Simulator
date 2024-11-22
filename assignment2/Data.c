#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Data.h"


#define BUFFER_SIZE 256


struct table{
    int size ;
    Data* array;
};

// Define a struct to store word and frequency
struct data{
    char word[BUFFER_SIZE];
    int frequency;
};




