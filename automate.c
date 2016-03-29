#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum States
	{
		State0,  //0
		State1,  //1
		State2,  //2
		State3,  //3
		State31,
		State32,
		State33,
		State34,
		State4,  //4
		State41, //5
		State5,  //6
		State51, //7
		State6,  //8
		State601,//9
		State61, //10
		State62, //11
		State7,  //12
		State71, //13
		State8,  //14
		State9,  //15
		StateFinal, //16
		StateFail, //17
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

	char command[] = "ls -al | texte | cat << texte.txt";
	int lenString = strlen(command);
	int i = 0;

	printf("%s de taille %d \n", command, lenString);

	for(i ; i < lenString ; i++){
		switch(curState)
		{
			case State0:
				// Permet de gérer les espaces qui peuvent etre placés devant la commande
				if(command[i] == ' '){
					curState = State0;
				}
				else if(isLetter(command[i])){
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
				else if(command[i] == '&'){
					curState = State31;
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

			case State31:
				if(command[i] == '&'){
					curState = State32;
				}
				else{
					curState = StateFail;
				}
				break;

			case State32:
				if(command[i] == ' '){
					curState = State33;
				}
				else{
					curState = StateFail;
				}
				break;

			case State33:
				if(isLetter(command[i])){
					curState = State34;
				}
				else{
					curState = StateFail;
				}
				break;

			case State34:
				if(command[i] == ' '){
					curState = State2;
				}
				else if(isLetter(command[i]) == 0 ){
					curState = StateFail;
				}
				break;

			case State4:
				if(command[i] == ' '){
					curState = State5;
				}
				else if(command[i] == '|'){
					curState = State41;
				}
				else{
					curState = StateFail;
				}
				break;

			case State41:
				if(command[i] == ' '){
					curState = State5;
				}
				else{
					curState = StateFail;
				}

			case State5:
				if(isLetter(command[i])){
					curState = State51;
				}
				else{
					curState = StateFail;
				}
				break;

			case State51:
				if(command[i] == ' '){
					curState = State2;
				}
				else if(isLetter(command[i])){
					curState = State51;
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
				else if(command[i] == ' '){
					curState = State601;
				}
				else{
					curState = StateFail;
				}
				break;

			case State62:
			// Cas du double chevron fermant
			// Revoir s'il est nécessaire de différencier le cas du simple fermant et du double fermant
			// Pour l'instant les deux cas renvoient sur le même Etat
				if(command[i] == '>'){
					curState = State601;
				}
				else if(command[i] == ' '){
					curState = State601;
				}
				else{
					curState = StateFail;
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
		printf("Le caractere %c nous envoie vers l'etape %d \n", command[i], curState);
	}
	printf("Fin de l'automate \n");
}
