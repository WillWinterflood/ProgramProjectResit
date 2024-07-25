#include <stdio.h>
#include <stdlib.h>

#define EXIT_SUCCESS 0
#define EXIT_ARG_ERROR 1
#define EXIT_FILE_ERROR 2
#define EXIT_MAZE_ERROR 3

void checkFile(FILE *file, char *Filename) {
    
}

int main (int argc, char *argv[]) {
    char Filename[101];
    FILE *file;

    if (argc != 2) {
        printf("Usage: maze <filename>");
        return EXIT_ARG_ERROR;  
    }




}