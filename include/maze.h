#ifndef MAZE
#define MAZE
#include <stdbool.h>

#define WIDTH 25
#define HEIGHT 11
#define WALL_CHAR '#'
#define PLAYER_CHAR 'o'
#define GOAL_CHAR '-'

typedef struct {
    char * mazeName;
    char ** maze;
    unsigned int height;
    unsigned int width;
    unsigned int playerPos[2];
    unsigned int goalPos[2];
    unsigned int numPlayers;
    unsigned int * score;
    char ** namePlayers;
}Maze;

typedef enum {ERROR=-1, TOP=0, BOTTOM, LEFT, RIGHT} Direction;

void printMaze(Maze maze);
Maze * newMaze();
void freeMaze(Maze * maze);
bool isGameFinished(Maze maze);
bool move(Maze * maze, Direction direction);
/* This function could be use if we want to show wich direction is not available */
bool isMovementPossible(Maze maze, Direction direction, unsigned posX, unsigned posY);
#endif