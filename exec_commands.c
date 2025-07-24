#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "includes/word_item.h"

static int count_list_elements(struct word_item* head)
{
    int counter;
    counter = 0;

    while (head != NULL) {
        counter++;             
        head = head->next;
    }
    return counter;  
}

static char** list_to_array(struct word_item** head)
{
    const int size_of_array = count_list_elements(*head);
    struct word_item* curr;
    char** cmdline; 
    int i;

    i = 0;
    cmdline = malloc((size_of_array + 1) * sizeof(char*));
    curr = *head;

    while (curr != NULL) {
        cmdline[i] = curr->word;
        i++;
        curr = curr->next;
    }
    cmdline[i] = NULL;
    return cmdline;
}

void exec_commands(struct word_item** head)
{
    char** cmdline; 
    int pid;

    cmdline = list_to_array(head);
    pid = fork();

    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        execvp(cmdline[0], cmdline);
    }
    wait(NULL);
    free(cmdline);
}
