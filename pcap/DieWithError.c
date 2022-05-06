#include<stdio.h>
#include<stdlib.h>
#include"DieWithError.h"

void DieWithError(const char* errorMessage)
{
    perror(errorMessage);
    exit(1);
}
