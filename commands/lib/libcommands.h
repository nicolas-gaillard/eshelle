#ifndef _LIBCOMMANDS_H
#define _LIBCOMMANDS_H 1

extern int ls(int __argc, char* __argv[]);
extern int cat(int __argc, char* __argv[]);
extern int cd(int __argc, char* __argv[]);
extern int local_mkdir(int __argc, char* __argv[]);
extern int pwd(int __argc, char* __argv[]);
extern int du(int __argc, char* __argv[]);
extern int rm(int __argc, char* __argv[]);
extern int cp(int __argc, char* __argv[]);
extern int local_chmod(int __argc, char* __argv[]);
extern int local_chown(int __argc, char* __argv[]);
extern int chgrp(int __argc, char* __argv[]);
extern int echo(int __argc, char* __argv[]);

#endif