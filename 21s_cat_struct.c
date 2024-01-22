#include <stdio.h>
#include <getopt.h> /*подключение библиотеки для использования функций getopt_long и структуры option*/
#include <stdlib.h>


/*создание структуры для флагов*/
typedef struct {
    int b;
    int e;
    int n;
    int v;
    int s;
    int t;
    
} options;



void get_options(int argc, char* argv[], options *opts);

int main(int argc, char* argv[]){
    options opts;
    get_options(argc, argv, &opts);
    
    return 0;
}
/*функция парсинга флагов(включая длиные флаги)*/
void get_options(int argc, char* argv[], options *opts){
    int opt;
    static struct option long_options[] = { /*что бы определять длинные и короткие флаги в структуретзначение val устанавливаем значение коротких параметров*/
        {"number-nonblank", no_argument, 0, 'b'},
        {"number", no_argument, 0, 'n'},
        {"squeeze-blank", no_argument, 0, 's'},
        {0, 0, 0, 0}
    }; 
  
    while((opt = getopt_long(argc, argv, "+bevntTE", long_options, NULL) != -1)){
        switch (opt) /*обработка результатов работы функции*/
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
            printf("Error");
            exit(1);
            break;
        }
    } 
}