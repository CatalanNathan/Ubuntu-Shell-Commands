/*
** CatalanNathan Project 2022
** Ubuntu-Shell-Commands
** File description:
** ls.h
*/

#ifndef LS_H_
    #define LS_H_
    #include <dirent.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <pwd.h>
    #include <grp.h>
    #include <time.h>
    #include <fcntl.h>

typedef struct flags_s {
    int flag_A;
    int flag_a;
    int flag_l;
    int flag_R;
    int flag_d;
    int flag_r;
    int flag_t;
} flags_t;

#endif /* !LS_H_ */
