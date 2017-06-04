#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct machine {

	int *memoryTape;
	long long int size;
	int *position;
	long long int mem;

} T;


int sizeFPOS = 1;

void formString (char *s){
	int i = 0, j;
	while (s[i] != '\0'){
		if ((s[i] == ' ') || (s[i] == '\t')){
			for (j = i; j < strlen(s); j++){
				s[j] = s[j + 1];
			}
		}
		else {
			i++;
		}
	}
}

void init (struct machine *T){

	int i;

	T->mem = 1;
	T->size = 5;
	T->memoryTape = (int *)malloc(T->size * sizeof(int));
	T->position = (int *)malloc(T->mem * sizeof(int));

	*(T->position) = 0;
	for (i = 0; i < 5; i++) {
		*(T->memoryTape + i) = 0;
	}
}

void resizePos (struct machine *T){
	T->mem++;
	T->position = realloc(T->position, T->mem * sizeof(int));
	*(T->position + T->mem) = 0;
}

void resizeTape (struct machine *T){

	int i;

	T->size += 3;
	T->memoryTape = realloc(T->memoryTape, T->size * sizeof(int));
	for (i = (T->size - 3); i < T->size; i++){
		*(T->memoryTape + i) = 0;
	}
	printf("Resize tape: new size = %d\n", T->size);
}

void movl (struct machine *T){

	if (T->size <= ( ((T->mem - 1) * 32767) + *(T->position + T->mem)+1)){
		resizeTape(T);
	}
	if (32767 <= *(T->position + T->mem)){
		resizePos(T);
	}
	(*(T->position + T->mem))++;
	printf("Nove left\n");
}

void movr (struct machine *T){
	if ((((T->mem - 1) * 32767) + *(T->position + T->mem)) == 0){
		printf("Can't go right, this is ZERO cell\n");
	}	else {
			(*(T->position + T->mem))--;
			printf("Move right\n");

		}
}

void inc (struct machine *T){

	if (*(T->memoryTape + (((T->mem - 1) * 32767) + *(T->position + T->mem))) == 255){
		printf("Overflow of cell (Max size = 255)\n");
		*(T->memoryTape + (((T->mem - 1) * 32767) + *(T->position + T->mem))) = 0;
	}
	else {
		(*(T->memoryTape + (((T->mem - 1) * 32767) + *(T->position + T->mem))))++;
		printf("Increment of cell\n");
	}
}

void dec (struct machine *T) {
	if (*(T->memoryTape + (((T->mem - 1) * 32767) + *(T->position + T->mem))) == 0 ){
		printf("Underflow of cell (Min size = 0)\n");
	}
	else {
		(*(T->memoryTape + (((T->mem - 1) * 32767) + *(T->position + T->mem))))--;
		printf("Decrement of cell\n");
	}
}

void print (struct machine *T) {

	printf("Value of cell[%d] = ", (((T->mem - 1) * 32767) + *(T->position + T->mem)));
	printf("%d\n", *(T->memoryTape + (((T->mem - 1) * 32767) + *(T->position + T->mem))));
}

void get (struct machine *T){

	int tmp;

	printf(" - Enter value of cell: \n");
	scanf("%d", &tmp);
	if ((tmp < 0)  || (tmp > 255)){
		printf("Value of cell must be from 0 to 255\n");
		get(T);
	}
	else {
		*(T->memoryTape + (((T->mem - 1) * 32767) + *(T->position + T->mem))) = tmp;
		printf("Value of cell changed \n\n");
	}
}

void printc (struct machine *T){

	printf("Value of cell[%d] = ", (((T->mem - 1) * 32767) + *(T->position + T->mem)));
	printf("%c \n\n", *(T->memoryTape + (((T->mem - 1) * 32767) + *(T->position + T->mem))));

}

void begin (struct machine *T, int *fpos, FILE *fp){

	char tmp[255];

	if (*(T->memoryTape + (((T->mem - 1) * 32767) + *(T->position + T->mem))) == 0 ){
		printf(" Exit from cycle\n");
		while (strstr(tmp, "end") == 0){
			fgets(tmp, 255, fp);
		}
	}
	else {
		if (ftell(fp) >= 32767) {
			sizeFPOS++;
			fpos = realloc(fpos, sizeFPOS * sizeof(int));
		}
		*fpos = ftell(fp);

	}

}
void end (struct machine *T, int *fpos, FILE *fp){
	if (*(T->memoryTape + (((T->mem - 1) * 32767) + *(T->position + T->mem))) == 0 ){
		printf("Exit from cycle\n");
	}
	else {
		printf("New iteration\n");
		fseek(fp, *fpos, SEEK_SET);
	}
}



int main () {

	FILE *fp;
	char str[255];
	int *fpos;
	init(&T);

	fpos = (int *)malloc(sizeFPOS * sizeof(int));

	fp = fopen("Algoruthm.txt", "r");

	while (!feof(fp)) {

		fgets(str, 255, fp);
		formString(str);

		printf("%s", str);

		if (feof(fp) != 0){
			printf("\n");
		}
		if (strstr(str, "movl") != 0) {
			movl(&T);
		}
		if (strstr(str, "movr") != 0) {
			movr(&T);
		}
		if (strstr(str, "inc") != 0) {
			inc(&T);
		}
		if (strstr(str, "printc") != 0) {
			printc(&T);
			continue;
		}
		if (strstr(str, "dec") != 0) {
			dec(&T);
		}
		if (strstr(str, "print") != 0) {
			print(&T);
		}
		if (strstr(str, "get") != 0) {
			get(&T);
		}
		if (strstr(str, "begin") != 0) {
			begin(&T, fpos, fp);
		}
		if (strstr(str, "end") != 0) {
			end(&T, fpos, fp);
		}

	}

	free(T.memoryTape);
	free(T.position);
	free(fpos);
	fclose(fp);



	return 0;
}



