#include <stdio.h>
#include <stdlib.h>

// Inclure les libraries

#define NUMBER_FONCTIONS 18

char *functions[NUMBER_FONCTIONS] = 
{"ls", "mkdir", "cd", "pwd", "cat", "more", "less", "mv", "cp", "rm", "du", "chown",
"chgrp", "echo", "ps", "top", "su"};

typedef struct {
	char *nom;
	int (*function)(int, char**);
}funcStruct;


// Function which inits each function structure
int init(){
	//funcStruct *ls, *mkdir, *cd, *pwd, *cat, *more, *less, *mv, *cp, *rm, *du, 
	//*chown, *chgr, *echo, *ps, *top, *su;

	funcStruct ls, mkdir, cd, pwd, cat, more, less, mv, cp, rm, du, 
	chown, chgr, echo, ps, top, su;

	ls.nom = "ls";
	mkdir.nom = "mkdir";
	cd.nom = "cd";
	cat.nom = "cat";
	pwd.nom = "pwd";
	more.nom ="more";
	less.nom ="less";
	mv.nom = "mv";
	cp.nom = "cp";
	rm.nom = "rm";
	du.nom= "du";
	chown.nom = "chown";
	chgr.nom = "chrg";
	echo.nom ="echo";
	ps.nom = "ps";
	top.nom = "top";
	su.nom = "su";

	// Manque les pointeurs de fonctions

	return 0;
}

// Pour accéder au champ x.bidule si variable, si pointeurs : x->bidule

int main(int argc, char const *argv[])
{

	return 0;
}