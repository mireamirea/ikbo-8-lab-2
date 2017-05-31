#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INIT__STRIP_SIZE 5
#define STRIP_INC 3

/*Main turing struct*/
struct Turing {
	
	int *stripMemory;
	long long int stripSize;	
	int *stripPosition;
	long long int memory;	
	
} turing_t;


int sizePOS = 1;		/* Size of variable, essential for functions begin & end */

/*main parsing function*/
void string_parse (char *s) {
	
	int i = 0, j;
	
	while (s[i] != '\0') {
		
		if ((s[i] == ' ') || (s[i] == '\t')) {
			for (j = i; j < strlen(s); j++) 
				s[j] = s[j + 1];
		} else {
			i++;
		}
		
	}
	
}


/* turing machine init */
void init (struct Turing *turing_t) {
	
	int i;
		
	turing_t->memory = 1;
	turing_t->stripSize = INIT__STRIP_SIZE;
	turing_t->stripMemory = (int *)malloc(turing_t->stripSize * sizeof(int));
	turing_t->stripPosition = (int *)malloc(turing_t->memory * sizeof(int));
	
	*(turing_t->stripPosition) = 0;
	
	for (i = 0; i < INIT__STRIP_SIZE; i++) {
		
		*(turing_t->stripMemory + i) = 0;
		
	}

}

void resize_strip_poaition (struct Turing *turing_t) {
	
	turing_t->memory++;
	turing_t->stripPosition = realloc(turing_t->stripPosition, turing_t->memory * sizeof(int));
	
	*(turing_t->stripPosition + turing_t->memory) = 0;
	
}

void resize_strip (struct Turing *turing_t) {
	
	int i;
		
	turing_t->stripSize += STRIP_INC;
	turing_t->stripMemory = realloc(turing_t->stripMemory, turing_t->stripSize * sizeof(int));
	
	for (i = (turing_t->stripSize - STRIP_INC); i < turing_t->stripSize; i++) {
		
		*(turing_t->stripMemory + i) = 0;
		
	}
	
	printf(" - Resize tape: new size = %d \n", turing_t->stripSize);
	
}

/* move left function */
void movl (struct Turing *turing_t) {
	
	if ( turing_t->stripSize <= ( ((turing_t->memory - 1) * 32767) + *(turing_t->stripPosition + turing_t->memory) + 1) ) {
		
		resize_strip(turing_t);
	}
	
	if (32767 <= *(turing_t->stripPosition + turing_t->memory)) {

		resize_strip_poaition(turing_t);
	}
	
	(*(turing_t->stripPosition + turing_t->memory))++;
	
	printf(" - Leap to the left \n\n");
	
	
}

/* move right function */
void movr (struct Turing *turing_t) {
	
	
	if ( ( ((turing_t->memory - 1) * 32767) + *(turing_t->stripPosition + turing_t->memory)))		
		printf("Strip can't moving right \n");
	else {
		
		(*(turing_t->stripPosition + turing_t->memory))--;
		printf("Strip was moving right \n"); 
	}
}


/* increment function */
void inc (struct Turing *turing_t) {		
	
	if ( *(turing_t->stripMemory + ( ((turing_t->memory - 1) * 32767) + *(turing_t->stripPosition + turing_t->memory) ) ) == 255) {
		
		printf(" Max size is 255 \n");
		
		*(turing_t->stripMemory + ( ((turing_t->memory - 1) * 32767) + *(turing_t->stripPosition + turing_t->memory) ) ) = 0;

	} else {
	
		(*(turing_t->stripMemory + ( ((turing_t->memory - 1) * 32767) + *(turing_t->stripPosition + turing_t->memory) ) ))++;
	
		printf("Incrementing\n");
	
	}
	
}



