#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "includes/word_item.h"
#include "includes/helpers.h"

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

static int check_chdir(const char* dir_path, struct word_item** curr)
{
    int chdir_result;
    chdir_result = chdir(dir_path);
    if (chdir_result == -1) {
        perror("chdir");                
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}

static int exec_cd(struct word_item** head)
{
    struct word_item* curr;
    int i;

    i = 0;
    curr = *head;
    
    if (!strcmp(curr->word, "cd")) {
        while (curr != NULL) {
            if (i == 0) {
                if (curr->next == NULL) {
                    const char* home_path = getenv("HOME");
                    check_chdir(home_path, &curr);
                }
            }

            if (i == 1) {
                const char* dir_path = curr->word;
                check_chdir(dir_path, &curr);
            }

            if (i == 2) {
                return SUCCESS_CODE;
            }
            i++;
            curr = curr->next;
        }
    }
    return ERROR_CODE;
}

void exec_commands(struct word_item** head)
{
    char** cmdline; 
    int pid, cd;
    
    cd = exec_cd(head);
    if (cd == -1) {
        return;
    }

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
