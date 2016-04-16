#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decoupe.h"

// Automate de découpe

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

char **str_split (char *s, const char *ct){
   char **tab = NULL;

   if (s != NULL && ct != NULL)
   {
      int i;
      char *cs = NULL;
      size_t size = 1;

/* (1) */
      for (i = 0; (cs = strtok (s, ct)); i++)
      {
         if (size <= i + 1)
         {
            void *tmp = NULL;

/* (2) */
            size <<= 1;
            tmp = realloc (tab, sizeof (*tab) * size);
            if (tmp != NULL)
            {
               tab = tmp;
            }
            else
            {
               fprintf (stderr, "Memoire insuffisante\n");
               free (tab);
               tab = NULL;
               exit (EXIT_FAILURE);
            }
         }
/* (3) */
         tab[i] = cs;
         s = NULL;
      }
      tab[i] = NULL;
   }
   return tab;
}


char*** decoupe(char *command, int* size){
	//char tab[20][20][20];

	char *** tab = malloc(sizeof(char**)*SIZE_CUTTING);
	for (int i = 0; i < 20; ++i)
	{
		tab[i] = malloc(sizeof(char*)*SIZE_CUTTING);
		for (int j = 0; j < 20; ++j)
		{
			tab[i][j] = malloc(sizeof(char)*SIZE_CUTTING);
		}
	}

	int i, j, pred;
	i=0;
 	j=0;
 	pred = 0;

 	//char *decoupe[20];
 	char **t = NULL;
	t = str_split(command, " ");

 	while (t[i]!=NULL)
	{
		if(pred == 99){
			strcpy(tab[j][1], t[i]);
			j++;
			pred = 0;
		}
		else if(isLetter(t[i][0])){
			strcpy(tab[j][0], t[i]);
			j++;
			pred = 0;
		}
		else if(t[i][0] == '|'){
			strcpy(tab[j][0], t[i]);
			j++;
		}
		else if(t[i][0] == '&'){
			strcpy(tab[j][0], t[i]);
			j++;
		}
		else if((t[i][0] == '>' ) || (t[i][0] == '<')){
			strcpy(tab[j][0], t[i]);
			pred = 99;
			//printf("%d\n",pred);
		}
		else if(t[i][0] == '-'){
			pred++;
			strcpy(tab[j-1][pred], t[i]);
		}
		//printf("%s\n", t[i]);
		i++;
	}

	*size = i;
	return tab;
}

int main(int argc, char const *argv[]){

	char ***tab;
	char command[] = "ls -al | texte | cat > texte.txt";
	int size = 0;
	tab = decoupe(command, &size);
	free(tab);

	/*
	printf("tab[0][0] : %s \n",tab[0][0]);
	printf("tab[0][1] : %s \n",tab[0][1]);
	printf("tab[1][0] : %s \n",tab[1][0]);
	printf("tab[1][1] : %s \n",tab[1][1]);
	printf("tab[2][0] : %s \n",tab[2][0]);
	printf("tab[2][1] : %s \n",tab[2][1]);
	printf("tab[3][0] : %s \n",tab[3][0]);
	printf("tab[3][1] : %s \n",tab[3][1]);
	printf("tab[4][0] : %s \n",tab[4][0]);
	printf("tab[4][1] : %s \n",tab[4][1]);
	printf("tab[5][0] : %s \n",tab[5][0]);
	printf("tab[5][1] : %s \n",tab[5][1]);
	*/
}