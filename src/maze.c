#include "maze.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define WIDTH 9
#define HEIGHT 9
#define WALL_CHAR '#'
#define PLAYER_CHAR 'o'
#define GOAL_CHAR '-'


typedef struct {
    int ** maze;
    bool ** verticalWall;
    bool ** horizontalWall;
}MazeConstructor;

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a < b ? b : a;
}

Maze * toMaze(MazeConstructor maze) {
    int count, j, i;
    Maze * finalMaze;
    count = 0;
    finalMaze = malloc(sizeof(Maze));
    /*We need * 2 size because we need to two information per box
    (for the final box, the wall and a character \0 to end the string)*/
    finalMaze->height = (HEIGHT+1)*2;
    finalMaze->width = (WIDTH+1)*2;
    finalMaze->maze = malloc(sizeof(char *)* finalMaze->height);
    finalMaze->maze[0] = calloc(finalMaze->width, sizeof(char));
    for(j=0; j<WIDTH; j++) {
        if(maze.horizontalWall[0][j]) {
            finalMaze->maze[count][j*2] = WALL_CHAR;
            finalMaze->maze[count][j*2+1] = WALL_CHAR;
        }else{
            if(maze.verticalWall[0][j]){
                finalMaze->maze[count][j*2] = WALL_CHAR;
            }else{
                finalMaze->maze[count][j*2] = ' ';
            }
            finalMaze->maze[count][j*2+1] = ' ';
        }
    }
    finalMaze->maze[count][(WIDTH)*2] = WALL_CHAR;
    finalMaze->maze[count++][(WIDTH*2)+1] = 0;
    finalMaze->maze[1] = calloc(finalMaze->width, sizeof(char));
    for(j=0; j<=WIDTH; j++) {
        finalMaze->maze[count][j*2] = maze.verticalWall[0][j] ? WALL_CHAR : ' ';
        finalMaze->maze[count][j*2+1] = ' ';
    }
    finalMaze->maze[count++][(WIDTH*2)+1] = 0;
    for(i=1; i<HEIGHT; i++) {
        finalMaze->maze[count] = calloc(finalMaze->width, sizeof(char));
        for(j=0; j<WIDTH; j++){
            finalMaze->maze[count][j*2] = maze.verticalWall[i][j] ? WALL_CHAR : ' ';
            finalMaze->maze[count][j*2+1] = maze.horizontalWall[i][j] ? WALL_CHAR : ' ';
        }
        if(maze.verticalWall[i][WIDTH] ) {
            finalMaze->maze[count][WIDTH*2] = WALL_CHAR;
        }
        finalMaze->maze[count++][(WIDTH*2)+1] = 0;
        finalMaze->maze[count] = calloc(finalMaze->width, sizeof(char));
        for(j=0; j<=WIDTH; j++) {
            finalMaze->maze[count][j*2] = maze.verticalWall[i][j] ? WALL_CHAR : ' ';
            finalMaze->maze[count][j*2+1] = ' ';
        }
        finalMaze->maze[count++][(WIDTH*2)+1] = 0;
    }
    finalMaze->maze[count] = calloc(finalMaze->width, sizeof(char));
    for(j=0; j<WIDTH; j++){
        if(maze.horizontalWall[HEIGHT][j]) {
            finalMaze->maze[count][j*2] = WALL_CHAR;
            finalMaze->maze[count][j*2+1] = WALL_CHAR;
        }else{
            if(maze.verticalWall[0][j]){
                finalMaze->maze[count][j*2] = WALL_CHAR;
            }else{
                finalMaze->maze[count][j*2] = ' ';
            }
            finalMaze->maze[count][j*2+1] = ' ';
        }
    }
    if(maze.verticalWall[HEIGHT-1][WIDTH] || maze.horizontalWall[HEIGHT][WIDTH-1])
        finalMaze->maze[count][WIDTH*2] = WALL_CHAR;
    finalMaze->maze[count][(WIDTH*2)+1] = 0;
    return finalMaze;
}

void initMazeConstructor(MazeConstructor * maze) {
    int i, j, count = 0;
    for(i=0; i<HEIGHT; i++) {
        for(j=0; j<WIDTH; j++) {
            maze->maze[i][j] = count++;
            maze->horizontalWall[i][j] = true;
            maze->verticalWall[i][j] = true;
        }
        maze->verticalWall[i][WIDTH] = true;
    }
    for(j=0; j<WIDTH; j++) {
        maze->horizontalWall[HEIGHT][j] = true;
    }
}

