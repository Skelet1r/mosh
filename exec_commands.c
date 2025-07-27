#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
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
        return 1;
    }
    return 0;
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
                return 0;
            }
            i++;
            curr = curr->next;
        }
    }
    return 1;
}

static char** bg_mode(char** cmdline, int cmdline_len)
{
    const int len_wo_amper = --cmdline_len;

    if (!strcmp(cmdline[len_wo_amper], "&")) {
        char** cmdline_wo_amp;
        cmdline_wo_amp = malloc((len_wo_amper) * sizeof(char*));
        if (cmdline_wo_amp == NULL) {
            perror("ERROR: allocation error\n");
            return NULL;
        }

        for (int i = 0; i < len_wo_amper; i++) {
            cmdline_wo_amp[i] = cmdline[i];
        }
        return cmdline_wo_amp;
    }    
    return NULL;
}

void exec_commands(struct word_item** head)
{
    char** cmdline;
    char** bg_mode_res;
    int p, pid, cd, cmdline_len;
   
    p = 0;
    cd = exec_cd(head);
    if (cd == -1) {
        return;
    }

    cmdline_len = count_list_elements(*head);
    cmdline = list_to_array(head);
    
    bg_mode_res = bg_mode(cmdline, cmdline_len);
    if (bg_mode_res != NULL) {
        pid = fork();

        if (pid == -1) {
            perror("fork");
            return;
        }

        if (pid == 0) {
            execvp(bg_mode_res[0], bg_mode_res);
            perror("execvp");
        }
        free(bg_mode_res);
        return;
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        execvp(cmdline[0], cmdline);
        perror("execvp");
    }

    do {
        p = wait(NULL);
    } while (p != pid);
    free(cmdline);
}
