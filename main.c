#include <alloca.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "includes/word_item.h"
#include "includes/helpers.h"
#include "includes/commands_parser.h"
#include "includes/exec_commands.h"

int main()
{
    while (1) {
        printf("> ");
        
        struct word_item* head;
        struct word_item* tail;
        
        head = NULL;
        tail = NULL;

        read_command(&head, &tail);
        exec_commands(&head);
        delete_list(&head);
        head = NULL;
    }
    return 0;
}
