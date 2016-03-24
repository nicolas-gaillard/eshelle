#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum States
	{
		State0,
		State1,
		State2,
		State3,
		State4,
		State5,
		State6,
		State601,
		State61,
		State62,
		State7,
		State71,
		State8,
		State9,
		StateFinal,
		StateFail,
	};

int isNumber(char c){
	if (c >= '0' && c <= '9')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int isLetter(char c){
	if ((c >= 'a' && c <= 'z') || (c>='A' && c <='Z'))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int main(){

	enum States curState;

	printf("Début de l'automate \n");

	char command[] = "ls | texte | cat << texte.txt";
	int lenString = strlen(command);

	printf("%s de taille %d \n", command, lenString);

	for(int i = 0; i < lenString ; i++){
		switch(curState)
		{
			case State0:
				if(isLetter(command[i]) || command[i] == ' '){
					curState = State1;
				}
				else{
					curState = StateFail;
				}
				break;

			case State1:
				if(command[i] == ' '){
					curState = State2;
				}
				else if(isLetter(command[i]) == 0){
					curState = StateFail;
				}
				break;

			case State2:
				if(command[i] == '-'){
					curState = State3;
				}
				else if(command[i] == ' '){
					curState = State2;
				}
				else if(command[i] == '|'){
					curState = State4;
				}
				else if(command[i] == '<'){
					curState = State61;
				}
				else if(command[i] == '>'){
					curState = State62;
				}
				else{
					curState = StateFail;
				}
				break;

			case State3:
				if(command[i] == '.'){
					curState = State8;
				}
				else if(isLetter(command[i]) == 0){
					curState = StateFail;
				}
				break;

			case State4:
				if(isLetter(command[i])){
					curState = State5;
				}
				else if(command[i] != ' '){
					curState = StateFail;
				}
				break;

			case State5:
				if(command[i] == ' '){
					curState = State2;
				}
				else if(isLetter(command[i] == 0)){
					curState = StateFail;
				}
				break;

			case State6:
				if(command[i] == '.'){
					curState = State8;
				}
				else if(isLetter(command[i]) == 0){
					curState = StateFail;
				}
				break;

			case State601:
				if(command[i] == ' '){
					curState = State6;
				}
				else{
					curState = StateFail;
				}
				break;

			case State61:
			// Cas du double chevron ouvrant
				if(command[i] == '<'){
					curState = State7;
				}
				else{
					curState = State601;
				}
				break;

			case State62:
			// Cas du double chevron fermant
			// Revoir s'il est nécessaire de différencier le cas du simple fermant et du double fermant
			// Pour l'instant les deux cas renvoient sur le même Etat
				if(command[i] == '>'){
					curState = State601;
				}
				else{
					curState = State601;
				}
				break;

			case State7:
				if(command[i] == ' '){
					curState = State71;
				}
				else{
					curState = StateFail;
				}
				break;
				

			case State71:
				if(command[i] == '.'){
					curState = State8;
				}
				else if(isLetter(command[i]) == 0){
					curState = StateFail;
				}
				break;

			case State8:
				if(isLetter(command[i]) == 0){
					curState = StateFail;
				}
				break;

			case StateFail:
				printf("La commande n'est pas correcte \n");
				return 0;
				break;
		}
		printf("%d \n", curState);
	}
	printf("Fin de l'automate \n");
}
