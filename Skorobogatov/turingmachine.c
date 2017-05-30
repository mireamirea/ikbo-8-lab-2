/*
  main.c
  turingmachine

  Created by Денис Скоробогатов on 23.03.17.
  Copyright © 2017 Denis Skorobogatov. All rights reserved.
*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LENGTH 256

/* file */
static FILE* fp;

/* command types */
typedef enum {
    command,
    comment,
    begin,
    end
} type;

typedef enum {
    false,
    true
} bool;

/* memory cell */
typedef struct _element {
    int value;
    struct _element* next;
    struct _element* prev;
} element_t;

element_t* movr(element_t*);
element_t* movl(element_t*);
element_t* inc(element_t*);
element_t* dec(element_t*);
element_t* print(element_t*);
element_t* printc(element_t*);
element_t* get(element_t*);

/* command or task */
typedef struct _task{
    element_t* (*command)(element_t*);
    struct _task* left;
    struct _task* right;
    struct _task* prev;
    type type;
} task_t;

/* pointer to a command function */
typedef element_t* (*Task)(element_t*);

static const Task CommandFunction[] = {
    &movr,
    &movl,
    &inc,
    &dec,
    &print,
    &printc,
    &get
};

enum {
    MOVR,
    MOVL,
    INC,
    DEC,
    PRINT,
    PRINTC,
    GET
};

char* readline()
{
    char *command = malloc(sizeof(char));
    int c; /* think of it as a char */
    int i = 1;
    while(!(((c = getc(fp)) == EOF) || c == '\n'))
    {
        if (!((char)c == ' ' || (char)c == '\t'))
        {
            command = realloc(command, (i+1)*sizeof(char));
            command[i-1] = toupper((char)c);
            i++;
        }
    }
    if (c == EOF || *command == '\0')
        return NULL;
    command[i-1] = '\0';
    return command;
}

/* returns a pointer to the command function */
Task swcommand(char* command, task_t* task)
{
    if(!strncmp(command, "MOVR", 4))
        return CommandFunction[MOVR];
    if(!strncmp(command, "MOVL", 4))
        return CommandFunction[MOVL];
    if(!strncmp(command, "INC", 3))
        return CommandFunction[INC];
    if(!strncmp(command, "DEC", 4))
        return CommandFunction[DEC];
    if(!strncmp(command, "PRINT", 5))
        return CommandFunction[PRINT];
    if(!strncmp(command, "PRINTC", 6))
        return CommandFunction[PRINTC];
    if(!strncmp(command, "GET", 3))
        return CommandFunction[GET];
    if(!strncmp(command, "BEGIN", 5))
    {
        task->type = begin;
        return NULL;
    }
    if(!strncmp(command, "END", 3))
    {
        task->type = end;
        return NULL;
    }
    task->type = comment;
    return NULL;
}

/* task_t constructor */
task_t* newtask(void)
{
    task_t* task = malloc(sizeof(task_t));
    task->command = NULL;
    task->left = NULL;
    task->right = NULL;
    task->prev = NULL;
    task->type = command;
    return task;
}

/* task_t initializer */
task_t* createtask(char* command, task_t* this)
{
    task_t* task = newtask();
    task->command = swcommand(command, task);
    if (this == NULL)
        return task;
    switch (this->type)
    {
        case begin:
            task->prev = this;
            this->right = task;
            break;
        case end:
            task->prev = this->prev->prev;
            this->prev->left = task;
            break;
        default:
            this->left = task;
            task->prev = this->prev;
    }
    return task;
}

/* generates command tree */
task_t* gentree()
{
    task_t* task;
    task_t* firsttask;
    char* command = readline();
    if (command == NULL)
        return NULL;
    task = createtask(command, NULL);
    firsttask = task;
    while((command = readline()) != NULL)
        task = createtask(command, task);
    return firsttask;
}

/* runs command tree */
void run(task_t* task, element_t* element)
{
    if (task == NULL)
        return;
    if (task->type == comment)
        return run(task->left, element);
    if (task->type == command)
    {
        element = task->command(element);
        return run(task->left, element);
    }
    if (task->type == begin)
    {
        if (element->value == 0)
            return run(task->left, element);
        else
            return run(task->right, element);
    }
    if (task->type == end)
    {
        if (element->value != 0)
            return run(task->prev, element);
        else
            return run(task->prev->left, element);
    }

}

