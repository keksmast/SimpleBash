#include <stdio.h>
#include <string.h>

typedef struct {
    int flag_b, flag_e, flag_n, flag_s, flag_t, flag_v;
} flag;

int parser(int argc, char **argv, flag *flags, int *index) {
    for (; *index < argc; (*index)++) {
        if (argv[*index][0] == '-') {
            for (int j = 1; argv[*index][j] != '\0'; j++) {
                switch (argv[*index][j]) {
                    case 'b': flags->flag_b = 1; break;
                    case 'e': flags->flag_v = 1; flags->flag_e = 1; break;
                    case 'E': flags->flag_e = 1; break;
                    case 'n': flags->flag_n = 1; break;
                    case 's': flags->flag_s = 1; break;
                    case 'T': flags->flag_t = 1; break;
                    case 't': flags->flag_v = 1; flags->flag_t = 1; break;
                    case 'v': flags->flag_v = 1; break;
                    default:
                        if (strcmp(argv[*index], "--number-nonblank") == 0) {
                            flags->flag_b = 1;
                            break;
                        } else if (strcmp(argv[*index], "--number") == 0) {
                            flags->flag_n = 1;
                            break;
                        } else if (strcmp(argv[*index], "--squeeze-blank") == 0) {
                            flags->flag_s = 1;
                            break;
                        } else {
                            printf("s21_cat: illegal option -- %c", argv[*index][j]);
                            return 1;
                        }
                }
            }
        } else {
            break;
        }
        if (flags -> flag_b)
            flags -> flag_n = 0;
    }
    return 0;
}

int textout(int argc, char **argv, flag flags, int index) {
    FILE *file = NULL;
    for (; index < argc; index++) {
        if ((file = fopen(argv[index], "r")) == NULL) {
            printf("s21_cat: %s: No such file or directory", argv[index]);
        } else {
            int last_char = '\n', count_of_str = 1, ent_count = 1;
            while (1) {
                int tmp = getc(file);
                if (tmp != 10) {
                    ent_count = 1;
                }
                if (tmp == EOF) {
                    break;
                }
                if (flags.flag_s && last_char == '\n' && tmp == '\n') {
                    if (ent_count) {
                        ent_count--;
                    } else {
                        continue;
                    }
                }
                if (flags.flag_b && last_char == '\n' && tmp != '\n') {
                    printf("%6d\t", count_of_str);
                    count_of_str++;
                } else if (flags.flag_n && last_char == '\n') {
                    printf("%6d\t", count_of_str);
                    count_of_str++;
                }
                if (flags.flag_t && tmp == '\t') {
                    printf("^I");
                    last_char = tmp;
                    continue;
                }
                if (flags.flag_e && tmp == '\n') {
                    printf("$");
                }
                if (flags.flag_v && (tmp == 127 || (tmp < 32 && tmp != 10 && tmp != 9))) {
                    if (tmp == 127) {
                        tmp -= 64;
                    } else if (tmp >= 0) {
                        tmp += 64;
                    } else {
                        tmp += 128;
                    }
                    printf("^");
                }
                printf("%c", tmp);
                last_char = tmp;
            }
            fclose(file);
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    flag flags = {
            .flag_b = 0,
            .flag_e = 0,
            .flag_n = 0,
            .flag_s = 0,
            .flag_t = 0,
            .flag_v = 0
    };
    int index = 1;
    if (parser(argc, argv, &flags, &index) == 0) {
        textout(argc, argv, flags, index);
    } else {
        printf("\nusage: s21_cat [-benstuv] [file ...]");
    }
    return 0;
}
