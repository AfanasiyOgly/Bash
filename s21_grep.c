#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
static const int OPTIONS_END = -1;
static const int PATTERNS_INIT = 1000;
static const int PATTERNS_ADD = 1000;
static const int BUFFER_INIT = 1000;
static const char *FOPEN_READ = "r";
static const int BUFFER_MULT = 1000;

struct Patterns
{
    char **data;
    size_t cur_size;
    size_t max_size;
};
typedef struct Patterns Patterns;

struct Options
{
    bool e;
    bool i;
    bool v;
    bool c;
    bool l;
    bool n;
    bool h;
    bool s;
    bool f;
    bool o;
    struct Patterns patts;
};
typedef struct Options Options;
static const char SHORTOPTIONS[] = "e:f:isvnholc";
static struct option LONGOPTIONS[] = {
    {"regexp", required_argument, NULL, 'e'},
    {"file", required_argument, NULL, 'f'},
    {"ignore-case", no_argument, NULL, 'i'},
    {"no-messages", no_argument, NULL, 's'},
    {"invert-match", no_argument, NULL, 'v'},
    {"line-number", no_argument, NULL, 'n'},
    {"no-filename", no_argument, NULL, 'h'},
    {"only-matching", no_argument, NULL, 'o'},
    {"file-witch-matches", no_argument, NULL, 'l'},
    {"count", no_argument, NULL, 'c'},
    {"help", no_argument, NULL, 0},
    {0, 0, 0, 0}};

/*функция выделения и проверки памяти*/
void *safe_malloc(const size_t size)
{
    void *ret_ptr = malloc(size);
    if (ret_ptr == NULL)
    {
        printf("ERROR");
        exit(1);
    }
    return ret_ptr;
}
/*функция добавления памяти*/
void *safe_realloc(void *buffer, const size_t size)
{
    void *ret_ptr = realloc(buffer, size);
    if (ret_ptr == NULL)
    {
        printf("ERROR no realloc\n");
        exit(1);
    }
    return ret_ptr;
}
static void patterns_add(Patterns *const patts, const char *const patt)
{
    if (patts->cur_size == patts->max_size)
    {
        patts->max_size += PATTERNS_ADD;
        patts->data = safe_realloc(patts->data, patts->max_size * sizeof(char *));
    }
    patts->data[patts->cur_size] = safe_malloc(sizeof(char) * strlen(patt) + sizeof(char));
    strcpy(patts->data[patts->cur_size++], patt);
}
FILE *safe_fopen(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    return file;
}
static void buffer_file(FILE *file, char *buffer)
{
    size_t size = 0;
    size_t max_size = BUFFER_INIT;
    char simbol = fgetc(file);
    while (!feof(file))
    {
        buffer[size++] = simbol;
        simbol = fgetc(file);
        if (size != max_size)
        {
            max_size *= BUFFER_MULT;
            buffer = safe_realloc(buffer, max_size * sizeof(char));
        }
    }
    buffer[size] = '\0';
}
static void patterns_add_from_string(Patterns *const patts, const char *const str)
{
    char *temp_patts = safe_malloc(sizeof(char) * strlen(str) + sizeof(char));
    strcpy(temp_patts, str);
    char *token = strtok(temp_patts, "\n");
    while (token != NULL)
    {
        patterns_add(patts, token);
        token = strtok(NULL, "\n");
    }
    free(temp_patts);
}
static void patterns_add_from_file(Patterns *const patts, char *const filename)
{
    FILE *file = safe_fopen(filename, FOPEN_READ);
    char *buffer = safe_malloc(sizeof(char) * BUFFER_INIT);
    buffer_file(file, buffer);
    patterns_add_from_string(patts, buffer);
    free(buffer);
    fclose(file);
}

static void set_options(const char option, Options *options)
{
    switch (option)
    {
    case 'e':
        options->e = true;
        patterns_add_from_string(&options->patts, optarg);
        break;
    case 'f':
        options->f = true;
        patterns_add_from_file(&options->patts, optarg);
        break;
    case 'i':
        options->i = true;
        break;
    case 's':
        options->s = true;
        break;
    case 'v':
        options->v = true;
        break;
    case 'n':
        options->n = true;
        break;
    case 'h':
        options->h = true;
        break;
    case 'o':
        options->o = true;
        break;
    case 'l':
        options->l = true;
        break;
    case 'c':
        options->c = true;
        break;
    default:
        printf("Error\n");
        exit(1);
    }
}

/*Освобождение структуры patts*/
static void patterns_free(Patterns *const patts)
{
    for (size_t i = 0; i < patts->cur_size; i++)
    {
        free(patts->data[i]);
    }
    free(patts->data);
}
static void options_free(Options *const opts) { patterns_free(&opts->patts); }
static void patterns_init(Patterns *const patts)
{
    patts->cur_size = 0;
    patts->max_size = PATTERNS_INIT;
    patts->data = safe_malloc(sizeof(char *) * patts->max_size);
}

void get_options(int argc, char *argv[], Options *opts)
{
    patterns_init(&opts->patts);
    int long_options_index = 0;
    char opt = getopt_long(argc, argv, SHORTOPTIONS, LONGOPTIONS, &long_options_index);
    while (opt != OPTIONS_END)
    {
        set_options(opt, opts);
        opt = getopt_long(argc, argv, SHORTOPTIONS, LONGOPTIONS, &long_options_index);
    }
}

int main(int argc, char *argv[])
{
    Options opts = {0};
    get_options(argc, argv, &opts);
    for (size_t i = 0; i < opts.patts.cur_size; i++)
    {
        puts(opts.patts.data[i]);
    }
    options_free(&opts);
    return 0;
}