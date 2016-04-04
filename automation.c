#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "redirection.h"
#include "struct.h"
#include "automation.h"


void executionCommands(char ***cmd, int size){

	if (strcmp(cmd[size-1][0],"&") == 0){
		// commandes en arrière plan
	}

	for (int i = 0; i < size; ++i)
	{
		int redirection = whatsThisRedirection(cmd[i]);
		switch(redirection)
		{
			case SIMPLE_REDIRECTION:
				// <, > or >>
				execute(cmd[i-1]);
				break;

			case PIPE:
				pipeExecute(cmd[i-1], cmd[i+1]);
				break;

			case BACKGROUND :
				backgroundExecute(cmd[i-1]);
				break;

			case OR :
				orExecute(cmd[i-1], cmd[i+1]);
				break;

			case AND :
				andExecute(cmd[i-1], cmd[i+1]);
				break;

			case HERE_COMMANDS:
				hereCommandsExecute(cmd[i], cmd[i-1]);
				break;

			default :
				break;
		}

		/*
		if (i == size){
			execute(cmd[i-1]);
		}
		*/
	}
}