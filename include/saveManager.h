#ifndef SAVEMANAGER
#define SAVEMANAGER
#include "maze.h"

typedef struct {
    int length;
    char ** saveFilesList;
}SaveFilesList;

typedef enum {OK=0, STOPPED_BY_THE_USER, DIRECTORY_UNREADABLE, FILE_UNREADABLE, DIRECTORY_CANNOT_BE_CREATED, FILE_CANNOT_BE_CREATED, OTHER} ERROR_SAVE_MANAGER;

SaveFilesList listSaveFiles();
Maze * readSaveFile(char * saveFile);
int save(Maze maze, char * saveFile);

#endif