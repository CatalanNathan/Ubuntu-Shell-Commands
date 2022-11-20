/*
** CatalanNathan Project, 2022
** Ubuntu-Shell-Commands
** File description:
** ls
*/

#include "include/ls.h"

int my_strlen(char const *str)
{
    int i = 0;

    for (; str[i]; i++);
    return (i);
}

void write_message(char *message, int fd)
{
    write(fd, message, my_strlen(message));
}

char *add_two_string(char *str1, char *str2)
{
    int a = my_strlen(str1);
    int b = my_strlen(str2);
    char *str = malloc(a + b + 1);

    for (int i = 0; str1[i]; i++)
        str[i] = str1[i];
    for (int i = 0; str2[i]; i++)
        str[a + i] = str2[i];
    str[a + b] = '\0';
    return (str);
}

int arg_folder(char **argv)
{
    for (int i = 0; argv[i]; i++) {
        DIR *dir = NULL;
        if (((dir = opendir(argv[i])) != NULL || open(argv[i], O_RDONLY) > 0)
            && i > 0) {
            closedir(dir);
            return 1;
        }
    }
    return 0;
}

char *parse_n(char *str)
{
    int i = 0;
    char *new = malloc(sizeof(char) * (my_strlen(str) + 1));

    for (; str[i] != '\n'; i++)
        new[i] = str[i];
    new[i] = '\0';
    return new;
}

void flag_l(char *filepath, char *name)
{
    struct stat fileStat = { 0 };

    if (stat(filepath, &fileStat) < 0) {
        printf("Je passe pas dans le flag_l\n");
        return;
    }
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf(" %ld", fileStat.st_nlink);
    printf(" %s", getpwuid(fileStat.st_uid)->pw_name);
    printf(" %s", getgrgid(fileStat.st_gid)->gr_name);
    printf(" %ld", fileStat.st_size);
    printf(" %s ", parse_n(ctime(&fileStat.st_mtime)));
    puts(name);
}

flags_t check_flags(int argc, char **argv)
{
    flags_t flags = { 0 };

    for (int i = 1; argv[i]; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j]; j++) {
                if (argv[i][j] == 'A')
                    flags.flag_A = 1;
                else if (argv[i][j] == 'a')
                    flags.flag_a = 1;
                else if (argv[i][j] == 'l')
                    flags.flag_l = 1;
                else if (argv[i][j] == 'R')
                    flags.flag_R = 1;
                else if (argv[i][j] == 'd')
                    flags.flag_d = 1;
                else if (argv[i][j] == 'r')
                    flags.flag_r = 1;
                else if (argv[i][j] == 't')
                    flags.flag_t = 1;
                else {
                    write_message("ls: invalid option -- '", 2);
                    write(2, &argv[i][j], 1);
                    write_message("'\n", 2);
                    write_message("Try 'ls --help' for more information.\n", 2);
                    exit(1);
                }
            }
        }
    }
    return flags;
}

char *add_slash(char *str)
{
    char *new = malloc(my_strlen(str) + 2);
    int i = 0;

    for (; str[i]; i++)
        new[i] = str[i];
    if (str[i - 1] != '/')
        new[i] = '/';
    new[i + 1] = '\0';
    return new;
}

void readdir_with_flag(DIR *dir, struct dirent *file, flags_t all_flags, char *path)
{
    while ((file = readdir(dir)) != NULL) {
        if (all_flags.flag_l && (file->d_name[0] != '.' || all_flags.flag_a)) {
            flag_l(add_two_string(path, file->d_name), file->d_name);
            continue;
        }
        if (!all_flags.flag_a && file->d_name[0] == '.')
            continue;
        if (all_flags.flag_l && all_flags.flag_A && file->d_name[0] == '.' && file->d_name[1] == '\0' ||
            all_flags.flag_A == 1 && file->d_name[0] == '.' &&
            file->d_name[1] == '.' && file->d_name[2] == '\0')
            continue;
        write_message(file->d_name, 1);
        write_message(" ", 1);
    }
    if (!all_flags.flag_l)
        write_message("\n", 1);
}

int main(int argc, char **argv)
{
    flags_t all_flags = check_flags(argc, argv);
    DIR *dir = NULL;
    struct dirent *file = NULL;
    struct stat file_stat = { 0 };

    if (!arg_folder(argv)) {
        dir = opendir(".");
        readdir_with_flag(dir, file, all_flags, "");
        closedir(dir);
    } else {
        for (int i = 1; argv[i]; i++) {
            if (argv[i][0] == '-')
                continue;
            if (stat(argv[i], &file_stat) < 0)
                continue;
            if (S_ISDIR(file_stat.st_mode)) {
                dir = opendir(argv[i]);
                readdir_with_flag(dir, file, all_flags, add_slash(argv[i]));
                closedir(dir);
            } else {
                flag_l(argv[i], argv[i]);
            }
        }
    }
    return 0;
}
