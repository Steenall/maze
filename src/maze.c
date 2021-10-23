#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "maze.h"

#define WALL_CHAR '#'
#define PLAYER_CHAR 'o'
#define GOAL_CHAR '-'
#define VOID_CHAR ' '

typedef enum {Void=VOID_CHAR, Wall=WALL_CHAR, Player=PLAYER_CHAR, Goal=GOAL_CHAR} ElementMaze;


char * directionToString(Direction direction) {
    char * tab = malloc(sizeof(char)*7);
    switch (direction)
    {
    case TOP:
        tab = "TOP";
        break;
    
    case BOTTOM:
        tab = "BOTTOM";
        break;
    
    case LEFT:
        tab = "LEFT";
        break;
    
    case RIGHT:
        tab = "RIGHT";
        break;
    
    default:
        tab = "ERROR";
        break;
    }
    return tab;
}

Maze * newMaze() {
    Maze * maze;
    int i;
    int j;
    int count;
    int currentx;
    int currenty;
    int ** visited;
    bool * possible;
    bool noActionPossible;
    int current;
    int nb;
    nb = 1;
    current = 1;
    currentx = 1;
    currenty = 1;
    j = 1;
    maze = malloc(sizeof(Maze));
    maze->maze = malloc(sizeof(int* )* HEIGHT);
    for(i=0; i<=HEIGHT; i++) {
        maze->maze[i] = calloc(WIDTH ,sizeof(int));
    }
    maze->mazeName = calloc(1, sizeof(char));
    /*We need * 2 size because we need to two information per box
    (for the final box, the wall and a character \0 to end the string)*/
    maze->score = calloc(10, sizeof(unsigned int));
    maze->namePlayers = malloc(sizeof(char *) * 10);
    for(i=0; i<10; i++){
        maze->score[i] = -1;
        maze->namePlayers[i] = calloc(1, sizeof(char));
    }
    maze->numPlayers = 0;
    maze->height = HEIGHT;
    maze->width = WIDTH;
    possible = malloc(sizeof(bool)*4);
    visited = malloc(sizeof(int*) * WIDTH*HEIGHT-1);
    for(i=0; i<WIDTH*HEIGHT-1; i++) {
        visited[i] = malloc(sizeof(int)*2);
    }
    visited[0][0] = 1;
    visited[0][1] = 1;
    count = 1;
    for(i=0; i<HEIGHT; i++) {
        for(j=0; j<WIDTH; j++) {
            if( (i + 1) % 2 == 0 && (j + 1) % 2 == 0 && i != HEIGHT-1 && j != WIDTH-1){
                maze->maze[i][j] = '0';
                count++;
            }else {
                maze->maze[i][j] = '#';
            }
            #ifdef DEBUG
            printf("%d", maze->maze[i][j]);
            #endif
        }
        #ifdef DEBUG
        printf("\n");
        #endif
    }
    #ifdef DEBUG
    printf("\n");
    #endif
    maze->maze[1][1] = ' ';
    do {
        #ifdef DEBUG
        printMaze(*maze);
        printf("Ok\n");
        getchar();
        printf("\n\n");
        #endif
        noActionPossible = true;
        for(i=0; i<4;i++) {
            switch ((Direction) i)
            {
            case TOP:
                if(currenty > 1 && maze->maze[currenty-2][currentx] != ' '){
                    possible[i] = true;
                    noActionPossible = false;
                }else {
                    possible[i] = false;
                }
                break;
            
            case BOTTOM:
                if(currenty < (HEIGHT-2) && maze->maze[currenty+2][currentx] != ' '){
                    possible[i] = true;
                    noActionPossible = false;
                }else {
                    possible[i] = false;
                }
                break;
            
            case LEFT:
                if(currentx > 1 && maze->maze[currenty][currentx-2] != ' '){
                    possible[i] = true;
                    noActionPossible = false;
                }else {
                    possible[i] = false;
                }
                break;
            
            case RIGHT:
                if(currentx < (WIDTH-2) && maze->maze[currenty][currentx+2] != ' '){
                    possible[i] = true;
                    noActionPossible = false;
                }else {
                    possible[i] = false;
                }
                break;
            
            default:
                break;
            }
        }
        #ifdef DEBUG
        printf("TOP: %d\tBOTTOM: %d\tLEFT: %d\tRIGHT: %d\n", possible[0], possible[1], possible[2], possible[3]);
        printf("%d;%d\n", currenty, currentx);
        #endif
        if(!noActionPossible){
            do{
                i = rand() % 4;
            }while(!possible[i]);
            nb++;
            #ifdef DEBUG
            printf("%s\n", directionToString((Direction) i));
            #endif
            visited[i] = malloc(sizeof(int)*2);
            switch ((Direction) i)
            {
                case TOP:
                    maze->maze[currenty-2][currentx] = ' ';
                    maze->maze[currenty-1][currentx] = ' ';
                    currenty-=2;
                    break;
                
                case BOTTOM:
                    maze->maze[currenty+2][currentx] = ' ';
                    maze->maze[currenty+1][currentx] = ' ';
                    currenty+=2;
                    break;
                
                case LEFT:
                    maze->maze[currenty][currentx-2] = ' ';
                    maze->maze[currenty][currentx-1] = ' ';
                    currentx-=2;
                    break;
                
                case RIGHT:
                    maze->maze[currenty][currentx+2] = ' ';
                    maze->maze[currenty][currentx+1] = ' ';
                    currentx+=2;
                    break;
                
                default:
                    break;
            }
            current++;
            visited[current][0] = currenty;
            visited[current][1] = currentx;
        }else{
            #ifdef DEBUG
            printf("Retour\n");
            getchar();
            #endif
            current--;
            currenty = visited[current][0];
            currentx = visited[current][1];
        }
    }while(nb<(count-1));
    maze->maze[0][1] = PLAYER_CHAR;
    maze->maze[maze->height-1][maze->width-2] = GOAL_CHAR;
    free(possible);
    for(i=0; i<count; i++) {
        free(visited[i]);
    }
    free(visited);
    printf("Labyrinthe généré\n");
    return maze;
}

