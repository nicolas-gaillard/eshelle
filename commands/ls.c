#include "option_folder.c"

#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <string.h>

#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>

int main(int argc,char *argv[]){
    //printf("------------------------------------------------------\n");
      
    DIR *dirp;
	struct dirent *dptr;
	struct stat st;
	
	struct passwd *userInfo;
	struct group *groupInfo;
	struct tm *timeInfo;
	
    if(argv[1]!=NULL){
	if(isOption(argv[1])){
	    if(strcmp(argv[1],"-l")==0 || strcmp(argv[1],"-long")==0){
		if(isFolder(argv[2])){
		    if((dirp=opendir(argv[2]))==NULL){
			printf("Error ! Le dossier %s n'existe pas ou plus !\n",argv[2]);
			return(-1);
		    }
		}
		else{
		    dirp=opendir("./");
		}
		while(dptr=readdir(dirp)){
		    //printf("%ld   %ld   %d   %s\n",dptr->d_ino,dptr->d_off,dptr->d_reclen,dptr->d_name);
		    //stat(dptr->d_name,&sts);
		    //printf("(st_uid,st_gid,st_ctime/st_mode) : %d   %d   %s   %o\n",sts.st_uid,sts.st_gid,ctime(&sts.st_ctime),sts.st_mode);
		    //printf("\n");
		    
		    lstat(dptr->d_name,&st);
		    if ((st.st_mode & S_IFSOCK)==S_IFSOCK) printf("s");	// socket
		    else if ((st.st_mode & S_IFLNK)==S_IFLNK) printf("l");	// symbolic link
		    else if ((st.st_mode & S_IFREG)==S_IFREG) printf("-");	// regular file
		    else if ((st.st_mode & S_IFBLK)==S_IFBLK) printf("b");	// block device
		    else if ((st.st_mode & S_IFDIR)==S_IFDIR) printf("d");	// directory
		    else if ((st.st_mode & S_IFCHR)==S_IFCHR) printf("c");	// character device
		    else if ((st.st_mode & S_IFIFO)==S_IFIFO) printf("p");	// FIFO
		    
		    printf("%c",(st.st_mode & S_IRUSR)==S_IRUSR ? 'r' : '-');	// owner R
		    printf("%c",(st.st_mode & S_IWUSR)==S_IWUSR ? 'w' : '-');	// owner W
		    printf("%c",(st.st_mode & S_IXUSR)==S_IXUSR ? 'x' : '-');	// owner X
		    printf("%c",(st.st_mode & S_IRGRP)==S_IRGRP ? 'r' : '-');	// group R
		    printf("%c",(st.st_mode & S_IWGRP)==S_IWGRP ? 'w' : '-');	// group W
		    printf("%c",(st.st_mode & S_IXGRP)==S_IXGRP ? 'x' : '-');	// group X
		    printf("%c",(st.st_mode & S_IROTH)==S_IROTH ? 'r' : '-');	// other R
		    printf("%c",(st.st_mode & S_IWOTH)==S_IWOTH ? 'w' : '-');	// other W
		    printf("%c",(st.st_mode & S_IXOTH)==S_IXOTH ? 'x' : '-');	// other X
		    
		    printf(" %d",st.st_nlink);

		    userInfo=getpwuid(st.st_uid);
		    printf(" %s",userInfo->pw_name);

		    groupInfo=getgrgid(st.st_gid);
		    printf(" %s",groupInfo->gr_name);

		    printf(" %5d",st.st_size);

		    timeInfo=localtime(&st.st_mtime);
		    printf(" %4d-%02d-%02d %02d:%02d",timeInfo->tm_year+1900,timeInfo->tm_mon+1,timeInfo->tm_mday,timeInfo->tm_hour,timeInfo->tm_min);

		    printf(" %s\n",dptr->d_name);
			  
		}
		printf("\n");
	    }
	    else{
		printf("Error : L'option %s n'existe pas pour cette commande !\n",argv[1]);
		return -1;
	    }
	}
	else if(isFolder(argv[1])){
	    if((dirp=opendir(argv[1]))==NULL){
		printf("Error ! Le dossier %s n'existe pas ou plus !\n",argv[1]);
		return(-1);
	    }
	    else{
		dirp=opendir("./");
	    }
	    while(dptr=readdir(dirp)){
		printf("%s ",dptr->d_name);
	    }
	    printf("\n");
        }
        else{
	    printf("Error ! L'argument %s n'est pas un dossier !\n",argv[1]);
	    return(-1);
	}
    }
    else{
	dirp=opendir("./");
	while(dptr=readdir(dirp)){
	    printf("%s ",dptr->d_name);
	}
	printf("\n");
    }
    closedir(dirp);
      
    //printf("------------------------------------------------------\n");
    return 0;
}