void dec (struct Turing *turing_t) {
	
	if ( *(turing_t->stripMemory + ( ((turing_t->memory - 1) * 32767) + *(turing_t->stripPosition + turing_t->memory) ) ) == 0 ) {
		printf("Min size is 0 \n");
	} else {
		(*(turing_t->stripMemory + ( ((turing_t->memory - 1) * 32767) + *(turing_t->stripPosition + turing_t->memory) ) ))--;
		printf("Decrementing \n");
	}
	
}

void print (struct Turing *turing_t) {
	
	printf("Value of cell %d is", ( ((turing_t->memory - 1) * 32767) + *(turing_t->stripPosition + turing_t->memory) ) );
	printf("%d \n\n", *(turing_t->stripMemory + ( ((turing_t->memory - 1) * 32767) + *(turing_t->stripPosition + turing_t->memory) )) );
}



void get (struct Turing *turing_t) {
	
	int tmp;
	
	printf("Input value of cell: \n");
	scanf("%d", &tmp);
	
	if ((tmp < 0)  || (tmp > 255)) {
		
		printf("Max value is 255, min value is 0 \n");
		get(turing_t);
	}
	
	else {
		
		*(turing_t->stripMemory + ( ((turing_t->memory - 1) * 32767) + *(turing_t->stripPosition + turing_t->memory) ) ) = tmp;
		printf(" Value was changed \n");
	}
	
}

void printc (struct Turing *turing_t) {
	printf(" - Value of cell %d is ", ( ((turing_t->memory - 1) * 32767) + *(turing_t->stripPosition + turing_t->memory) ) );
	printf("%c \n\n", *(turing_t->stripMemory + ( ((turing_t->memory - 1) * 32767) + *(turing_t->stripPosition + turing_t->memory) ) ));
}



void begin (struct Turing *turing_t, int *fpos, FILE *fp) {
	
	char tmp[255];
	
	
	if ( *(turing_t->stripMemory + ( ((turing_t->memory - 1) * 32767) + *(turing_t->stripPosition + turing_t->memory) ) ) == 0 ) {
		
		printf("Value of cell is 0  \nthen exit from cycle \n\n");
		while (strstr(tmp, "end") == 0) {
			
			fgets(tmp, 255, fp);
		}
	
	} else {
			
		if (ftell(fp) >= 32767) {	
			sizePOS++;
			fpos = realloc(fpos, sizePOS * sizeof(int));
		}
		
		*fpos = ftell(fp);
	}
} 

void end (struct Turing *turing_t, int *fpos, FILE *fp) {
		
	if ( *(turing_t->stripMemory + ( ((turing_t->memory - 1) * 32767) + *(turing_t->stripPosition + turing_t->memory) ) ) == 0 ) {	
		printf("Value of cell is 0  \nthen exit from cycle \n\n");
	} else {	
		printf("Transiting new iteration\n");
		fseek(fp, *fpos, SEEK_SET);
	}
}

int main () {
	
	FILE *fp;
	char str[255];
	int *fpos;			

	init(&turing_t);
	fpos = (int *)malloc(sizePOS * sizeof(int));
	
	fp = fopen("turing.txt", "r");
	
	while (!feof(fp)) {
	
		fgets(str, 255, fp);
		string_parse(str);
		
		printf("%s", str);
		
		if (feof(fp) != 0) printf("\n");
		if (strstr(str, "movl") != 0) movl(&turing_t);
		if (strstr(str, "movr") != 0) movr(&turing_t);
		if (strstr(str, "inc") != 0) inc(&turing_t);
		
		if (strstr(str, "printc") != 0) {	
			printc(&turing_t);
			continue;
		}

		if (strstr(str, "dec") != 0) dec(&turing_t);
		if (strstr(str, "print") != 0) print(&turing_t);
		if (strstr(str, "get") != 0) get(&turing_t);
		
		if (strstr(str, "begin") != 0) {
			printf("........\n");
			begin(&turing_t, fpos, fp);
		}

		if (strstr(str, "end") != 0) end(&turing_t, fpos, fp);
	}
	
	fclose(fp);
	free(turing_t.stripMemory);
	free(turing_t.stripPosition);
	free(fpos);
	
	return 0;
}