void freeMaze(Maze * maze) {
    unsigned int i;
    for(i=0; i<maze->height; i++) {
        free(maze->maze[i]);
    }
    free(maze->maze);
    free(maze);
}

void printMaze(Maze maze) {
    unsigned int i;
    unsigned int j;
    for(i=0; i<maze.height; i++){
        for(j=0; j<maze.width; j++) {
            printf("%c", maze.maze[i][j]);
        }
        printf("\n");
    }
}

bool isMovementPossible(Maze maze, Direction direction, unsigned int posX, unsigned int posY) {
    bool movementPossible;
    switch (direction)
    {
        case TOP:
            movementPossible = posX!=0 && maze.maze[posX-1][posY] != WALL_CHAR;
            break;
        case BOTTOM:
            movementPossible = posX != maze.height-1 && maze.maze[posX+1][posY] != WALL_CHAR;
            break;
        case LEFT:
            movementPossible = posY!=0 && maze.maze[posX][posY-1] != WALL_CHAR;
            break;
        default:
            movementPossible = posY != maze.width-1 && maze.maze[posX][posY+1] != WALL_CHAR;
            break;
    }
    return movementPossible;
}

bool move(Maze * maze, Direction direction) {
    if(isMovementPossible(*maze, direction, maze->playerPos[0], maze->playerPos[1])){
        maze->maze[maze->playerPos[0]][maze->playerPos[1]] = ' ';
        switch (direction)
        {
            case TOP:
                maze->maze[--(maze->playerPos[0])][maze->playerPos[1]] = PLAYER_CHAR;
                break;
            case BOTTOM:
                maze->maze[++(maze->playerPos[0])][maze->playerPos[1]] = PLAYER_CHAR;
                break;
            case LEFT:
                maze->maze[maze->playerPos[0]][--(maze->playerPos[1])] = PLAYER_CHAR;
                break;
            default:
                maze->maze[maze->playerPos[0]][++(maze->playerPos[1])] = PLAYER_CHAR;
                break;
        }
        return true;
    }else{
        return false;
    }
}

bool isGameFinished(Maze maze) {
    return maze.goalPos[0] == maze.playerPos[0] && maze.goalPos[1] == maze.playerPos[1];
}