/* element_t constructor */
element_t* element(void)
{
    element_t* el = malloc(sizeof(element_t));
    el->value = 0;
    el->next = NULL;
    el->prev = NULL;
    return el;
}

/* movl */
element_t* movl(element_t* this)
{
    element_t *el;
    if (this->prev == NULL)
    {
        el = element();
        el->next = this;
    } else {
        el = this->prev;
    }
    return el;
}

/* movr */
element_t* movr(element_t* this)
{
    element_t* el;
    if (this->next == NULL)
    {
        el = element();
        el->prev = this;
    } else {
        el = this->next;
    }
    return el;
}

/* inc */
element_t* inc(element_t* this)
{
    this->value += 1;
    if (this->value >= 256)
    {
        printf("Buffer overflow\n");
        this->value = 0;
    }
    return this;
}

/* dec */
element_t* dec(element_t* this)
{
    this->value -= 1;
    if (this->value <= -1)
    {
        printf("Buffer overflow\n");
        this->value = 0;
    }
    return this;
}

/* print */
element_t* print(element_t* this)
{
    printf("%d\n", this->value);
    return this;
}

/* printc */
element_t* printc(element_t* this)
{
    printf("%c\n", (char)this->value);
    return this;
}

/* reads int value from 0 to 255 */
int _get(element_t* this)
{
    char c;
    int result = 0;
    int k = 0;
    int overflow = 0;
    int isnumber = 1;
    while ((c = getchar()) != '\n')
    {
        if (k < MAX_LENGTH)
        {
            if (c < '0' || c > '9')
                isnumber = 0;
            if (result * 10 + (int)(c-'0') <= MAX_LENGTH)
            {
                result *= 10;
                result += (int)(c - '0');
            } else {
                if (overflow == 0)
                    printf("Buffer overflow. Setting zero\n");
                result = 0;
                overflow = 1;
            }
            k++;
        }
    }
    if (k == 0 || isnumber == 0)
        return 0;
    (this->value) = result;
    return 1;
}

/* get */
element_t* get(element_t* this)
{
    while ((_get(this)) != 1)
    {
        printf("TRY AGAIN\n");
    }
    return this;
}

/* file constructor */
FILE* file(const char* filename)
{
    fp = fopen(filename, "r");
    return fp;
}

/* check syntax */
bool syntax()
{
    unsigned int line = 0;
    int loops = 0;
    bool errordisplayed = false;
    char* cmd;
    task_t* task = newtask();
    bool result = true;
    while(!feof(fp))
    {
        cmd = readline();
        if (cmd == NULL && !feof(fp))
        {
            printf("Error at line %d.\nNo empty lines allowed\n--------------------\n", line);
            result = false;
            line++;
            continue;
        }
        if (cmd == NULL && feof(fp))
            break;
        swcommand(cmd, task);
        if (task->type == comment && *cmd != '*')
        {
            printf("Syntax error at line %d.\nAll comments must start with \"*\"\n--------------------\n", line);
            result = false;
        }
        if (task->type == begin)
            loops++;
        if (task->type == end)
            loops--;
        if (loops < 0 && errordisplayed == false)
        {
            result = false;
            errordisplayed = true;
            printf("Loop error at line %d.\n BEGIN and END commands are misplaced\n--------------------\n", line);
        }
        line++;
        task->type = command;
    }
    if (loops != 0)
    {
        printf("Loop error. Number of BEGIN and END commands does not match\n--------------------\n");
        result = false;
    }
    fseek(fp, 0, SEEK_SET);
    return result;
}

int main(int argc, const char * argv[]) {
    element_t* el = element();
    task_t* task;
    if (argc == 1) /* debug session on my mac */
        fp = file("/Users/denisskorobogatov/Desktop/MTU/turingmachine/turingmachine/cmd");
    else
        fp = file(argv[1]); /* first arg is a directory to the command file */
     if (!syntax())
        return 0;
    task = gentree();
    if (task == NULL)
        printf("Command file is empty\n");
    run(task, el);
    return 0;
}
