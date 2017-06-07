#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *tampe;
int sz = 1;
int col = 0;

int movl(void){
    if (col == 0){
        int i;
        int* tpm;
        sz++;
        tpm = (int*) realloc (tampe, sizeof(int) * sz);
        if(tpm != NULL){
            for (i = sz - 1; i != 0; i--)
                tpm[i]=tpm[i-1];
            tpm[0] = 0;
            tampe = tpm;
        }
        else
            return 1;
    }
    else col--;
return 0;
}

int movr(void){
    if (col == (sz - 1)){
        int* tpm;
        sz++;
        col++;
        tpm = (int*) realloc (tampe, sizeof(int) * sz);
        if(tpm != NULL){
            tpm[sz-1] = 0;
            tampe = tpm;
        }
        else
            return 1;
    }
    else col++;
return 0;
}

int inc (void) {
    if(tampe[col] < 255)
        tampe[col]++;
    else
        tampe[col] = 0;
return 0;
}

int dec (void) {
    if(tampe[col] > 0)
        tampe[col]--;
    else
        tampe[col] = 255;
return 0;
}

int commands (char** a, int first, int last){
    int i, tpm = 0;
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
            printf("\n%d", tampe[col]);
        else if (strcmp(a[i], "get") == 0){
                printf("\nEnter symbol:");
            tampe[col] = getchar();
        }
        else if (strcmp(a[i], "printc") == 0)
            printf("\n%c", tampe[col]);
        else if (strcmp(a[i], "begin") == 0)
             tpm=i+1;
        else if (strcmp(a[i], "end") == 0)
            {
                while (tampe[col] != 0)
                    commands(a, tpm, i);
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
    char **d;
    int index = 0, i = 0;;
    char *fname = NULL;

    FILE * fpe;
    fpe = fopen("file.txt", "r");
    while (fpe == NULL){
        printf(" Specify the path to the file:");
        scanf("%s", fname);
        fpe=fopen(fname, "rb");
    }

    d=(char **) malloc ((index+1)*sizeof(char *));
    while(fgets(s, 255, fpe)){
        if(s[0] != '*'){
            d[index] = delprobel(s, d[index]);
            index++;
            d = (char**) realloc (d,(index+1)*sizeof(char*));
        }
    }
    fclose(fpe);

    tampe = (int*) malloc (sizeof(int));
    tampe[col]=0;
    commands(d, 0, index);

    for(i = 0; i < index; i++)
        free(d[index]);
    free(d);
    printf("\n Tape : ");
    for(i = 0; i < sz; i++)
        printf("  %d", tampe[i]);
    return 0;
}
