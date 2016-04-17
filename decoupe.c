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

	int i, j, k;
	i=0;
	k=0;
 	j=0;

 	char **t = NULL;
	t = str_split(command, " ");

 	while (t[i]!=NULL)
	{
		if( (strcmp(t[i],"|")==0) || (strcmp(t[i],"&&")==0) || (strcmp(t[i],"||")==0) ){
			tab[j][k] = NULL;
			j++;
			k=0;
			strcpy(tab[j][k], t[i]);
			j++;
			k=0;
		}
		else if( (strcmp(t[i],"<")==0) || (strcmp(t[i],">")==0) || (strcmp(t[i],">>")==0) || (strcmp(t[i],"<<")==0) || (strcmp(t[i],"&")==0) ){
			tab[j][k] = NULL;
			j++;
			k=0;
			strcpy(tab[j][k], t[i]);
			k++;
		}
		else{
			strcpy(tab[j][k], t[i]);
			k++;
		}
		i++;
	}
	tab[j][k] = NULL;
	j++;
	tab[j] = NULL;
	
	*size = j;
	return tab;
}