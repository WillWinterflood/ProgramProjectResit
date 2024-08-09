#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXDIM 100
#define MINDIM 5

#define EXIT_SUCCESS 0  // Game runs correctly
#define EXIT_ARG_ERROR 1  // Bad number of arguments or bad dimension
#define EXIT_FILE_ERROR 2 // File cannot be read
#define EXIT_DATA_ERROR 3 // File is not a valid map

typedef struct {
    int x;
    int y;
    int treasuresCollected;
} Coordinates;

typedef struct {
    char **map;
    int height;
    int width;
    Coordinates start;
} Island;

void checkFile(const char *Filename, Island *island) {
    FILE *file;
    file = fopen(Filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Error: File cannot be read\n");
        exit(EXIT_FILE_ERROR);
    }

    int buffer_size = 101;
    char line_buffer[buffer_size];
    island->height = 0;
    island->width = 0;

    while (fgets(line_buffer, buffer_size, file) != NULL) {
        int lineLength = strlen(line_buffer);
        if (line_buffer[lineLength - 1] == '\n') {
            line_buffer[lineLength - 1] = '\0';
            lineLength--;
        }

        if (island->width == 0) {
            island->width = lineLength;
        } else if (lineLength != island->width) {
            fprintf(stderr, "The file is not square\n");
            exit(EXIT_DATA_ERROR);
        }

        for (int i = 0; i < lineLength; i++) {
            char ch = line_buffer[i];
            if (ch != 'S' && ch != 'w' && ch != ' ' && ch != 'T' && ch != 'H') {
                fprintf(stderr, "Incorrect characters in the file\n");
                exit(EXIT_DATA_ERROR);
            }
        }
        island->height++;
    }
    
    rewind(file); //Go back to the top of the text file

    int currentLine = 0; //Ensuring we start from line 0
    while (fgets(line_buffer, buffer_size, file) != NULL) {
        int lineLength = strlen(line_buffer);
        if (lineLength > 0 && line_buffer[lineLength - 1] == '\n') {
            line_buffer[lineLength - 1] = '\0';
            lineLength--;
        }
        if (currentLine == 0 || currentLine == island->height - 1) { //Checking top and bottom border
            for (int i = 0; i < lineLength; i++) {
                if (line_buffer[i] != 'w') {
                    fprintf(stderr, "Border not all water!\n");
                    exit(EXIT_DATA_ERROR);
                }
            }
        } 
        else {
            if (line_buffer[0] != 'w' || line_buffer[lineLength - 1] != 'w') { //Checkking left and right border
                fprintf(stderr, "Border not all water!\n");
                exit(EXIT_DATA_ERROR);
            }
        }
    currentLine++;
    }




    fclose(file);

    if (island->width < MINDIM || island->height < MINDIM || island->width > MAXDIM || island->height > MAXDIM) {
        fprintf(stderr, "Width and height of map are too big or small!\n");
        exit(EXIT_DATA_ERROR);
    }

    if (island->width != island->height) {
        fprintf(stderr, "The island isn't square!\n");
        exit(EXIT_DATA_ERROR);
    }
}

void allocateMemory(Island *island) { https://chatgpt.com/share/1ed88aff-2f17-4877-8a2d-84ff537ab962 
    //I used ChatGPT to help me understand when the correct and incorrect time is to use a malloc function.
    island->map = (char **)malloc(island->height * sizeof(char *));
    if (island->map == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FILE_ERROR);
    }

    for (int i = 0; i < island->height; i++) {
        island->map[i] = (char *)malloc((island->width + 1) * sizeof(char));
        if (island->map[i] == NULL) {
            fprintf(stderr, "Memory allocation failed!\n");
            exit(EXIT_FILE_ERROR);
        }
    }
}

void readMap(const char *Filename, Island *island) {
    FILE *file = fopen(Filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file!\n");
        exit(EXIT_FILE_ERROR);
    }

    int buffer_size = 101;
    char line_buffer[buffer_size];
    int currentLine = 0;

    while (fgets(line_buffer, buffer_size, file) != NULL) {
        int length = strlen(line_buffer);
        if (length > 0 && line_buffer[length - 1] == '\n') {
            line_buffer[length - 1] = '\0';
        }

        strncpy(island->map[currentLine], line_buffer, island->width);
        island->map[currentLine][island->width] = '\0';
        currentLine++;
    }

    fclose(file);
}

void initialiseIsland(Island *island, Coordinates *player) {
    int startCount = 0;
    int treasureCount = 0;

    for (int i = 0; i < island->height; i++) {
        for (int j = 0; j < island->width; j++) {
            if (island->map[i][j] == 'S') {
                island->start.x = i;
                island->start.y = j;

                player->x = i;
                player->y = j;
                
                player->treasuresCollected = 0;
                
                startCount++;
            }
            if (island->map[i][j] == 'H') {
                
                treasureCount++;
            }
        }
    }

    if (startCount == 0) {
        fprintf(stderr, "No starts present!\n");
        exit(EXIT_DATA_ERROR);
    } 
    else if (startCount > 1) {
        fprintf(stderr, "Too many starts present!\n");
        exit(EXIT_DATA_ERROR);
    }

    if (treasureCount != 3) {
        fprintf(stderr, "There has to be 3 treasures!\n");
        exit(EXIT_DATA_ERROR);
    }
}

void displayMap(Island *island, Coordinates *player) {
    for (int i = 0; i < island->height; i++) {
        
        for (int j = 0; j < island->width; j++) {

            if (i == player->x && j == player->y) {
                printf("X");
            } 
            else {
                printf("%c", island->map[i][j]);
            }
        }
        printf("\n");
    }
}

void movePlayer(char move, Coordinates *player, Island *island) {
    int X = player->x; //declaring them as X and Y to make it easier to type
    //I change them back at the end of the function
    int Y = player->y;

    switch (move) {
        case 'a':
        case 'A':
            Y--;
            break;
        case 'w':
        case 'W':
            X--;
            break;
        case 's':
        case 'S':
            
            X++;
            break;
        case 'd':
        case 'D':
            Y++;

            break;
    }

    char nextMove = island->map[X][Y];

    if (nextMove == 'w') {
        printf("Can't go into water!\n");

    } 
    else if (nextMove == 'T') {
        printf("Can't go into trees!\n");
    } 
    else if (nextMove == 'H') {
        player->treasuresCollected++;
        printf("You have collected %d treasure(s)\n", player->treasuresCollected);
        island->map[X][Y] = ' '; // When one treasure is collected, it disappears to ensure you cannot collect it again and cheat.
        if (player->treasuresCollected == 3) { // When all 3 treasures are collected, the game ends.
            printf("Congratulations! You have completed the game!\n");
            exit(EXIT_SUCCESS);
        }
    } 
    else {
        player->x = X;
        player->y = Y;
    }
}

int main(int argc, char *argv[]) {
    Island island;
    Coordinates player;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_ARG_ERROR;
    }

    checkFile(argv[1], &island);

    allocateMemory(&island);

    readMap(argv[1], &island);

    initialiseIsland(&island, &player);

    char move;
    while (1) {
        printf("Move using W, A, S, D and view the map using M:\n");
        scanf(" %c", &move);
        if (move == 'm' || move == 'M') {
            displayMap(&island, &player);
        } 
        else {
            movePlayer(move, &player, &island);
        }
    }

    for (int i = 0; i < island.height; i++) {
        free(island.map[i]); //Freeing the allocated memory, otherwise causes a SEGMENTATION FAULT
    }
    free(island.map);

    return EXIT_SUCCESS;
}