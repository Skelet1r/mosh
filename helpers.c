#include <stdio.h>
#include <stdlib.h>

#include "includes/helpers.h"

void allocation_error(char* word)
{
    if (!word) {
        printf("ERROR: allocation error.\n");
        exit(ERROR_CODE);
    }
}

