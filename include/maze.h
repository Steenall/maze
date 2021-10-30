/**
 * @file maze.h
 * @brief Header of maze.c
 * @author Steenall
 * @version 1.0.0
 * @date 30 october 2021
 */
#ifndef MAZE
#define MAZE
#include <stdbool.h>

#define WALL_CHAR '#'
#define PLAYER_CHAR 'o'
#define GOAL_CHAR '-'

/**
 * @brief A structure containing all the information for the maze
 * 
 */
typedef struct {
    /** The name of the maze */
    char * mazeName;
    /**An array of string containing all the blocks and entity in it */
    char ** maze;
    /** The height of the Maze */
    unsigned int height;
    /** The width of the Maze */
    unsigned int width;
    /** The position of the player in the x and y axis */
    unsigned int playerPos[2];
    /** The position of the goal in the x and y axis */
    unsigned int goalPos[2];
    /** The number of player who play and save their score (max: 10) */
    unsigned int numPlayers;
    /** 
     * An array of score from players who play and save their score (max: 10)
    */
    unsigned int * score;
    /**
     * An array of name from players who play and save their score (max: 10)
    */
    char ** namePlayers;
}Maze;

/**
 * @brief An enumeration of all types of direction possible
 * 
 */
typedef enum {ERROR=-1, TOP=0, BOTTOM, LEFT, RIGHT} Direction;

/**
 * @brief Tranform a direction to a string
 * 
 * @param direction The direction to transform
 * @return char* The direction in a string
 */
char * directionToString(Direction direction);

/**
 * @brief Print the maze in the terminal
 * 
 * @param maze The mlaze to print
 */
void printMaze(Maze maze);

/**
 * @brief Create a new Maze and return it
 * 
 * @param width The width of the maze
 * @param height The height of the maze
 * @return Maze* The new Maze created
 */
Maze * newMaze(int width, int height);

/**
 * @brief free the memory used by the maze
 * 
 * @param maze The maze to free
 */
void freeMaze(Maze * maze);

/**
 * @brief Check if the game is finished
 * 
 * @param maze 
 * @return true The game is finished
 * @return false The game isn't finished
 */
bool isGameFinished(Maze maze);

/**
 * @brief Move the player in a Direction
 * 
 * @param maze The maze
 * @param direction The Direction in which the player
 * @return true if the player has been move succesfully
 * @return false if the player cannot be moved
 */
bool move(Maze * maze, Direction direction);

/**
 * This function could be use if we want to show
 * which Direction is not available
 */

/**
 * @brief Check if a movement is possible
 * 
 * @param maze The maze
 * @param direction The Direction in which the entity aims to move
 * @param posX The position of the entity on the x-axis
 * @param posY The position of the entity on the y-axis
 * @return true The movement is possible
 * @return false The movement isn't possible
 */
bool isMovementPossible(Maze maze, Direction direction, unsigned int posX, unsigned int posY);
#endif