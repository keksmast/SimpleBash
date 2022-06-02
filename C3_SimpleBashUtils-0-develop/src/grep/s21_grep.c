#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

typedef struct {
    int flag_i, flag_n, flag_s, flag_c, flag_v,
            flag_l, flag_h, flag_o, num_of_patterns, num_of_files;
    char **patterns, **files;
} flag_matr;

void read_flag_e(char *str,  flag_matr *matrix) {
    matrix -> patterns = (char**)realloc(matrix -> patterns, (matrix -> num_of_patterns + 1) * sizeof(char*));
    matrix -> patterns[matrix -> num_of_patterns] = (char*)malloc(strlen(str) +1);
    strcpy(matrix -> patterns[matrix -> num_of_patterns], str);
    matrix -> num_of_patterns++;
}

int read_flag_f(char *str, flag_matr *matrix) {
    FILE *file;
    int err = 0;
    if ((file = fopen(str, "r")) == NULL) {
        printf("s21_grep: %s: No such file or directory", str);
        err++;
    } else {
        while (1) {
            char pattern_str[100];
            if (fgets(pattern_str, 100, file) == NULL) break;
            if (pattern_str[strlen(pattern_str)-1] == '\n') pattern_str[strlen(pattern_str)-1] = '\0';
            read_flag_e(pattern_str, matrix);
        }
        fclose(file);
    }
    return err;
}

void read_file_name(char *str,  flag_matr *flags) {
    flags -> files = (char**)realloc(flags -> files, (flags -> num_of_files + 1) * sizeof(char*));
    flags -> files[flags -> num_of_files] = (char*)malloc(strlen(str) +1);
    strcpy(flags -> files[flags -> num_of_files], str);
    flags -> num_of_files++;
}

int parser(int argc, char **argv, flag_matr *flags) {
    int err_num = 0, flags_e_f = 0;
    for (int i = 1; i < argc && err_num == 0; i++) {
        if (strcmp(argv[i - 1], "-e") == 0 || strcmp(argv[i - 1], "-f") == 0) continue;
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j] != '\0' && err_num == 0; j++) {
                switch (argv[i][j]) {
                    case 'i': flags->flag_i = 1; break;
                    case 'e': {
                        read_flag_e(argv[i + 1], flags);
                        flags_e_f++;
                        break;
                    }
                    case 'n': flags->flag_n = 1; break;
                    case 's': flags->flag_s = 1; break;
                    case 'c': flags->flag_c = 1; break;
                    case 'v': flags->flag_v = 1; break;
                    case 'l': flags->flag_l = 1; break;
                    case 'h': flags->flag_h = 1; break;
                    case 'f': {
                        err_num = read_flag_f(argv[i + 1], flags);
                        flags_e_f++;
                        break;
                    }
                    case 'o': flags->flag_o = 1; break;
                    default: printf("%s: invalid option -- %c", argv[0], argv[i][j]); return 2;
                }
            }
        } else {
            if (!flags_e_f) {
                read_flag_e(argv[i], flags);
                flags_e_f++;
            } else {
                read_file_name(argv[i], flags);
            }
        }
    }
    if (flags->flag_c || flags->flag_l) flags->flag_o = 0;
    return err_num;
}

void print_ent(int *num) {
    if (*num == 0)
        (*num)++;
    else
        printf("\n");
}

char *return_clear_str(char *str) {
    int len = strlen(str);
    char *tmp = malloc(len);
    if (str[0] == '^') {
        for (int i = 0; i < len; ++i) {
            tmp[i] = str[i + 1];
        }
    } else {
        strcpy(tmp, str);
    }
    return tmp;
}

void flag_c_works(flag_matr *flags, char *file_name, int c_count) {
    if (flags->flag_c) {
        if (flags->num_of_files > 1) {
            if (!flags->flag_h) printf("%s:", file_name);
            printf("%d", c_count);
        } else {
            printf("%d", c_count);
        }
        printf("\n");
    }
}
void flag_o_works(flag_matr *flags, int *n_p, int fl_n_cou, char *pattern, char *file_name,
                  char *str, int flag_o_is_works) {
    if (!flags->flag_v) print_ent(n_p);
    if (flag_o_is_works == 0) {
        if (!flags->flag_h && flags->num_of_files > 1) printf("%s:", file_name);
        if (flags->flag_n) printf("%d:", fl_n_cou);
    }
    if (!flags->flag_v) {
        char *tmp = return_clear_str(pattern);
        printf("%s", tmp);
    } else {
        printf("%s", str);
    }
}

