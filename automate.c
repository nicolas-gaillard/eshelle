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
		State7,
		State8,
		State9,
		StateFinal,
		StateFail,
	};

int isNumber(c){
	if (c >= '0' && c <= '9')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int isLetter(c){
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

	char command[] = "ls -l";
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
				else if(command[i] == '<<'){
					curState = State7;
				}
				else if(command[i] == '<' || command[i] == '>' || command[i] == '>>'){
					curState = State6;
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


		}
	}
}
