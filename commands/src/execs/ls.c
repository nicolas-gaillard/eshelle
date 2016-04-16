/*** Author : Quentin Levavasseur ***/

#include "IS_file.c"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>

/*
	Function name : 
		advancedDisplay 
	Arguments : 
		struct dirent *dptr (the current file/directory)
		int dflag (flag which represents if option "-d" is required or not)
	Description : 
		displays the current content file/directory information on a line
*/
void advancedDisplay(struct dirent *dptr,int dflag){
	// Declaration of variables
	struct stat st;				// used to know the general information of the content file/directory
	struct passwd *userInfo;	// used to know the user information of the content file/directory
    struct group *groupInfo;	// used to know the group information of the content file/directory
    struct tm *timeInfo;		// used to know the time information of the content file/directory
    
    // We collect the information of the content file/directory
    lstat(dptr->d_name,&st);
    
    // We display the file nature
    if ((st.st_mode & S_IFSOCK)==S_IFSOCK) printf("s");	// socket
	else if ((st.st_mode & S_IFLNK)==S_IFLNK) printf("l");	// symbolic link
	else if ((st.st_mode & S_IFREG)==S_IFREG) printf("-");	// regular file
	else if ((st.st_mode & S_IFBLK)==S_IFBLK) printf("b");	// block device
	else if ((st.st_mode & S_IFDIR)==S_IFDIR) printf("d");	// directory
	else if ((st.st_mode & S_IFCHR)==S_IFCHR) printf("c");	// character device
	else if ((st.st_mode & S_IFIFO)==S_IFIFO) printf("p");	// FIFO
	
	// We display the rights according to users
	printf("%c",(st.st_mode & S_IRUSR)==S_IRUSR ? 'r' : '-');	// owner R
	printf("%c",(st.st_mode & S_IWUSR)==S_IWUSR ? 'w' : '-');	// owner W
	printf("%c",(st.st_mode & S_IXUSR)==S_IXUSR ? 'x' : '-');	// owner X
	printf("%c",(st.st_mode & S_IRGRP)==S_IRGRP ? 'r' : '-');	// group R
	printf("%c",(st.st_mode & S_IWGRP)==S_IWGRP ? 'w' : '-');	// group W
	printf("%c",(st.st_mode & S_IXGRP)==S_IXGRP ? 'x' : '-');	// group X
	printf("%c",(st.st_mode & S_IROTH)==S_IROTH ? 'r' : '-');	// other R
	printf("%c",(st.st_mode & S_IWOTH)==S_IWOTH ? 'w' : '-');	// other W
	printf("%c",(st.st_mode & S_IXOTH)==S_IXOTH ? 'x' : '-');	// other X
	
	// We display the number of link
	printf(" %d",st.st_nlink);

	// We display the owner of the file/directory
	userInfo=getpwuid(st.st_uid);
	printf(" %s",userInfo->pw_name);

	// We display the owner group of the file/directory
	groupInfo=getgrgid(st.st_gid);
	printf(" %s",groupInfo->gr_name);

	// We display the file/directory size
	printf(" %5d",st.st_size);

	// We display the information about time
	timeInfo=localtime(&st.st_mtime);
	printf(" %4d-%02d-%02d %02d:%02d",timeInfo->tm_year+1900,timeInfo->tm_mon+1,timeInfo->tm_mday,timeInfo->tm_hour,timeInfo->tm_min);

	// If option "-d" is not required and if it's a directory the information will be green
	if(dflag==0 && isFolder(dptr->d_name)){
		printf("\033[32m");
		printf(" %s \n",dptr->d_name);
    	printf("\033[0m");
	}
    else printf(" %s \n",dptr->d_name);
}

/*
	Function name : 
		main 
	Arguments : 
		int argc (number of passed arguments)
		char *argv[] (arguments)
	Description : 
		displays the current repertory's content
*/
int main(int argc,char *argv[]){      
	// Declaration of variables	
	DIR *dirp;						// the directory we want to know the content
    struct dirent *dptr;			// used to know the information of the directory
    int aflag=0, dflag=0, lflag=0;	// used to know what options are required
    
    // We check if option "-a", "-d" or "-l" is required
	char c;
	while ( (c = getopt(argc, argv, "adl")) != -1 ){
		switch(c){
			case 'a':
				aflag = 1;
				break;
			case 'd':
				dflag = 1;
				break;
			case 'l':
				lflag = 1;
				break;
			case '?':
				printf("ls : option %s doesn't exist for command ls\n",argv[optind-1]);
				return -1;
		}
	}
		
	int index = optind;
	// If there is no argument we take automatically the current directory
	if(argv[index]==NULL){
		if ((dirp=opendir("."))==NULL){
			printf("ls : directory %s doesn't exist\n",".");
			return -1;
		}
		while((dptr=readdir(dirp))){
	    	// If option "-a" is not required we ignore the "." and ".." directories
			if(aflag==0) if (!strcmp(dptr->d_name,".") || !strcmp(dptr->d_name,"..")) continue;
	    	// If option "-l" is required we display the advanced information of the content file/directory
	    	if(lflag==1){
	    		advancedDisplay(dptr,dflag);
	    	}
	    	else{
	    		// If option "-d" is not required and if it's a directory the information will be green
	    		if(dflag==0 && isFolder(dptr->d_name)){
		    		printf("\033[32m");
					printf("%s ",dptr->d_name);
			    	printf("\033[0m");
		    	}
			    else printf("%s ",dptr->d_name);
	    	}
	    }
	    printf("\n");
	    closedir(dirp);
	}
	// For each arguments we display his information
	else{
		for (index = optind; index < argc; index++){
			// If it's a folder we 
			if(isFolder(argv[index])){
				if ((dirp=opendir(argv[index]))==NULL){
					printf("ls : directory %s doesn't exist\n",argv[index]);
				}
				else{
					while((dptr=readdir(dirp))){
				    	// We ignore the "." and ".." directories
						if(aflag==0) if (!strcmp(dptr->d_name,".") || !strcmp(dptr->d_name,"..")) continue;
				    	// If option "-l" is required we display the advanced information of the content file/directory
				    	if(lflag==1){
				    		advancedDisplay(dptr,dflag);
				    	}
				    	else{
				    		// If option "-d" is not required and if it's a directory the information will be green
				    		if(dflag==0 && isFolder(dptr->d_name)){
					    		printf("\033[32m");
								printf("%s ",dptr->d_name);
						    	printf("\033[0m");
					    	}
						    else printf("%s ",dptr->d_name);
				    	}
				    }
				    printf("\n");
				    closedir(dirp);
				}
			}
			// Else an error is displayed
			else{
				printf("ls : directory %s doesn't exist\n",argv[index]);
			}
		}
	}
      
	return 0;
}
