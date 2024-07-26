#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS 0  //Game runs coorectly 
#define EXIT_ARG_ERROR 1  //Bad number of arguments or bad dimension
#define EXIT_FILE_ERROR 2 //File cannot be read 
#define EXIT_DATA_ERROR 3 //File is not a valid map 

void checkFile(FILE *file, char *Filename) {
    
    file = fopen(Filename, "r");

    int buffer_size = 101;
    char line_buffer[buffer_size];

    int height = 0;
    int width = 0;

    if (file == NULL) {
        exit(EXIT_FILE_ERROR);
    }

    while (fgets(line_buffer, buffer_size, file) != NULL) { 
       
       if (strlen(line_buffer) >= buffer_size - 1) {  //compares the bufferSize and the length of the line to ensure there is no segmentation fault. 
            printf("Length of line bigger than \n");
            exit(EXIT_DATA_ERROR);
       }
       int lineLength = strlen(line_buffer);
       width = lineLength;

       for (int i = 0; i < lineLength; i++) {
            char ch = line_buffer[i];
            if (ch != 'S' && ch != 'w' && ch != ' ' && ch != 'T' && ch != 'H' && ch != '\n') { //Checks the valid characters in the file.
                printf("Incorrect chars in this file");
                exit(EXIT_DATA_ERROR);
            }
       }
    }

}

int main (int argc, char *argv[]) {
    char Filename[101];
    FILE *file;

    if (argc != 2) {
        printf("Usage: island <filename>");
        return EXIT_ARG_ERROR;  
    }




}