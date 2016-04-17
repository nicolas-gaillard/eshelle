#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "automate.h"

enum States
	{
		State0,  //0
		State1,  //1
		State2,  //2
		State29, //3
		State3,  //4
		State31, //5
		State32, //6
		State33, //7
		State34, //8
		State4,  //9
		State41, //10
		State5,  //11
		State51, //12
		State52, //13
		State53, //14
		State6,  //15
		State601,//16
		State61, //17
		State62, //18
		State7,  //19
		State71, //20
		State8,  //21
		State9,  //22
		State91, //23
		State92, //24
		State95,
		//StateFinal, //21
		StateFail, //25
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
int automate(char command[]){
	enum States curState;
	int lenString = strlen(command);
	int i;
	for(i = 0; i < lenString ; i++){
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
				else if(isLetter(command[i])){
					curState = State51;
				}
				else if(command[i] == '.'){
					curState = State9;
				}
				else if(command[i] == '$'){
					curState = State95;
				}
				else{
					curState = StateFail;
				}
				break;

			case State29:
				if(command[i] == ' '){
					curState = State3;
				}
				else{
					curState = StateFail;
				}

			case State3:
				if(command[i] == ' '){
					curState = State2;
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
				else if(command[i] == '/'){
					curState = State52;
				}
				else if(isLetter(command[i])){
					curState = State51;
				}
				else if(command[i] == '.'){
					curState = State53;
				}
				else{
					curState = StateFail;
				}
				break;

			case State52:
				if(isLetter(command[i])){
					curState = State52;
				}
				else if(command[i] == '.'){
					curState = State53;
				}
				else{
					curState = StateFail;
				}
				break;

			case State53:
				if(isLetter(command[i])){
					curState = State53;
				}
				else if(command[i] == ' '){
					curState = State2;
				}
				else{
					curState = StateFail;
				}
				break;

			case State6:
				if(command[i] == '.'){
					curState = State8;
				}
				else if(command[i] == '/'){
				}
				else if(isLetter(command[i])){
					curState = State6;
				}
				else{
					curState = StateFail;
				}
				break;

			case State601:
				if(command[i] == ' '){
					curState = State6;
				}
				else if(isLetter(command[i])){
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
					curState = State6;
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

			case State9:
				if(command[i] == '.'){
					curState = State91;
				}
				else if(command[i] == '/'){
					curState = State92;
				}
				else if(command[i] == ' '){
					curState = State2;
				}
				else if(isLetter(command[i])){
					curState = State2;
				}
				else{
					curState = StateFail;
				}
				break;	

			case State91:
				if(command[i] == '/'){
					curState = State92;
				}
				else if(command[i] == ' '){
					curState = State2;
				}
				else{
					curState = StateFail;
				}
				break;

			case State92:
				if(command[i] == ' '){
					curState = State2;
				}
				else if(isLetter(command[i])){
					curState = State51;
				}
				else{
					curState = StateFail;
				}

				break;

			case State95:
				if(isLetter(command[i])){
					curState = State51;
				}
				else{
					curState = StateFail;
				}
				break;

			case StateFail:
				printf("La commande n'est pas correcte \n");
				return 0;
				break;
		}
	}
	return 1;
}