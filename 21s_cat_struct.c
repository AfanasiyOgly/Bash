#include <stdio.h>
#include <getopt.h> /*подключение библиотеки для использования функций getopt_long и структуры option*/
#include <stdlib.h>
#include <string.h>
#define MAX_FILES 10
#define MAX_FILENAME_LENGTH 256

/*создание структуры для флагов*/
typedef struct {
    int b;
    int e;
    int n;
    int v;
    int s;
    int t;
    
} options;


/*функция парсинга флагов(включая длиные флаги)*/
void get_options(int argc, char* argv[], options *opts){
    int opt;
    static struct option long_options[] = { 
        {"number-nonblank", no_argument, 0, 'b'},
        {"number", no_argument, 0, 'n'},
        {"squeeze-blank", no_argument, 0, 's'},
        {0, 0, 0, 0}
    }; 
    int options_index;
    while((opt = (getopt_long(argc, argv, "+bevntTE", long_options, &options_index))) != -1){
        printf("%d", opt);
        switch (opt) 
        {
        case 'b':
            opts -> b = 1;
            break;
        case 'e':
            opts -> e = 1;
            opts -> v = 1;
            break;
        case 'v':
            opts -> e = 1;
            break;
        case 'n':
            opts -> n = 1;
            break;
        case 't':
            opts -> t = 1;
            opts -> v = 1;
            break;
        case 'T':
            opts -> v = 1;
            break;
        case 'E':
            opts -> v = 1;
            break;        
        default:
            printf("Error\n");
            break;
        }
    } 
}

void get_files(int argc, char* argv[], char** files_mass, int* count_index){
   
    for (int i=0; i<argc && *count_index < MAX_FILES; i++){
        if (argv[i][0] != '-'){
            files_mass[*count_index] = malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(files_mass[*count_index], argv[i]);
            (*count_index)++;
        }
    }
}

int main(int argc, char* argv[]){
    options opts;
    int count_index = 0;
    char** file_mass = malloc(10 * sizeof(char*)); // Выделение памяти под 10 указателей на char
    for (int i = 0; i < 10; i++) {
        file_mass[i] = malloc(256 * sizeof(char)); // Выделение памяти под каждый указатель
        if (file_mass[i] == NULL) {
            printf("Не удалось выделить память\n");
            return 1;
        }
    }
    get_options(argc, argv, &opts);
    get_files(argc, argv, file_mass, &count_index);
    for (int i=0; i<count_index; i++){
        printf("%s\n", file_mass[i]);
    }
    for (int i = 0; i < 10; i++) {
        free(file_mass[i]);
    }    
    free(file_mass);
    printf("%d\n", opts.b);
    printf("%d\n", opts.e);
    printf("%d\n", opts.v);
    printf("%d\n", opts.n);
    printf("%d\n", opts.t);
    return 0;
}