bool isMazeConstructorFill(MazeConstructor maze) {
    int i, j;
    for(i=0; i<HEIGHT; i++) {
        for(j=0; j<WIDTH; j++) {
            if(maze.maze[i][j]){
                return false;
            }
        }
    }
    return true;
}
void newMaze2(MazeConstructor mazeConstructor) {
    char ** test = malloc(sizeof(char*) * (HEIGHT+1) * 3);
    int i, j;
    for(i=0; i<HEIGHT; i++) {
        test[i*3] = calloc((WIDTH+1)*3, sizeof(char));
        test[i*3+1] = calloc((WIDTH+1)*3, sizeof(char));
        test[i*3+2] = calloc((WIDTH+1)*3, sizeof(char));
        for(j=0; j<WIDTH; j++){
            test[i*3+1][j*3+1] = ' ';
            test[i*3][j*3] = '#';
            test[i*3+2][j*3] = '#';
            test[i*3][j*3+2] = '#';
            test[i*3+2][j*3+2] = '#';
            if(mazeConstructor.horizontalWall[i][j]){
                test[i*3][j*3+1] = '#';
            }else{
                test[i*3][j*3+1] = ' ';
            }
            if(mazeConstructor.horizontalWall[i+1][j]){
                test[i*3+2][j*3+1] = '#';
            }else{
                test[i*3+2][j*3+1] = ' ';
            }
            if(mazeConstructor.verticalWall[i][j]){
                test[i*3+1][j*3] = '#';
            }else{
                test[i*3+1][j*3] = ' ';
            }
            if(mazeConstructor.verticalWall[i][j+1]){
                test[i*3+1][j*3+2] = '#';
            }else{
                test[i*3+1][j*3+2] = ' ';
            }
        }
    }
    for(i=0; i<HEIGHT*3; i++) {
        for(j=0; j<WIDTH*3; j++) {
            printf("%c", test[i][j]);
        }
        free(test[i]);
        printf("\n");
    }
    free(test);
}
void printMazeConstructor(MazeConstructor maze) {
    int i, j;
    for(j=0; j<WIDTH; j++) {
        printf("%s", maze.horizontalWall[0][j] ? "##" : "  ");
    }
    if(maze.verticalWall[0][WIDTH] ) {
        printf("#");
    }
    printf("\n");
    for(j=0; j<=WIDTH; j++) {
        printf("%c ", maze.verticalWall[0][j] ? '#' : ' ');
    }
    printf("\n");
    for(i=1; i<HEIGHT; i++) {
        for(j=0; j<WIDTH; j++){
            printf("%c%c", maze.verticalWall[i][j] ? '#' : ' ' ,maze.horizontalWall[i][j] ? '#' : ' ');
        }
        if(maze.verticalWall[i][WIDTH] ) {
            printf("#");
        }
        printf("\n");
        for(j=0; j<=WIDTH; j++) {
            printf("%c ", maze.verticalWall[i][j] ? '#' : ' ');
        }
        printf("\n");
    }
    for(j=0; j<WIDTH; j++){
        printf("%s", maze.horizontalWall[HEIGHT][j] ? "##" : "  ");
    }
    if(maze.verticalWall[HEIGHT-1][WIDTH] ) {
        printf("#");
    }
    printf("\n");
}

void replace(MazeConstructor * mazeConstructor, int oldIndice, int newIndice, int x, int y) {
    if(mazeConstructor->maze[x][y]==oldIndice) {
        mazeConstructor->maze[x][y] = newIndice;
        if(x!=0)
            replace(mazeConstructor, oldIndice, newIndice, x-1, y);
        if(x!=HEIGHT-1)
            replace(mazeConstructor, oldIndice, newIndice, x+1, y);
        if(y!=0)
            replace(mazeConstructor, oldIndice, newIndice, x, y-1);
        if(y!=WIDTH-1)
            replace(mazeConstructor, oldIndice, newIndice, x, y+1);
    }
}

void fillMazeConstructor(MazeConstructor * maze) {
    Direction wichWall;
    int wichBox, col, line, newIndice, oldIndice;
    while(!isMazeConstructorFill(*maze)){
        /*printMazeConstructor(*maze);*/
        wichWall = rand() % 4;
        wichBox = rand() % (WIDTH*HEIGHT);
        line = wichBox % HEIGHT;
        col = floor(wichBox /HEIGHT);
        oldIndice = 0;
        newIndice = 0;
        /*printf("\n\n[%d %d]\n\n", line, col);*/
        switch (wichWall)
        {
        case TOP:
            if((line != 0) && maze->maze[line][col] != maze->maze[line-1][col]){
                newIndice = min(maze->maze[line][col], maze->maze[line-1][col]);
                oldIndice = max(maze->maze[line][col], maze->maze[line-1][col]);
                maze->horizontalWall[line][col] = false;
                replace(maze, oldIndice, newIndice, line-1, col);
            }
            break;
        case BOTTOM:
            if((line != HEIGHT-1) && maze->maze[line][col] != maze->maze[line+1][col]){
                newIndice = min(maze->maze[line][col], maze->maze[line+1][col]);
                oldIndice = max(maze->maze[line][col], maze->maze[line+1][col]);
                maze->horizontalWall[line+1][col] = false;
                replace(maze, oldIndice, newIndice, line+1, col);
            }
            break;
        case LEFT:
            if((col != 0) && maze->maze[line][col] != maze->maze[line][col-1]){
                newIndice = min(maze->maze[line][col], maze->maze[line][col-1]);
                oldIndice = max(maze->maze[line][col], maze->maze[line][col-1]);
                maze->verticalWall[line][col] = false;
                replace(maze, oldIndice, newIndice, line, col-1);
            }
            break;
        default:
            if((col != WIDTH -1) && maze->maze[line][col] != maze->maze[line][col+1]){
                newIndice = min(maze->maze[line][col], maze->maze[line][col+1]);
                oldIndice = max(maze->maze[line][col], maze->maze[line][col+1]);
                maze->verticalWall[line][col+1] = false;
                replace(maze, oldIndice, newIndice, line, col+1);
            }
            break;
        }
        if(maze->maze[line][col]!=newIndice)
            replace(maze, oldIndice, newIndice, line, col);
        #ifdef DEBUG
        if(oldIndice!=0) {
            printMazeConstructor(*maze);
            #ifdef STEPBYSTEP
            getchar();
            #endif
        }
        #endif
    }

}

