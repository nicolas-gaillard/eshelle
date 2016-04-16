#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decoupe.h"
#include "automate.h"

// Automate de découpe

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
	//char tab[SIZE_CUTTING][SIZE_CUTTING][SIZE_CUTTING];

	int m,l;

	char *** tab = malloc(sizeof(char**)*SIZE_CUTTING);
	for (m = 0; m < SIZE_CUTTING; ++m)
	{
		tab[m] = malloc(sizeof(char*)*SIZE_CUTTING);
		for (l = 0; l < SIZE_CUTTING; ++l)
		{
			tab[m][l] = malloc(sizeof(char)*SIZE_CUTTING);
		}
	}

	int i, j, k, pred;
	i=0;
	k=0;
 	j=0;
 	pred = 0;

 	//char *decoupe[SIZE_CUTTING];
 	char **t = NULL;
	t = str_split(command, " ");

 	while (t[i]!=NULL)
	{
		if( (strcmp(t[i],"|")==0) || (strcmp(t[i],"&&")==0) || (strcmp(t[i],"||")==0) ){
			j++;
			k=0;
			strcpy(tab[j][k], t[i]);
			j++;
			k=0;
		}
		else if( (strcmp(t[i],"<")==0) || (strcmp(t[i],">")==0) || (strcmp(t[i],">>")==0) || (strcmp(t[i],"<<")==0) ){
			j++;
			k=0;
			strcpy(tab[j][k], t[i]);
			k++;
		}
		else{
			strcpy(tab[j][k], t[i]);
			k++;
		}
		//printf("%s\n", t[i]);
		i++;
	}/*
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
	}*/

	*size = j+1;
	return tab;
}
/*
int main(int argc, char const *argv[])
{
	char ***tab;
	char *command = "ls -l";
	int size;
	tab = decoupe(command, &size);
	printf("%s\n", tab[0][0]);
	printf("%s\n", tab[0][1]);
	printf("%s\n", tab[0][2]);
	return 0;
}
*/
