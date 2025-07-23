#include <stdio.h>
#include <stdlib.h>
#include "includes/helpers.h"
#include "includes/word_item.h"

static void reset_word(char** command, int* i, struct word_item** head, struct word_item** tail);
static char* parse_quotes(char type_of_quotes);

void read_command(struct word_item** head, struct word_item** tail)
{
    char* command;    
    char c;
    int i;
   
    i = 0;
    command = malloc(BUF_SIZE);
    allocation_error(command); 

    while ((c = getchar()) != '\n') {
        if (c == ' ') {
            reset_word(&command, &i, head, tail);
        } else if (c =='"' || c == '\'') {
            char* tmp;
            tmp = parse_quotes(c);        
            if (tmp != NULL) {
                add_word(head, tail, tmp);
                free(tmp);
            } else {
                free(command);
                return;
            }
        } else { 
            command[i++] = c;
        }
    }   
    if (i > 0) {
        reset_word(&command, &i, head, tail);
    }
}

static char* parse_quotes(char type_of_quotes)
{    
    char* word = malloc(BUF_SIZE);
    allocation_error(word);
    int j;
    char c;
    
    j = 0;
    while ((c = getchar()) != type_of_quotes) {
        if (c == '\n') {
            printf("ERROR: unmatched quotes\n");
            free(word);
            return NULL;
        }
        word[j] = c;
        j++;
    }
    word[j] = '\0';
    return word;
}

static void reset_word(char** command, int* i, struct word_item** head, struct word_item** tail)
{    
    (*command)[(*i)] = '\0';
    add_word(head, tail, *command);
    free(*command);
    *command = malloc(BUF_SIZE);
    allocation_error(*command);
    *i = 0;
}