void prepareMazeConstructor(MazeConstructor * maze){
    maze->horizontalWall[0][0] = false;
    maze->horizontalWall[HEIGHT][WIDTH-1] = false;
}

void prepareMaze(Maze * maze){
    maze->goalPos[0] = maze->height-2;
    maze->goalPos[1] = maze->width-3;
    maze->maze[maze->height-2][maze->width-3] = GOAL_CHAR;
    maze->playerPos[0] = 0;
    maze->playerPos[1] = 1;
    maze->maze[0][1] = PLAYER_CHAR;
}

MazeConstructor * newMazeConstructor() {
    int i;
    MazeConstructor * maze = malloc(sizeof(MazeConstructor));
    maze->maze = malloc(sizeof(int*) * HEIGHT);
    maze->verticalWall = malloc(sizeof(bool*) * HEIGHT);
    maze->horizontalWall = malloc(sizeof(bool*) * (HEIGHT+1));
    for(i=0; i<HEIGHT; i++) {
        maze->maze[i] = calloc(WIDTH, sizeof(int));
        maze->verticalWall[i] = calloc(WIDTH+1, sizeof(bool));
        maze->horizontalWall[i] = calloc(WIDTH, sizeof(bool));
    }
    maze->horizontalWall[HEIGHT] = calloc(WIDTH, sizeof(bool));
    initMazeConstructor(maze);
    fillMazeConstructor(maze);
    prepareMazeConstructor(maze);
    return maze;
}

void freeMazeConstructor(MazeConstructor * maze) {
    int i;
    for(i=0; i<HEIGHT; i++){
        free(maze->horizontalWall[i]);
        free(maze->verticalWall[i]);
        free(maze->maze[i]);
    }
    free(maze->horizontalWall[HEIGHT]);
    free(maze->horizontalWall);
    free(maze->verticalWall);
    free(maze->maze);
    free(maze);
}

Maze * newMaze() {
    MazeConstructor * mazeConstructor = newMazeConstructor();
    Maze * maze = toMaze(*mazeConstructor);
    freeMazeConstructor(mazeConstructor);
    prepareMaze(maze);
    return maze;
}

void freeMaze(Maze * maze) {
    int i;
    for(i=0; i<maze->height; i++) {
        free(maze->maze[i]);
    }
    free(maze->maze);
    free(maze);
}

void printMaze(Maze maze) {
    int i;
    for(i=0; i<maze.height-1; i++)
        printf("%s\n", maze.maze[i]);
}

bool isMovementPossible(Maze maze, Direction direction) {
    bool movementPossible;
    switch (direction)
    {
        case TOP:
            movementPossible = maze.playerPos[0]!=0 && maze.maze[maze.playerPos[0]-1][maze.playerPos[1]] != WALL_CHAR;
            break;
        case BOTTOM:
            movementPossible = maze.playerPos[0] != maze.height-1 && maze.maze[maze.playerPos[0]+1][maze.playerPos[1]] != WALL_CHAR;
            break;
        case LEFT:
            movementPossible = maze.playerPos[1]!=0 && maze.maze[maze.playerPos[0]][maze.playerPos[1]-1] != WALL_CHAR;
            break;
        default:
            movementPossible = maze.playerPos[1] != maze.width-1 && maze.maze[maze.playerPos[0]][maze.playerPos[1]+1] != WALL_CHAR;
            break;
    }
    return movementPossible;
}

bool move(Maze * maze, Direction direction) {
    if(isMovementPossible(*maze, direction)){
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

#ifdef GENERATION
int main(int argc, char ** argv) {
    if(argc==2){
        int rand = 0;
        rand = atoi(argv[1]);
        if(rand!=0 || argv[1][0]!='0'){
            srand(rand);
        }else{
            printf("Veuillez rentrez une valeur aléatoire");
        }
    }else{
        srand(time(NULL));
    }
    MazeConstructor * maze = newMazeConstructor();
    newMaze2(*maze);
    freeMazeConstructor(maze);
}
#endif