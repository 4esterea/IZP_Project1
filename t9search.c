#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define INPUT_ERR 0
#define INPUT_OK 1

int inputcheck(char *input) // kontroluje znaky vstupního argumentu
{
	for (long unsigned int i = 0; i < strlen(input); i++){
		if ((input[i]) <= '/' || (input[i]) >= ':'){
			return INPUT_ERR;
		} 
	}
	return INPUT_OK;	
}

void rowoutput(char *name, char *number) // zobrazí jméno a telefonní číslo kontaktu ve správném formátu
{
	if (strchr(name, '\n') != NULL){
		name[strlen(name)-2] = '\0';
	}
	if (strchr(number, '\n') != NULL){
		number[strlen(number)-2] = '\0';
	}
	printf("%s, %s\n", name, number);
}

char makeatwink(char element) // nahradí znak ze seznamu kontaktů číslem odpovídajícím danému znaku
{
	char matrix[12][10] =
	{{'0', '+'},
	{'1'},
	{'2', 'a', 'b', 'c', 'A', 'B', 'C'},
	{'3', 'd', 'e', 'f', 'D', 'E', 'F'},
	{'4', 'g', 'h', 'i', 'G', 'H', 'I'},
	{'5', 'j', 'k', 'l', 'J', 'K', 'L'},
	{'6', 'm', 'n', 'o', 'M', 'N', 'O'},
	{'7', 'p', 'q', 'r', 's', 'P', 'Q', 'R', 'S'},
	{'8', 't', 'u', 'v', 'T', 'U', 'V'},
	{'9', 'w', 'x', 'y', 'z', 'W', 'X', 'Y', 'Z'},
	{'\n'},
	{' '}};
	for (int i = 0; i < 12; i++){
		if (strchr(matrix[i], element) != NULL){
			return matrix[i][0];
		}
	}
	return FALSE;
}

int main(int argc, char *argv[])
{
	char printall = FALSE;
	if (argc == 1){
		printall = TRUE;
		goto skipinputcheck;
	}
	if (argc > 2){
		fprintf(stderr, "Error : Unexpected number of arguments\n");
		abort();
	}
	if (inputcheck(argv[1]) == INPUT_ERR){
		fprintf(stderr, "Error : Unexpected input format\n");
		abort();
	}
	skipinputcheck:;
    int row = 0;
    int column = 0;
    char globlist[100][110] = {{0,0}}; 
    char c = getc(stdin);                // vloží znaky ze souboru do pole globlist
	while (c != EOF) {
		if (c == '\n') {
			globlist[row][column] = c;
			row++;
			column = 0;
		}
		else {
			globlist[row][column] = c;
			column++;
			if (column > 100){
				fprintf(stderr, "Error: Too long contact name or number\n");
				abort();
			}
		}
		c = getc(stdin);
	}	
	if (printall == TRUE){ // pokud uživatel nezadal číslo, vytiskne úplný seznam kontaktů
		for (int i = 0; i < row; i+=2){
			rowoutput(globlist[i], globlist[i+1]);
		}
		return 0;
	}
	char *searchKey = argv[1];
	char twinkedlist[100][110] = {{0,0}}; // vytvaří nové pole pro další vyhledávání kontaktů
	for (int i = 0; i <= row; i++) {
		for (int j = 0; globlist[i][j] != '\0'; j++){
			twinkedlist[i][j] = makeatwink(globlist[i][j]);
		}
	}
	char flag = FALSE;
	for (int i = 0; i <= row; i++){
		if (strstr(twinkedlist[i], searchKey) != NULL){ // pomocí strstr hledá argument v poli vytvořeném dříve
			flag = TRUE;
			if (i % 2 == 0){
				rowoutput(globlist[i], globlist[i+1]);
				i++;
			} else{
				rowoutput(globlist[i-1], globlist[i]);
			}
		}
	}
	if (flag == FALSE){  // pokud nebyl nalezen žádný kontakt, vytiskne "Not found"
		printf("Not found\n");
	}
	return 0;
}