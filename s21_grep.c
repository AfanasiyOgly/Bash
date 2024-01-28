#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s21_grep
{
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
    int f;
    int o;

} options;

void get_options(int argc, char *argv[], options *opts, char **templates, int *temlates_count, char **templates_files, int *temlates_files_count)
{
    int opt;
    static struct option long_options[] = {
        /*длинных флагов нет поэтому зануляем лонг*/
        {0, 0, 0, 0}};
    int options_index;
    while ((opt = (getopt_long(argc, argv, "e:ivclnhsf:o", long_options,
                               &options_index))) !=
           -1) /*установка коротких флагов в структуре*/
    {
        switch (opt)
        {
        case 'e':
            opts->e = 1;
            templates[*temlates_count] = optarg;
            temlates_count++;
            break;
        case 'i':
            opts->i = 1;
            break;
        case 'v':
            opts->v = 1;
            break;
        case 'c':
            opts->c = 1;
            break;
        case 'l':
            opts->l = 1;
            break;
        case 'n':
            opts->n = 1;
            break;
        case 'h':
            opts->h = 1;
            break;
        case 's':
            opts->s = 1;
            break;
        case 'f':
            opts->f = 1;
            templates_files[*temlates_files_count] = optarg;
            temlates_files_count++;
            break;
        case 'o':
            opts->o = 1;
            break;
        default:
            printf("Error\n");
            exit(1); /*выход из программы если неверный флаг*/
            break;
        }
    }
    for (int i = 0; i < *temlates_files_count; i++)
    {
        FILE *file = NULL;
        file = fopen(templates_files[i], "r");
        if (file == NULL)
        {
            fprintf(stderr, "Error - file NULL");
            exit(1); /*выход из программы если файл пустой*/
        }
        else
            fclose(file);
    }
}

int main(int argc, char *argv[])
{
    char *templates[argc];
    int temlates_count = 0;
    char *templates_files[argc];
    int temlates_files_count = 0;
    options opts;
    memset(&opts, 0, sizeof(opts)); // инициализация структуры нулями
    get_options(argc, argv, &opts, templates, &temlates_count, templates_files, &temlates_files_count);
    for (int i = 0; i < temlates_count; i++)
    {
        printf("templates = %s\n", templates[i]);
    }
    for (int i = 0; i < temlates_files_count; i++)
    {
        printf("temlaates_files = %s", templates_files[i]);
    }
    printf("templates_files_count = %d\n", temlates_files_count);
    printf("templates_count = %d\n", temlates_count);
    printf("e = %d\n", opts.e);
    printf("i = %d\n", opts.i);
    printf("v = %d\n", opts.v);
    printf("c = %d\n", opts.c);
    printf("l = %d\n", opts.l);
    printf("n = %d\n", opts.n);
    printf("h = %d\n", opts.h);
    printf("s = %d\n", opts.s);
    printf("f = %d\n", opts.f);
    printf("o = %d\n", opts.o);

    return 0;
}