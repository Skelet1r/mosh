#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024
#define ERROR_CODE 1

struct word_item {
    char *word;
    struct word_item *next;
};

static struct word_item* add_word(struct word_item** head, struct word_item** tail, char* command);
static void print_list(struct word_item* head);
static void delete_list(struct word_item** head);
static char* read_command(struct word_item** head, struct word_item** tail);
static void allocation_error(char* command);
static struct word_item* build_list(struct word_item** head, struct word_item** tail, char* command);

int main()
{
    while (1) {
        printf("> ");
        
        struct word_item* head;
        struct word_item* tail;
        char* command;
        
        head = NULL;
        tail = NULL;
        command = NULL;

        command = read_command(&head, &tail);
        if (command) {
            build_list(&head, &tail, command);
            print_list(head);
            delete_list(&head);
            free(command);
            head = NULL;
        }
    }
    return 0;
}

static void allocation_error(char* word)
{
    if (!word) {
        printf("ERROR: allocation error.\n");
        exit(ERROR_CODE);
    }
}

static struct word_item* add_word(struct word_item** head, struct word_item** tail, char* command)
{
    struct word_item* node_word;

    node_word = malloc(sizeof(struct word_item));
    if (!node_word) {
        printf("ERROR: An error while mallocing memory.\n");
    }
    node_word->word = strdup(command);
    node_word->next = NULL;

    if (*head == NULL) {
        *head = node_word;
        *tail = node_word;
    } else {
        (*tail)->next = node_word;
        *tail = node_word;
    }
    return node_word;
}

static void print_list(struct word_item* head)
{
    struct word_item* curr; 
    curr = head;

    while (curr != NULL) {
        printf("[%s]\n", curr->word);
        curr = curr->next;
    }
}

static void delete_list(struct word_item** head)
{
    struct word_item* curr;
    curr = *head;

    while (curr != NULL) {
        struct word_item* next;
        next = curr->next;
        free(curr->word);
        free(curr);
        curr = next;
    }
}

static char* read_command(struct word_item** head, struct word_item** tail)
{
    int bufsize, position, c;
    char* command; 

    bufsize = BUF_SIZE;
    position = 0;
    command = malloc(sizeof(char) * bufsize);

    allocation_error(command);

    while (1) {
        c = getchar();

        if (c == EOF || c == '\n') {
            command[position] = '\0';
            return command;
        } else {
            command[position] = c;
        }
        position++;
        
        if (position >= bufsize) {
            bufsize += BUF_SIZE;
            command = realloc(command, bufsize);
            allocation_error(command);
        }
    }
    return command;
}

static struct word_item* build_list(struct word_item** head, struct word_item** tail, char* command)
{
    char* word;
    word = strtok(command, " ");

    while (word != NULL) {
        add_word(head, tail, word);
        word = strtok(NULL, " ");
    }
    return *head;
}
