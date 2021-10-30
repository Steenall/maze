/**
 * @file saveManager.c
 * @brief Functions related to saving or reading a save file
 * @author Steenall
 * @version 1.0.0
 * @date 30 october 2021
 */
#include "maze.h"
#include <dirent.h>
#include <stdio.h>
#include "saveManager.h"
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include "util.h"

#define SAVE_DIRECTORY_NAME "./saves/"
#define EXTENSION ".cfg"

void checkReturnValueForScanf(int valueOfScanf, int goodReturnValue) {
    if(valueOfScanf!=goodReturnValue) {
        if(valueOfScanf==EOF) {
            printf("Error %d when calling scanf function\n", errno);
            exit(errno);
        }
        printf(
            "Unknown error when calling scanf, scanf didn't read or write all the values needed"
        );
    }
}

SaveFilesList listSaveFiles() {
    SaveFilesList saveFilesList;
    struct dirent *d;
	DIR *dh;
    unsigned int i;
    char * ext;
    saveFilesList.length = 0;
    i = 0;
    dh = opendir(SAVE_DIRECTORY_NAME);
	if (!dh) {
		if (errno = ENOENT) {
            #ifdef _WIN32
            if(mkdir(SAVE_DIRECTORY_NAME)==0){
            #else
            if(mkdir(SAVE_DIRECTORY_NAME, S_IRWXU)==0){
            #endif
                dh = opendir(SAVE_DIRECTORY_NAME);
                if(dh) {
                    saveFilesList.saveFilesList = NULL;
                    return saveFilesList;
                }
                else {
			        perror("Unable to read the directory");
		            exit(EXIT_FAILURE);
                }
            }
            else {
                perror("Unable to create the save directory");
		        exit(EXIT_FAILURE);
            }
		}
        else {
			perror("Unable to read the directory");
		    exit(EXIT_FAILURE);
		}
	}
    /**
     * On estime qu'il est imporbable d'avoir
     * plus de 256 fichiers de sauvegardes
     */
    saveFilesList.saveFilesList = malloc(sizeof(char*)*256);
	while ((d = readdir(dh)) != NULL)
	{
        if(strcmp(d->d_name, ".")==0||strcmp(d->d_name, "..")==0) {
            continue;
        }
        #ifdef DEBUG
		printf("%s\n", d->d_name);
        #endif
        ext = strrchr(d->d_name, '.');
        if(strcmp(ext, EXTENSION)==0){
            printf("Bonne extension !\n");
            saveFilesList.saveFilesList[i++] = d->d_name;
        }else{
            printf("Mauvaise extension !\n");
        }
	}
    saveFilesList.length = i;
    return saveFilesList;
}

Maze * readSaveFile(char * saveFile) {
    FILE *fptr;
    unsigned int i;
    int err;
    Maze * maze;
    char * path;
    maze = malloc(sizeof(Maze));
    path = malloc(sizeof(char) * (
                strlen(saveFile) + 5 + strlen(SAVE_DIRECTORY_NAME) )
            );
    strcpy(path, SAVE_DIRECTORY_NAME);
    strcat(path, saveFile);
    if ((fptr = fopen(path, "r")) == NULL) {
        printf("Error! File cannot be opened.");
        exit(1);
    }
    maze->mazeName = malloc(sizeof(char)*strlen(saveFile));
    strcpy(maze->mazeName, saveFile);
    err = fscanf(fptr, "%u;%u\n", &(maze->height), &(maze->width));
    /* checkReturnValueForScanf(err, 2); */
    maze->maze = malloc(sizeof(char * )* maze->height);
    for(i=0; i<maze->height; i++){
        printf("%d\n", i);
        maze->maze[i] = calloc(maze->width, sizeof(char));
        err = fscanf(fptr, "%[^\n]\n", maze->maze[i]);
        /* checkReturnValueForScanf(err, 1); */
    }
    err = fscanf(fptr, "%u;%u\n", &(maze->playerPos[0]),
                    &(maze->playerPos[1])
                );

    checkReturnValueForScanf(err, 2);
    err = fscanf(fptr, "%u;%u", &(maze->goalPos[0]), &(maze->goalPos[1]));
    printf("%u %u\n", maze->goalPos[0], maze->goalPos[1]);
    printf("%u %u\n", maze->playerPos[0], maze->playerPos[1]);
    checkReturnValueForScanf(err, 2);
    fclose(fptr);
    memcpy(path+strlen(path)-3,"score",5);
    printf("%s\n",path);
    if ((fptr = fopen(path, "r")) == NULL) {
        printf("Error! File cannot be opened.");
        exit(1);
    }
    err = fscanf(fptr, "%u", &(maze->numPlayers));
    maze->score = malloc(sizeof(short)*10);
    maze->namePlayers = malloc(sizeof(char *)*10);
    for(i=0; i<maze->numPlayers; i++){
        maze->namePlayers[i] = calloc(50, sizeof(char));
        err = fscanf(fptr, "%s:%u\n", maze->namePlayers[i], &(maze->score[i]));
        printf("%s:%u", maze->namePlayers[i], maze->score[i]);
    }
    for(i; i<10; i++) {
        maze->namePlayers[i] = calloc(50, sizeof(char));
        maze->score[i] = 0;
        printf("Score non enregistré\n");
    }
    fclose(fptr);
    return maze;
}

bool fileExist(char * saveFile) {
    FILE *fptr;
    fptr = fopen(saveFile, "r");
    if(fptr!=NULL)
        fclose(fptr);
    return fptr!=NULL;
}

int save(Maze maze, char * saveFile, int len) {
    FILE *fptr;
    unsigned int i;
    char * path;
    printf("%s\n", saveFile);
    if((len < 4) || (0 != strcmp(saveFile + len - 4, EXTENSION))){
        saveFile = strcat(saveFile, EXTENSION);
        printf("\n%s\n", saveFile);
    }
    path = malloc(sizeof(char)*(len+5+strlen(SAVE_DIRECTORY_NAME)));
    strcpy(path, SAVE_DIRECTORY_NAME);
    strcat(path, saveFile);
    printf("%s", path);
    if(fileExist(path)){
        if(!promptBool(
                "Error, another file with the same name has been detected, would you like to overwrite it ?"
            )) {

            return 1;
        }
    }
    if ((fptr = fopen(path, "w")) == NULL) {
        printf(
            "Error! File cannot be created or opened with writing permssion."
            );

        exit(1);
    }
    fprintf(fptr, "%hd;%hd\n", maze.height, maze.width);
    for(i = 0; i < maze.height; i++) {
        fprintf(fptr, "%s\n", maze.maze[i]);
    }
    fprintf(fptr, "%hd;%hd\n", maze.playerPos[0], maze.playerPos[1]);
    fprintf(fptr, "%hd;%hd\n", maze.goalPos[0], maze.goalPos[1]);
    fclose(fptr);
    memcpy(path+strlen(path)-3,"score",5);
    if ((fptr = fopen(path, "w")) == NULL) {
        printf(
            "Error! File cannot be created or opened with writing permssion."
            );

        exit(1);
    }
    fprintf(fptr, "%d", maze.numPlayers);
    for(i=0; i < maze.numPlayers; i++) {
        fprintf(fptr, "%s:%hd\n", maze.namePlayers[i], maze.score[i]);
    }
    fclose(fptr);
    return 0;
}
#ifdef TESTSAVEMANAGER
int main(void) {
    printf("The saveFile test.cfg %s exist\n",
        fileExist("saves/test.cfg") ? "" : "doesn't"
    );
    save(*(newMaze()), "test123", 5);
    Maze * maze = readSaveFile("test123");
    printf("Sauvegarde et lecture effectué avec succès\n");
    printMaze(*maze);
    return 0;
}
#endif