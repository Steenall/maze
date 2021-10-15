#ifndef MAZE
#define MAZE
#include <stdbool.h>

typedef struct {
    char ** maze;
    short height;
    short width;
    short playerPos[2];
    short goalPos[2];
    int numPlayers;
    int * score;
    char ** namePlayers;
}Maze;

typedef enum {TOP=0, BOTTOM, LEFT, RIGHT} Direction;

void printMaze(Maze maze);
Maze * newMaze();
void freeMaze(Maze * maze);
bool isGameFinished(Maze maze);
bool move(Maze * maze, Direction direction);
/* This function could be use if we want to show wich direction is not available */
bool isMovementPossible(Maze maze, Direction direction);
#endif