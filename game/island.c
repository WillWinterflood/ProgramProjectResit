#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS 0  //Game runs coorectly 
#define EXIT_ARG_ERROR 1  //Bad number of arguments or bad dimension
#define EXIT_FILE_ERROR 2 //File cannot be read 
#define EXIT_DATA_ERROR 3 //File is not a valid map 

void checkFile(const char *Filename) {
    FILE *file;
    
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
            fprintf(stderr, "Length of line bigger than buffer size \n");
            exit(EXIT_DATA_ERROR);
       }
       int lineLength = strlen(line_buffer);
       width = lineLength;

       if (line_buffer[lineLength - 1] == '\n') {
            line_buffer[lineLength - 1] == '\0';
            lineLength--; 
       }

       for (int i = 0; i < lineLength; i++) {
            char ch = line_buffer[i];
            if (ch != 'S' && ch != 'w' && ch != ' ' && ch != 'T' && ch != 'H' && ch != '\n') { //Checks the valid characters in the file.
                fprintf(stderr, "Incorrect chars in this file\n");
                exit(EXIT_DATA_ERROR);
            }
        }
    
        printf("%s", line_buffer);
        
    }
    fclose(file);
}

int main (int argc, char *argv[]) {
    FILE *file;

    if (argc != 2) {
        fprintf(stderr, "Usage: island <filename>");
        return EXIT_ARG_ERROR;  
    }
    checkFile(argv[1]);
    return EXIT_SUCCESS;

}