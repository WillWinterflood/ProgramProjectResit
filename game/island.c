#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXDIM 100
#define MINDIM 5

#define EXIT_SUCCESS 0  //Game runs coorectly 
#define EXIT_ARG_ERROR 1  //Bad number of arguments or bad dimension
#define EXIT_FILE_ERROR 2 //File cannot be read 
#define EXIT_DATA_ERROR 3 //File is not a valid map 

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
    Coordinates end;
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
       
       if (strlen(line_buffer) >= buffer_size - 1) {  //compares the bufferSize and the length of the line to ensure there is no segmentation fault. 
            fprintf(stderr, "Length of line bigger than buffer size \n");
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
            if (ch != 'S' && ch != 'w' && ch != ' ' && ch != 'T' && ch != 'H') { //Checks the valid characters in the file.
                fprintf(stderr, "Incorrect chars in this file\n");
                exit(EXIT_DATA_ERROR);
            } //NOT WORKING RIGHT NOW 
        }

        island->height++;

        
    }
    fclose(file);

    if (island->width < MINDIM || island->height < MINDIM || island->width > MAXDIM || island->height > MAXDIM) {
        fprintf(stderr, "width and height of map are too big or small!\n");
        exit(EXIT_FILE_ERROR);
    }
}

void initialiseIsland (Island *island, Coordinates *player) {
    int startCount = 0;

    for (int i = 0; i < island->height; i++) {
        for (int j = 0; j < island->width; j++) {
            if (island->map[i][j] == 'S') {
                island->start.x = i;
                island->start.y = j;
                player->y = j;
                player->x = i;
                startCount++;
                if (startCount == 0) {
                    fprintf(stderr, "No starts\n");
                    exit(EXIT_DATA_ERROR);
                }
            }
        }
    }

    if (startCount > 1) { //Not working right now ??
        fprintf(stderr, "Too many starts\n");
        exit(EXIT_DATA_ERROR);
    }

}

void allocateMemory (Island *island, const char *Filename) {  //Dynamically allocating memory 
    FILE *file;

    file = fopen(Filename, "r");

    island->map = (char **)malloc(island->height * sizeof(char *));

    if (island->map == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_DATA_ERROR);
    }

    for (int i = 0; i < island->height; i++) {
        island->map[i] = (char *)malloc(island->width * sizeof(char));
        if (island->map[i] == NULL) {
            fprintf(stderr, "Memory allocation failed!\n");
            exit(EXIT_DATA_ERROR);
        }

    }
}

void movePlayer (char move, Coordinates *player, Island *island) {
    int buffer_size = 101;
    char line_buffer[buffer_size];
    FILE *file;

    switch (move) {
        case 'a':
        case 'A':
            if (island->map[player->x][player->y-1] == 'w') {
                printf("Cant go into water!\n");
                player->y == 0;
            }
            else if (island->map[player->x][player->y-1] == 'T') {
                printf("Cant go into trees!\n");
                player->y == 0;
            }
            else if (island->map[player->x][player->y-1] == 'H') {
                player->treasuresCollected++;
                printf("You have collected %d treasures \n", player->treasuresCollected);
                player->y -= 1;  
                if (player->treasuresCollected == 3) {
                    printf("Congratulations! You have completed the game!\n");
                    exit(EXIT_SUCCESS);
                }   
            }
            player->y -= 1;
        break;
        case 'w':
        case 'W':
            if (island->map[player->x+1][player->y] == 'w') {
                printf("Cant go into water!\n");
                player->x == 0;
            }
            else if (island->map[player->x+1][player->y] == 'T') {
                printf("Cant go into trees!\n");
                player->x == 0;
            }
            else if (island->map[player->x+1][player->y] == 'H') {
                player->treasuresCollected++;
                printf("You have collected treasures\n", player->treasuresCollected);
                player->x += 1;
                if (player->treasuresCollected == 3) {
                    printf("Congratulations! You have completed the game!\n");
                    exit(EXIT_SUCCESS);
                }    
            }
            player->x += 1;
        break;
        case 's':
        case 'S':
            if (island->map[player->x-1][player->y] == 'w') {
                printf("Cant go into water!\n");
                player->x == 0;
            }
            else if (island->map[player->x-1][player->y] == 'T') {
                printf("Cant go into trees!\n");
                player->x == 0;
            }
            else if (island->map[player->x-1][player->y] == 'H') {
                player->treasuresCollected++;
                printf("You have collected treasures \n", player->treasuresCollected);
                player->x -= 1;
                if (player->treasuresCollected == 3) {
                    printf("Congratulations! You have completed the game!\n");
                    exit(EXIT_SUCCESS);
                }        
            }
            player->x -= 1;
        case 'd':
        case 'D':
            if (island->map[player->x][player->y+1] == 'w' ) {
                printf("Cant go into water!\n");
                player->y == 0;
            }
            else if (island->map[player->x][player->y+1] == 'T') {
                printf("Cant go into trees!\n");
                player->y == 0;
            }
            else if (island->map[player->x][player->y+1] == 'H') {
                player->treasuresCollected++;
                printf("You have now collected treasures\n", player->treasuresCollected);
                player->y += 1;
                if (player->treasuresCollected == 3) {
                    printf("Congratulations! You have completed the game!\n");
                    exit(EXIT_SUCCESS);
                }    
            }
            player->y += 1;
        break; 
        case 'm':
        case 'M':
            for (int i = 0; i < island->height; i++) {
                printf("%s\n", island->map[i]);
            }
        break; 
    }
}

void hasWon () {

}


int main (int argc, char *argv[]) {
    FILE *file;
    Island island;
    Coordinates player;
    char Filename [101];

    if (argc != 2) {
        fprintf(stderr, "Usage: island <filename>\n");
        return EXIT_ARG_ERROR;  
    }
    
    checkFile(argv[1], &island);

    allocateMemory(&island, Filename);

    initialiseIsland(&island, &player);

    char move;
    while (1) {
        printf("Move using W,A,S,D and view the map using M:\n");
        scanf("%c", &move);
        movePlayer(move, &player, &island);
    }

    for (int i = 0; i < island.height; i++) {
        free(island.map[i]);
    }
    free(island.map);

    return EXIT_SUCCESS;

}