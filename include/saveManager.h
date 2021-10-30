/**
 * @file saveManager.h
 * @brief Header of saveManger.c
 * @author Steenall
 * @version 1.0.0
 * @date 30 october 2021
 */

#ifndef SAVEMANAGER
#define SAVEMANAGER
#include "maze.h"

/**
 * @brief A structure containing a list of names of savefiles and the length
 * 
 */
typedef struct {
    /** The number of save files */
    int length;
    /** The list of the names of save files */
    char ** saveFilesList;
}SaveFilesList;

/**
 * @brief Enumeration with all types of error codes when reading or saving
 * a save file
 * 
 */
typedef enum {
    OK=0,
    STOPPED_BY_THE_USER,
    DIRECTORY_UNREADABLE,
    FILE_UNREADABLE,
    DIRECTORY_CANNOT_BE_CREATED,
    FILE_CANNOT_BE_CREATED,
    OTHER
} ERROR_SAVE_MANAGER;

/**
 * @brief List all the save files stored in the save folder
 * 
 * @return SaveFilesList The list of saveFiles
 */
SaveFilesList listSaveFiles();

/**
 * @brief Read a save file an return it
 * 
 * @param saveFile The name of the save file to read
 * @return Maze* The maze
 */
Maze * readSaveFile(char * saveFile);

/**
 * @brief Save a save file on the disk in the save folder
 * 
 * @param maze The maze to save
 * @param saveFile The name for the save file
 * @param len The length of the name for the save file
 * @return int The error code
 */
int save(Maze maze, char * saveFile, int len);

#endif