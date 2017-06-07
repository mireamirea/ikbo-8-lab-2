#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LENGTH 256


typedef enum {
    command,
    comment,
    begin,
    end
} type;


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


typedef struct _task{
    element_t* (*command)(element_t*);
    struct _task* left;
    struct _task* right;
    struct _task* prev;
    type type;
} task_t;


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

char* readline(FILE *f)
{
    char *command = malloc(sizeof(char));
    int c;
    int i = 1;
    while(!(((c = getc(f)) == EOF) || c == '\n'))
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


task_t* createtask(char* command, task_t* this)
{
    task_t* task = newtask();
    task->command = swcommand(command, task);
    if (this->type == begin && this->right == NULL)
    {
        task->prev = this;
        this->right = task;
    }
    else
        this->left = task;
    if (this->type != begin)
        task->prev = this->prev;
    return task;
}


task_t* gentree(FILE *f)
{
    task_t* task;
    task_t* firsttask;
    char* command;
    task = createtask(readline(f), task);
    firsttask = task;
    while((command = readline(f)) != NULL)
        task = createtask(command, task);
    return firsttask;
}


void run(task_t* task, element_t* element)
{
    if (task->type == command)
    {
        element = task->command(element);
        task = task->left;
        return run(task, element);
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
            return (task->prev->left == NULL) ? NULL : run(task->prev->left, element);
    }

}


element_t* element(void)
{
    element_t* el = malloc(sizeof(element_t));
    el->value = 0;
    el->next = NULL;
    el->prev = NULL;
    return el;
}


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


element_t* print(element_t* this)
{
    printf("%d\n", this->value);
    return this;
}


element_t* printc(element_t* this)
{
    printf("%c\n", (char)this->value);
    return this;
}


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
                    printf("Buffer overflow\n");
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


element_t* get(element_t* this)
{
    while ((_get(this)) != 1)
    {
        printf("TRY AGAIN\n");
    }
    return this;
}


FILE* file(const char* filename)
{
    FILE *fp;
    fp = fopen(filename, "r");
    return fp;
}



int main(int argc, const char * argv[]) {
    element_t *el = element();
    task_t *task;
    FILE *f;
    if (argc == 1)
        f = file("mrx.cmd");
    else
        f = file(argv[1]);
    if (f == NULL)
    {
        printf("No commands in the file\n");
        return 0;
    }
    task = gentree(f);
    run(task, el);
    return 0;
}
