/** Author : Jordan BLOUIN **/

#include "IS_file.c"
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

/*
	Function name : 
		recursiveMv
	Arguments : 
		char *nameDir (name of the directory we want to remove)
	Description : 
		removes recursively directory (if "-r")
*/
int recursiveMv(char *nameSource,char *nameDestination){
	// Declaration of variables	
	DIR *source;           	            // the source directory we want to move ...
	DIR *destination;           	    // ... in the destination directory
	struct dirent *sourceEntry;     	// used to know the information of the source directory
	struct stat file_stat;    	        // used to know the information of the content file
	char buffer[1024] = {0};	        // a tab which contains the path of a file containing in the source directory

	if (isFolder(nameSource)){
	    // Source directory opening
    	if ((source=opendir(nameSource))==NULL){
    		printf("mv : cannot open the directory %s\n",nameSource);
    		return -1;
    	}
	
	    // Destination directory opening
		if(mkdir(nameDestination,0775)!=0){
		    printf("mv2 : repertory %s already exists\n",nameDestination);
		}
		else printf("win !!!!!!\n");
	    if ((destination=opendir(nameDestination))==NULL){
    		printf("mv : no such directory %s\n",nameDestination);
		    return -1;
    	}
    	
    	// For each content file of the source directory we move it
    	while ((sourceEntry=readdir(source))!=NULL) {
    		printf("source : %s  destination : %s\n",nameSource,nameDestination);
    		// We ignore the "." and ".." directories
    		if (strcmp(sourceEntry->d_name,".")==0 || strcmp(sourceEntry->d_name,"..")==0) continue;
    		// We make the path of the content file
    		snprintf(buffer, 1024, "%s/%s", nameSource, sourceEntry->d_name);
    		// We collect the information of the content file
    		stat(buffer, &file_stat);
    		// If the content file is a regular file we unlink it
    		if (S_ISREG(file_stat.st_mode)){
    		    FILE *f_Src;
                FILE *f_Dest;
                char *path_Dest = nameDestination, *path_Src = buffer;
                char tmp[4096];
                int i;
                
                /* Setting the path to create a new file into the designated folder */
                if(buffer[0] != '.')
                {
                    strcat(path_Dest, "/");
                    strcat(path_Dest, sourceEntry->d_name);
                    printf("Dest path : %s\n", path_Dest);
                }
                
                if((f_Src = fopen(path_Src, "r")) == NULL)
                {
                    /* If the file to move can't be opened */
                    printf("Source file not found\n");
                    return 1;
                }
                if((f_Dest = fopen(path_Dest, "w")) == NULL)
                {
                    /* If the destination folder doesn't exist */
                    printf("Destination file not found\n");
                    fclose(f_Src);
                    return 2;
                }
                
                while((i = fread(tmp, 1, 4096, f_Src)) != 0)
                {
                    fwrite(tmp, 1, i, f_Dest);
                }
                fclose(f_Dest);
                fclose(f_Src);
    		    unlink(buffer);
    		}
    		// Else it's a directory and we throw the recursive function on it
    		else if ( S_ISDIR(file_stat.st_mode) ){
    		    char tmp[strlen(nameDestination)];
    		    strcpy(tmp,nameDestination);
    		    strcat(tmp,"/");
    		    strcat(tmp,sourceEntry->d_name);
                
    		    recursiveMv(buffer,tmp);
    		}
    	}
    	if(rmdir(nameSource)!=0){
    		printf("rm : directory %s doesn't exist\n",nameSource);
    	}
	}
	else if(isRegularFile(nameSource)){
	    printf("source : %s  destination : %s\n",nameSource,nameDestination);
	    FILE *f_Src;
        FILE *f_Dest;
        char *path_Dest = nameDestination, *path_Src = nameSource;
        char tmp[4096];
        int i;
        
        if((f_Src = fopen(path_Src, "r")) == NULL)
        {
            /* If the file to move can't be opened */
            printf("Source file not found\n");
            return 1;
        }
        if((f_Dest = fopen(path_Dest, "w")) == NULL)
        {
            /* If the destination folder doesn't exist */
            printf("Destination file not found\n");
            fclose(f_Src);
            return 2;
        }
        while((i = fread(tmp, 1, 4096, f_Src)) != 0)
        {
            fwrite(tmp, 1, i, f_Dest);
        }
        fclose(f_Dest);
        fclose(f_Src);
	    unlink(nameSource);
	}
	else printf("mv : file/directory %s doesn't exist\n",nameSource);
    
    // Directory closure
    //closedir(destination);
    // Directory closure
    //closedir(source);
    	
	return 0;
}


/*  Function :
*       main
*   Arguments :
*       int argc        -> number of arguments
*       char *argv[]    -> arguments
*   Description :
*       Move a file into the indicated folder
*       It's the same logic as cp.c but we remove the file from the source folder after the copy is done
*/
int main(int argc, char *argv[]){
    /*
    * Displays an error message if there are too few/many arguments
    */
    if(argc<2){
        printf("mv : too few argument\n");
        return -1;
    }
    if(argc==2){
        printf("mv : missing destination file after '%s'\n",argv[1]);
        return -1;
    }
    else if(argc>3){
        printf("mv : too many arguments\n");
        return -1;
    }
    else{
        if(isRegularFile(argv[1])){
            FILE *f_Src;
            FILE *f_Dest;
            char *path_Dest = argv[2], *path_Src = argv[1];
            char tmp[4096];
            int i;
            
            /* Setting the path to create a new file into the designated folder */
            if(argv[1][0] != '.')
            {
                strcat(path_Dest, "/");
                strcat(path_Dest, argv[1]);
                printf("Dest path : %s\n", path_Dest);
            }
            
            if((f_Src = fopen(path_Src, "r")) == NULL)
            {
                /* If the file to move can't be opened */
                printf("Source file not found\n");
                return 1;
            }
            if((f_Dest = fopen(path_Dest, "w+")) == NULL)
            {
                /* If the destination folder doesn't exist */
                printf("Destination folder not found\n");
                fclose(f_Src);
                return 2;
            }
            while((i = fread(tmp, 1, 4096, f_Src)) != 0)
            {
                fwrite(tmp, 1, i, f_Dest);
            }
            fclose(f_Dest);
            fclose(f_Src);
            remove(argv[1]);
        }
        else if(isFolder(argv[1])){
            recursiveMv(argv[1],argv[2]);
        }
        else{
            printf("mv : file/directory '%s' doesn't exist\n", argv[1]);
            return -1;
        }
    }
    return 0;
}