#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TuringM{

	struct TuringM *next;
	struct TuringM *prev;
	int data;

}TuringM;

TuringM* CreateTur(){

	TuringM *NewItem;
	NewItem = (TuringM*)malloc(sizeof(TuringM));
	if(NewItem != NULL){

		NewItem->prev = NewItem->next = NULL;
		NewItem->data = 0;

	}

	else{

		printf("Error.No memory\n");
		return(NULL);

	}

	return(NewItem);

}

void TurAddBefore(TuringM *ExitingItem){

	TuringM *NewItem;
	NewItem = CreateTur();
	if(ExitingItem != NULL && NewItem != NULL){
		NewItem->next = ExitingItem;
		NewItem->prev = ExitingItem->prev;
		ExitingItem->prev = NewItem;

	}

	if(NewItem->prev != NULL){
		NewItem->prev->next = NewItem;

	}

	return;

}

void TurAddAfter(TuringM *ExitingItem){

	TuringM *NewItem;
	NewItem = CreateTur();
	if(ExitingItem != NULL && NewItem != NULL){

		NewItem->next = ExitingItem->next;
		NewItem->prev = ExitingItem;
		ExitingItem->next = NewItem;

	}

	if(NewItem->next != NULL){

		NewItem->next->prev = NewItem;

	}

	return;

}
void GetFirst(TuringM *current){

	while(current->prev != NULL)
		current = current->prev;
	return;

}
void DeleteTur(TuringM *current){

	TuringM *Next;
	GetFirst(current);
	while(current->next != NULL){

		Next = current->next;
		free(current);

	}

	return;

}

void delsp(char* buffer, char *command){

	int i, j = 0;
	for(i = 0; buffer[i] != '\0'; i++)
		if(buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '\t'){
			command[j] = buffer[i];
			if(command[0] == '*'){
				strcpy(command, "skip");
				break;
			}

			j++;
		}

	command[j] = '\0';
}


int main(int argc, char **argv){

	FILE *fp;
	char buffer[255],com[255],name[255];
	long int b[10];
	TuringM *curr;
	size_t i=-1;
	if(argc != 2){
		printf("Enter the name of the file:\n");
		scanf("%s", name);

	}

	else
		strcpy(name, argv[1]);
	fp = fopen(name, "r");
	while(fp==NULL){

		printf("Can not open the file. Please try again:\n");
		scanf("%s", name);
		fp = fopen(name, "r");

	}

	curr=CreateTur();
	while(fgets(buffer, 256, fp)){

		delsp(buffer,com);
		if(!strcmp(com, "movl")){

			if(curr->prev == NULL){

				TurAddBefore(curr);
				curr = curr->prev;
			}

			else
				curr = curr->prev;
			continue;

		}

		if(!strcmp(com, "movr")){

			if(curr->next == NULL){

				TurAddAfter(curr);
				curr = curr->next;

			}

			else
				curr = curr->next;
			continue;

		}

		if(!strcmp(com, "inc")){

			if(curr->data > 255){

				printf("Too big value. Can't increase.\n");
				curr->data = 0;
				continue;

			}

			curr->data++;
			continue;

		}

		if(!strcmp(com, "dec")){

			if(curr->data == 0){

				printf("Can't decrease.\n");
				continue;

			}

			curr->data--;
			continue;

		}

		if(!strcmp(com, "print")){

			printf("Your data: %d\n", curr->data);
			continue;

		}

		if(!strcmp(com, "get")){

			printf("Enter data: ");
			scanf("%d", &curr->data);
			continue;

		}

		if(!strcmp(com, "printc")){

			printf("Your symbol: %c\n", curr->data);
			continue;
		}

		if(!strcmp(com, "begin")){

            if(curr->data==0){

                while(1){

                    if(fgets(buffer, 256, fp)==0)
                        return 0;
                    delsp(buffer,com);
                    if(!strcmp(com,"end"))
                        break;

                }

            }

			i++;
			b[i] = ftell(fp);
			if(i > 9){
				printf("Error.Too much nesting\n");
				break;

			}

			continue;

		}

		if(!strcmp(com, "end")){

			if(i == -1){
				printf("Error.Miss begin");
				break;

			}

			if(curr->data != 0){

				fseek(fp, b[i], SEEK_SET);
				continue;

			}

			i--;
			continue;

		}

		if(!strcmp(com, "skip")){

			continue;

		}

		printf("Invalid syntax \n");
		break;

	}

	DeleteTur(curr);
	return 0;

}
