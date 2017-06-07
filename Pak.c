#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

#define K 255
#define S 255
#define F 255

int x = 0;
int y = 1;
int *tape;

int movx(){
    if (x == 0){
        int i;
        int* temp;
        y++;
        temp = (int*)realloc(tape,y * sizeof(int));
        if(temp != NULL){
            for (i = y - 1; i != 0; i--)
                temp[i]=temp[i-1];
            temp[0] = 0;
            tape = temp;
        }
        else
            return 1;
    }
    else y--;
    return 0;
}

int movr(){
    if (x == (y - 1)){
        x++;
        y++;
        int* temp;
        temp = (int*) realloc (tape, sizeof(int) * y);
        if(temp != NULL){
            temp[y-1] = 0;
            tape = temp;
        }
        else
            return 1;
    }
    else x++;
    return 0;
}

int inc () {
    if(tape[x] < K)
        tape[x]++;
    else
        tape[x] = 0;
    return 0;
}

int dec () {
    if(tape[x] > 0)
        tape[x]--;
    else
        tape[x] = K;
    return 0;
}

int commands(char **a, int left, int right) {
    int i, temp = 0;
    for (i=left; i<right; i++) {
        if (strcmp(a[i], "inc")==0) inc();
        else if (strcmp(a[i], "dec")==0) dec();
        else if (strcmp(a[i], "movr")==0) movr();
        else if (strcmp(a[i], "movx")==0) movx();
        else if (strcmp(a[i], "get")==0){
            printf("\nEnter symbol:\n");
            scanf ("%i", &tape[x]);
        }
        else if (strcmp(a[i], "print")==0) printf("\n%d", tape[x]);
        else if (strcmp(a[i], "printc")==0) printf("\n%c", tape[x]);
        else if (strcmp(a[i], "begin")==0) temp=i+1;
        else if (strcmp(a[i], "end")==0) {
            while (tape[x]!=0) commands(a, temp, i);
        }
    }

    return 0;
}

char* space (char* str){
    int i, j;
    char* a;
    a = (char*) malloc (S*sizeof(char));
    for(i = j = 0; str[i] != '\0'; i++)
        if(isalnum(str[i]))
            a[j++] = str[i];
    a[j] = '\0';
    return a;
}

int main(void){
    int N = 0, i = 0;
    char **a, str[K];
    FILE *file = NULL;
    char *fname=(char *)malloc(F);
    printf ("Enter file name\n");
    scanf("%s",fname);
    file=fopen(fname,"r");

    while (file == NULL) {
        printf("Error opening the file\n Enter new file name (Example: file.dat or C:\\\\Users\\\\Desktop\\\\file.dat)\n");
        scanf("%s",fname);
        file=fopen(fname,"r");
    }

    a=(char**)malloc((N+1)*sizeof(char*));
    while(fgets(str, K, file)) {
        if(str[0] != '*'){
            a[N] = space(str);
            N++;
            a = (char**)realloc(a,(N+1)*sizeof(char*));
        }
    }

    tape = (int*) malloc (sizeof(int));
    tape[x]=0;
    commands(a, 0, N);

    fclose(file);
    free(a);
    printf("\n Tape : ");
    for(i = 0; i < y; i++)
        printf(" %d", tape[i]);
    printf("\n");
    return 0;
}