void print_text(char **argv, flag_matr *flags) {
    regex_t regex;
    int num_of_prints = 0;
    for (int i = 0; i < flags->num_of_files; i++) {
        FILE *file;
        if ((file = fopen(flags->files[i], "r")) == NULL) {
            if (!flags->flag_s) {
                print_ent(&num_of_prints);
                printf("%s: %s: No such file or directory", argv[0], flags->files[i]);
                if (flags->num_of_files > 1) print_ent(&num_of_prints);
            }
        } else {
            char tmp_str[200];
            int flag_n_count = 0, flag_c_count = 0;
            while (fgets(tmp_str, 200, file) != NULL) {
                int flag_l_is_works = 0, flag_o_cou = 0;
                flag_n_count++;
                for (int j = 0; j < flags->num_of_patterns; j++) {
                    int val;
                    if (flags->flag_i)
                        val = regcomp(&regex, flags->patterns[j], REG_ICASE);
                    else
                        val = regcomp(&regex, flags->patterns[j], 0);
                    if (!val || flags->patterns[j][0] == '\0') {
                        val = regexec(&regex, tmp_str, 0, NULL, 0);
                        regfree(&regex);
                        if (flags->patterns[j][0] == '\0') val = 0;
                        if (val == 0) {
                            if (!flags->flag_v) {
                                flag_c_count++;
                                if (!flags->flag_c) {
                                    if (flags->flag_o) {
                                        flag_o_works(flags, &num_of_prints, flag_n_count,
                                                     flags->patterns[j], flags->files[i],
                                                     tmp_str, flag_o_cou);
                                        flag_o_cou++;
                                    } else {
                                        if (!flags->flag_l) {
                                            if (flags->flag_h) {
                                                if (flags->flag_n) printf("%d:", flag_n_count);
                                                printf("%s", tmp_str);
                                            } else {
                                                if (flags->num_of_files > 1) printf("%s:", flags->files[i]);
                                                if (flags->flag_n) printf("%d:", flag_n_count);
                                                printf("%s", tmp_str);
                                            }
                                        } else {
                                            printf("%s\n", flags->files[i]);
                                            flag_l_is_works = 1;
                                        }
                                    }
                                }
                                if (!flags->flag_o) {
                                    break;
                                }
                            }
                        } else {
                            if (flags->flag_v) {
                                flag_c_count++;
                                if (!flags->flag_c) {
                                    if (flags->flag_o) {
                                        flag_o_works(flags, &num_of_prints, flag_n_count,
                                                     flags->patterns[j], flags->files[i],
                                                     tmp_str, flag_o_cou);
                                    } else if (flags->flag_l) {
                                        print_ent(&num_of_prints);
                                        printf("%s", flags->files[i]);
                                        flag_l_is_works = 1;
                                    } else {
                                        if (flags->flag_h) {
                                            if (flags->flag_n) printf("%d:", flag_n_count);
                                            printf("%s", tmp_str);
                                        } else {
                                            if (flags->num_of_files > 1) printf("%s:", flags->files[i]);
                                            if (flags->flag_n) printf("%d:", flag_n_count);
                                            printf("%s", tmp_str);
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                if (flag_l_is_works) {
                    break;
                }
            }
            flag_c_works(flags, flags->files[i], flag_c_count);
        }
    }
    if (flags->flag_h) printf("\n");
}

void init_struct(flag_matr *flags_and_matrix) {
    flags_and_matrix -> flag_s = 0;
    flags_and_matrix -> flag_i = 0;
    flags_and_matrix -> flag_n = 0;
    flags_and_matrix -> flag_c = 0;
    flags_and_matrix -> flag_v = 0;
    flags_and_matrix -> flag_l = 0;
    flags_and_matrix -> flag_h = 0;
    flags_and_matrix -> flag_o = 0;
    flags_and_matrix -> num_of_patterns = 0;
    flags_and_matrix -> patterns = NULL;
    flags_and_matrix -> files = NULL;
    flags_and_matrix -> num_of_files = 0;
}

int main(int argc, char ** argv) {
    flag_matr flags_and_matrix;
    init_struct(&flags_and_matrix);
    int err_num = parser(argc, argv, &flags_and_matrix);
    if (err_num) {
        if (err_num == 2) {
            printf("\nusage: s21_grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] [-C[num]]\n"
                   "        [-e pattern] [-f file] [--binary-files=value] [--color=when]\n"
                   "        [--context[=num]] [--directories=action] [--label] [--line-buffered]\n"
                   "        [--null] [pattern] [file ...]");
        }
    } else {
        print_text(argv, &flags_and_matrix);
    }
    for (int i = 0; i < flags_and_matrix.num_of_patterns; i++) {
        free(flags_and_matrix.patterns[i]);
    }
    for (int i = 0; i < flags_and_matrix.num_of_files; i++) {
        free(flags_and_matrix.files[i]);
    }
    free(flags_and_matrix.files);
    free(flags_and_matrix.patterns);
    return 0;
}
