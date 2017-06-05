#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *lenta;
int size = 1;
int loc = 0;

int movl(void){
    if (loc == 0){
        int i;
        int* tmp;
        size++;
        tmp = (int*) realloc (lenta, sizeof(int) * size);
        if(tmp != NULL){
            for (i = size - 1; i != 0; i--)
                tmp[i]=tmp[i-1];
            tmp[0] = 0;
            lenta = tmp;
        }
        else
            return 1;
    }
    else loc--;
return 0;
}

int movr(void){
    if (loc == (size - 1)){
        int* tmp;
        size++;
        loc++;
        tmp = (int*) realloc (lenta, sizeof(int) * size);
        if(tmp != NULL){
            tmp[size-1] = 0;
            lenta = tmp;
        }
        else
            return 1;
    }
    else loc++;
return 0;
}

int inc (void) {
    if(lenta[loc] < 255)
        lenta[loc]++;
    else
        lenta[loc] = 0;
return 0;
}

int dec (void) {
    if(lenta[loc] > 0)
        lenta[loc]--;
    else
        lenta[loc] = 255;
return 0;
}

int commands (char** a, int first, int last){
    int i, tmp = 0;
    for (i = first; i < last; i++){
        if (strcmp(a[i], "movl") == 0)
            movl();
        else if (strcmp(a[i], "movr") == 0)
            movr();
        else if (strcmp(a[i], "inc") == 0)
            inc();
        else if (strcmp(a[i], "dec") == 0)
            dec();
        else if (strcmp(a[i], "print") == 0)
            printf("\n%d", lenta[loc]);
        else if (strcmp(a[i], "get") == 0){
                printf("\nEnter symbol:");
            lenta[loc] = getchar();
        }
        else if (strcmp(a[i], "printc") == 0)
            printf("\n%c", lenta[loc]);
        else if (strcmp(a[i], "begin") == 0)
             tmp=i+1;
        else if (strcmp(a[i], "end") == 0)
            {
                while (lenta[loc] != 0)
                    commands(a, tmp, i);
            }
        else
            printf("\nerror");
    }

return 0;
}

char* delprobel (char* s, char* a){
    int i, j;
    j = 0;
    a = (char*) malloc (sizeof(char)*6);
    for (i = 0; s[i]; i++){
          if (s[i] != ' ' && s[i] != '\n'){
      a[j]=s[i];
      j++;
      a = (char*) realloc (a,(j+6)*sizeof(char));
      }
    }
    a = (char*) realloc (a, j*sizeof(char));
    return a;
}

int main(){
    char s[255];
    char **a;
    int index = 0, i = 0;;
    char *fname = NULL;

    FILE * f;
    f = fopen("file.txt", "r");
    while (f == NULL){
        printf(" Specify the path to the file:");
        scanf("%s", fname);
        f=fopen(fname, "rb");
    }

    a=(char **) malloc ((index+1)*sizeof(char *));
    while(fgets(s, 255, f)){
        if(s[0] != '*'){
            a[index] = delprobel(s, a[index]);
            index++;
            a = (char**) realloc (a,(index+1)*sizeof(char*));
        }
    }
    fclose(f);

    lenta = (int*) malloc (sizeof(int));
    lenta[loc]=0;
    commands(a, 0, index);

    for(i = 0; i < index; i++)
        free(a[index]);
    free(a);
    printf("\n Tape : ");
    for(i = 0; i < size; i++)
        printf("  %d", lenta[i]);
    return 0;
}
