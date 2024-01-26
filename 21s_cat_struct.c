#include <stdio.h>
#include <getopt.h> /*подключение библиотеки для использования функций getopt_long и структуры option*/
#include <stdlib.h>
#include <string.h>

/*создание структуры для флагов*/
typedef struct
{
    int b;
    int e;
    int n;
    int v;
    int s;
    int t;
} options;

/*функция парсинга флагов(включая длиные флаги)*/
void get_options(int argc, char *argv[], options *opts)
{
    int opt;
    static struct option long_options[] = {/*определение длинных флагов и перевод их в короткие*/
                                           {"number-nonblank", no_argument, 0, 'b'},
                                           {"number", no_argument, 0, 'n'},
                                           {"squeeze-blank", no_argument, 0, 's'},
                                           {0, 0, 0, 0}};
    int options_index;
    while ((opt = (getopt_long(argc, argv, "+bevnstTE", long_options, &options_index))) != -1) /*установка коротких флагов в структуре*/
    {

        switch (opt)
        {
        case 'b':
            opts->b = 1;
            break;
        case 's':
            opts->s = 1;
            break;
        case 'e':
            opts->e = 1;
            opts->v = 1;
            break;
        case 'v':
            opts->v = 1;
            break;
        case 'n':
            opts->n = 1;
            break;
        case 't':
            opts->t = 1;
            opts->v = 1;
            break;
        case 'T':
            opts->t = 1;
            break;
        case 'E':
            opts->e = 1;
            break;
        default:
            printf("Error\n");
            exit(1); /*выход из программы если неверный флаг*/
            break;
        }
    }
}

void get_files(int argc, char *argv[], char **files_mass, int *count_index)
{

    for (int i = 0; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {

            files_mass[*count_index] = malloc(strlen(argv[i]) * sizeof(char)); /*выделение памяти под название файлов*/
            files_mass[*count_index] = argv[i];                                /*заполнение масива названием файлов*/
            (*count_index)++;
        }
    }
}

void getoutput(options *opts, FILE *files)
{
    char temp[2] = {-1, -1};
    int count_enter = 0;
    int moment_s = 0; // переменная показывающая применение флага -s на следующем шаге
    char c;
    int line_number = 1;
    while ((c = getc(files)) != EOF)
    {
        temp[0] = temp[1];
        temp[1] = c;

        if (opts->s == 1) // убирает пробелы больше двух
        {
            if (temp[0] == -1 && temp[1] == '\n' && count_enter == 0) // для первой строки
                moment_s = 1;
            else if (temp[0] == '\n' && temp[1] == '\n' && !moment_s) // если две строки подряд с переносом стрки но флаг -s не включен
                moment_s = 1;
            else if (temp[1] != '\n') // если  не перенос строки то выключаем флаг -s
                moment_s = 0;
            else if (temp[0] == '\n' && temp[1] == '\n' && moment_s)
            { // если две предидущие с переносом стоки и флаг -s включен, пропускаем
                moment_s = 1;
                continue;
            }
        }
        if (opts->b == 1) // нумерует не пустые строки
        {
            if (count_enter == 0 && line_number == 1 && c != '\n')
            {
                printf("%6d\t", line_number);
                line_number++;
            }
            else if (c != '\n' && temp[0] == '\n')
            {
                printf("%6d\t", line_number);
                line_number++;
            }
        }
        else if (opts->n == 1) // нумерует все строки (с пробелом тоже)
        {
            if (count_enter == 0 && line_number == 1)
            {
                printf("%6d\t", line_number);
                line_number++;
            }
            if (temp[0] == '\n')
            {
                printf("%6d\t", line_number);
                line_number++;
            }
        }

        if (opts->v == 1) // меняет все спец символы на ^|
        {
            if ((c >= 0 && c < 32 && c != 10 && c != 9) || c == 127)
            {
                if (c == 127)
                {
                    printf("^?");
                    continue;
                }
            }
            else
            {
                printf("^%c", (c + 64));
                continue;
            }
        }
        if (opts->e == 1)
        {
            if (c == '\n')
                printf("$");
        }
        if (opts->t == 1)
        {
            if (c == '\t')
            {
                printf("^|");
                continue;
            }
        }
        printf("%c", c);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    options opts;
    int count_index = 0;
    char **file_mass = malloc(argc * sizeof(char *));
    get_options(argc, argv, &opts);
    get_files(argc, argv, file_mass, &count_index);
    for (int i = 1; i < count_index; i++)
    {
        FILE *file = fopen(file_mass[i], "r");
        getoutput(&opts, file);
    }
    for (int i=0; i<count_index; i++){
        free(file_mass[i]);
    }
    free(file_mass);
    printf("s = %d\n", opts.s);
    printf("b = %d\n", opts.b);
    printf("n = %d\n", opts.n);
    printf("v = %d\n", opts.v);
    printf("e = %d\n", opts.e);
    printf("t = %d\n", opts.t);
    return 0;
}
