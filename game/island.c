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

    int buffer_size = 101;
    char line_buffer[buffer_size];

    island->height = 0;
    island->width = 0;

    if (file == NULL) {
        exit(EXIT_FILE_ERROR);
    }

    while (fgets(line_buffer, buffer_size, file) != NULL) {
        if (strlen(line_buffer) >= buffer_size - 1) {
            fprintf(stderr, "Length of line bigger than buffer size\n");
            exit(EXIT_DATA_ERROR);
        }
        int lineLength = strlen(line_buffer);
        if (island->width == 0) {
            island->width = lineLength;
        }

        if (line_buffer[lineLength - 1] == '\n') {
            line_buffer[lineLength - 1] = '\0';
            lineLength--;
        }

        for (int i = 0; i < lineLength; i++) {
            char ch = line_buffer[i];
            if (ch != 'S' && ch != 'w' && ch != ' ' && ch != 'T' && ch != 'H') {
                fprintf(stderr, "Incorrect characters in this file\n");
                exit(EXIT_DATA_ERROR);
            }
        }

        island->height++;
    }
    fclose(file);

    if (island->width < MINDIM || island->height < MINDIM || island->width > MAXDIM || island->height > MAXDIM) {
        fprintf(stderr, "Width and height of map are too big or small!\n");
        exit(EXIT_DATA_ERROR);
    }
}

void allocateMemory(Island *island) {
    island->map = (char **)malloc(island->height * sizeof(char *));
    if (island->map == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_DATA_ERROR);
    }

    for (int i = 0; i < island->height; i++) {
        island->map[i] = (char *)malloc((island->width + 1) * sizeof(char));
        if (island->map[i] == NULL) {
            fprintf(stderr, "Memory allocation failed!\n");
            exit(EXIT_DATA_ERROR);
        }
    }
}

void readMap(const char *Filename, Island *island) {
    FILE *file = fopen(Filename, "r");
    int buffer_size = 101;
    char line_buffer[buffer_size];
    int currentLine = 0;

    while (fgets(line_buffer, buffer_size, file) != NULL) {
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
        fprintf(stderr, "No starts\n");
        exit(EXIT_DATA_ERROR);
    } else if (startCount > 1) {
        fprintf(stderr, "Too many starts\n");
        exit(EXIT_DATA_ERROR);
    }

    if (treasureCount != 3) {
        fprintf(stderr, "Too many or too little treasures!\n");
        exit(EXIT_DATA_ERROR);
    }
}

void displayMap(Island *island, Coordinates *player) {
    for (int i = 0; i < island->height; i++) {
        for (int j = 0; j < island->width; j++) {
            if (i == player->x && j == player->y) {
                printf("X");
            } else {
                printf("%c", island->map[i][j]);
            }
        }
        printf("\n");
    }
}

void movePlayer(char move, Coordinates *player, Island *island) {
    int X = player->x;
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

    if (X < 0 || X >= island->height || Y < 0 || Y >= island->width) {
        printf("Move out of bounds!\n");
        return;
    }

    char nextMove = island->map[X][Y];

    if (nextMove == 'w') {
        printf("Can't go into water!\n");
        return;
    } 
    else if (nextMove == 'T') {
        printf("Can't go into trees!\n");
        return;
    } 
    else if (nextMove == 'H') {
        player->treasuresCollected++;
        printf("You have collected %d treasure(s)\n", player->treasuresCollected);
        island->map[X][Y] = ' '; //When one treasure is collected, it disappears to ensure you cannot go off and on.  
        if (player->treasuresCollected == 3) { // When all 3 treasures are collected, the game ends. 
            printf("Congratulations! You have completed the game!\n");
            exit(EXIT_SUCCESS);
        }
    }

    player->x = X;
    player->y = Y;

}

int main(int argc, char *argv[]) {
    Island island;
    Coordinates player;

    if (argc != 2) {
        fprintf(stderr, "Usage: island <filename>\n");
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
            displayMap(&island, &player); // Only displays the map when 'm' or 'M' is inputted
        }
        else {
            printf("Havent found any treasure yet!\n");
        }
        movePlayer(move, &player, &island); 
    }

    for (int i = 0; i < island.height; i++) {
        free(island.map[i]); // Freeing the dynamically allocated memory
    }
    free(island.map);

    return EXIT_SUCCESS;
}