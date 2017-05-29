#include <stdio.h>

#include <stdlib.h>

#include <string.h>



int main(const int argc, const char *argv[]){

    struct packet {

        int x;

        char y[255];

        float z;

    } a;

    FILE *file;

    int i = 0;

    int j = -1;

    char filename[255];

    strcpy(filename, *argv[1]);

    while(){

        if (!(file = fopen(filename, "rb"))){

            printf("File name: ");

            scanf("%s", filename);

        }

        else break;

    }

    while (fread(&a, sizeof(struct packet), 1, file)){

        printf(" %d  %s  %f \n", a.x, a.y, a.z);

        i++;

    }

    printf("File size = %d byte\n", sizeof(struct packet)*i);

    fclose(file);

    return 0;

}
