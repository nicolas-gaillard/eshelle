/* Author : Jordan BLOUIN */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * Method :
 *      main
 * Arguments :
 *      int argc -> number of arguments
 *      char *argv[] -> arguments
 * Description :
 *      Report a snapshot of the current processes
 **/
 int main(int argc, char *argv[])
 {
    printf("      PID        TTY            TIME            CMD\n");
    pid_t pid = getpid();
    pid_t ppid = getppid();
    pid_t sid = getsid(pid);
    char *tty = ttyname(2);
    char *nom;
    int temps;
    printf("    %5d     %10s  %d:%d:%d      %15s\n", sid, tty, temps, nom);
    printf("    %5d     %10s  %d:%d:%d      %15s\n", ppid, tty, temps, nom);
    printf("    %5d     %10s  %d:%d:%d      %15s\n", pid, tty, temps, nom);
    
    
 }