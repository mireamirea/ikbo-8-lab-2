#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define INIT_SIZE 5		
#define INCREASE 3


struct machine {
	
	int *memoryTape;
	long long int size;			
	int *position;
	long long int mem;			
	
} Turing;


int sizeFPOS = 1;		


void formString (char *s);
void init (struct machine *Turing);
void resizeTape (struct machine *Turing);
void resizePos (struct machine *Turing);
void movl (struct machine *Turing);
void movr (struct machine *Turing);
void inc (struct machine *Turing);
void dec (struct machine *Turing);
void print (struct machine *Turing);
void get (struct machine *Turing);
void printc (struct machine *Turing);
void begin (struct machine *Turing, int *fpos, FILE *fp);
void end (struct machine *Turing, int *fpos, FILE *fp);



int main () {
	
	FILE *fp;
	char str[255];
	int *fpos;			
	
	
	
	init(&Turing);
	fpos = (int *)malloc(sizeFPOS * sizeof(int));
	
	
	/* Reading of file */
	
	fp = fopen("file.txt", "r");
	
	while (!feof(fp)) {
	
		fgets(str, 255, fp);
		formString(str);
		
		
		printf("%s", str);
		
		
		/* For last iteration */
		
		if (feof(fp) != 0){
			printf("\n");
		}
		
		
		/* Call of functions */
		
		if (strstr(str, "movl") != 0) {
			
			movl(&Turing);
		}
		
		if (strstr(str, "movr") != 0) {
			
			movr(&Turing);
		}
		
		if (strstr(str, "inc") != 0) {
			
			inc(&Turing);
		}
		
		if (strstr(str, "printc") != 0) {
			
			printc(&Turing);
			continue;
		}
		
		if (strstr(str, "dec") != 0) {
			
			dec(&Turing);
		}
		
		if (strstr(str, "print") != 0) {
			
			print(&Turing);
		}
		
		if (strstr(str, "get") != 0) {
			
			get(&Turing);
		}
		
		if (strstr(str, "begin") != 0) {
			
			printf("------------ \n");
			begin(&Turing, fpos, fp);
		}
		
		if (strstr(str, "end") != 0) {
			
			end(&Turing, fpos, fp);
		}
		
	}
	
	
	/* Freeing of memory */
	
	free(Turing.memoryTape);
	free(Turing.position);
	free(fpos);
	fclose(fp);
	
	
	
	return 0;
}



void formString (char *s) {
	
	int i = 0, j;


	
	while (s[i] != '\0') {
		
		
		/* Offset of string */
		
		if ((s[i] == ' ') || (s[i] == '\t')) {
			
			for (j = i; j < strlen(s); j++) {
				s[j] = s[j + 1];
			}
			
		}
		
		else {
			i++;
		}
		
		
	}
	
	
}



void init (struct machine *Turing) {
	
	int i;
	
	

	/* Init of Memory Tape */
	
	Turing->mem = 1;
	Turing->size = INIT_SIZE;
	Turing->memoryTape = (int *)malloc(Turing->size * sizeof(int));
	Turing->position = (int *)malloc(Turing->mem * sizeof(int));
	
	
	/* Nullification */
	
	*(Turing->position) = 0;
	
	for (i = 0; i < INIT_SIZE; i++) {
		
		*(Turing->memoryTape + i) = 0;
		
	}
	
	
}



void resizePos (struct machine *Turing) {
	
	Turing->mem++;
	Turing->position = realloc(Turing->position, Turing->mem * sizeof(int));
	
	*(Turing->position + Turing->mem) = 0;
	
}



void resizeTape (struct machine *Turing) {
	
	int i;
	
	
	/* Resize */
	
	Turing->size += INCREASE;
	Turing->memoryTape = realloc(Turing->memoryTape, Turing->size * sizeof(int));
	
	
	/* Nullification */
	
	for (i = (Turing->size - INCREASE); i < Turing->size; i++) {
		
		*(Turing->memoryTape + i) = 0;
		
	}
	
	printf(" - Resize tape: new size = %d \n", Turing->size);
	
}



void movl (struct machine *Turing) {
	
	
	/* Checking of memory */
	
	if ( Turing->size <= ( ((Turing->mem - 1) * 32767) + *(Turing->position + Turing->mem) + 1) ) {
		
		resizeTape(Turing);
	}
	
	if (32767 <= *(Turing->position + Turing->mem)) {
		
		resizePos(Turing);
	}
	
	
	(*(Turing->position + Turing->mem))++;
	
	printf(" - Leap to the left \n\n");
	
	
}



void movr (struct machine *Turing) {
	
	
	if ( ( ((Turing->mem - 1) * 32767) + *(Turing->position + Turing->mem) ) == 0) {
		
		printf(" - Can not go to the right, this is 0 cell \n\n");
		
	}	else {
		
			(*(Turing->position + Turing->mem))--;
			printf(" - Leap to the right \n\n");
	
		}

}



void inc (struct machine *Turing) {
	
	
	/* Checking of errors */
	
	if ( *(Turing->memoryTape + ( ((Turing->mem - 1) * 32767) + *(Turing->position + Turing->mem) ) ) == 255) {
		
		printf(" - Overflow of cell (Max size = 255) \n\n");
		
		*(Turing->memoryTape + ( ((Turing->mem - 1) * 32767) + *(Turing->position + Turing->mem) ) ) = 0;
		
	}
	
	
	else {
	
		(*(Turing->memoryTape + ( ((Turing->mem - 1) * 32767) + *(Turing->position + Turing->mem) ) ))++;
	
		printf(" - Increment of cell \n\n");
	
	}
	
}



void dec (struct machine *Turing) {
	
	
	/* Checking of errors */
	
	if ( *(Turing->memoryTape + ( ((Turing->mem - 1) * 32767) + *(Turing->position + Turing->mem) ) ) == 0 ) {
		
		printf(" - Underflow of cell (Min size = 0)\n\n");
		
	}
	
	
	else {
	
		(*(Turing->memoryTape + ( ((Turing->mem - 1) * 32767) + *(Turing->position + Turing->mem) ) ))--;
	
		printf(" - Decrement of cell \n\n");
	}
	
}



void print (struct machine *Turing) {
	
	printf(" - Value of cell[%d] = ", ( ((Turing->mem - 1) * 32767) + *(Turing->position + Turing->mem) ) );
	printf("%d \n\n", *(Turing->memoryTape + ( ((Turing->mem - 1) * 32767) + *(Turing->position + Turing->mem) )) );
}



void get (struct machine *Turing) {
	
	int tmp;
	
	
	printf(" - Enter value of cell: \n");
	scanf("%d", &tmp);
	
	
	/* Checking for Max & Min values */
	
	if ((tmp < 0)  || (tmp > 255)) {
		
		printf(" - Value of cell must be 0 to 255 \n");
		get(Turing);
	}
	
	else {
		
		*(Turing->memoryTape + ( ((Turing->mem - 1) * 32767) + *(Turing->position + Turing->mem) ) ) = tmp;
		printf(" - Value of cell changed \n\n");
	}
	
}



void printc (struct machine *Turing) {
	
	printf(" - Value of cell[%d] = ", ( ((Turing->mem - 1) * 32767) + *(Turing->position + Turing->mem) ) );
	printf("%c \n\n", *(Turing->memoryTape + ( ((Turing->mem - 1) * 32767) + *(Turing->position + Turing->mem) ) ));
	
}



void begin (struct machine *Turing, int *fpos, FILE *fp) {
	
	char tmp[255];
	

	/* If value of cell = 0 */
	
	if ( *(Turing->memoryTape + ( ((Turing->mem - 1) * 32767) + *(Turing->position + Turing->mem) ) ) == 0 ) {
		
		printf(" - Value of cell = 0 --> exit from cycle \n------------  \n\n");
		while (strstr(tmp, "end") == 0) {
			
			fgets(tmp, 255, fp);
		}
	
	}
	
	
	else {
		
		
		/* Error processing and memory allocation (for ftell) */
		
		if (ftell(fp) >= 32767) {
			
			sizeFPOS++;
			fpos = realloc(fpos, sizeFPOS * sizeof(int));
			
		}
		
		
		*fpos = ftell(fp);

	}
	
	
} 



void end (struct machine *Turing, int *fpos, FILE *fp) {
	
	
	if ( *(Turing->memoryTape + ( ((Turing->mem - 1) * 32767) + *(Turing->position + Turing->mem) ) ) == 0 ) {
		
		printf(" - Value of cell = 0 --> exit from cycle \n------------ \n\n");
	}
	
	else {
		
		printf(" - Transition to a new iteration \n\n");
		fseek(fp, *fpos, SEEK_SET);
	}
	
}



