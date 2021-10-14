#include "maze.h"
#include <dirent.h>
#include <stdio.h>
#include "saveManager.h"
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

#define SAVE_DIRECTORY_NAME "saves"
#define EXTENSION ".cfg"

SaveFilesList listSaveFiles() {
    SaveFilesList saveFilesList;
    saveFilesList.length =0;
    struct dirent *d;
	DIR *dh = opendir(SAVE_DIRECTORY_NAME);
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
    int i=0;
    saveFilesList.saveFilesList = malloc(sizeof(char*)*256); //On estime qu'il est imporbable d'avoir plus de 256 fichiers de sauvegardes
	while ((d = readdir(dh)) != NULL)
	{
        if(strcmp(d->d_name, ".")==0||strcmp(d->d_name, "..")==0) {
            continue;
        }
        #ifdef DEBUG
		printf("%s\n", d->d_name);
        #endif
        saveFilesList.saveFilesList[i++] = d->d_name;
	}
    saveFilesList.length = i;
    return saveFilesList;
}

Maze * readSaveFile(char * saveFile) {
    FILE *fptr;
    if ((fptr = fopen(saveFile, "r")) == NULL) {
        printf("Error! File cannot be opened.");
        exit(1);
    }
    Maze * maze = malloc(sizeof(Maze));
    fscanf(fptr, "%hd;%hd\n", &(maze->height), &(maze->width));
    maze->maze = malloc(sizeof(char * )* maze->height);
    for(int i=0; i<maze->height; i++){
        printf("%d\n", i);
        maze->maze[i] = calloc(maze->width, sizeof(char));
        fscanf(fptr, "%[^\n]\n", maze->maze[i]);
    }
    fscanf(fptr, "%hd;%hd\n", &(maze->playerPos[0]), &(maze->playerPos[1]));
    fscanf(fptr, "%hd;%hd", &(maze->goalPos[0]), &(maze->goalPos[1]));
    fclose(fptr);
    return maze;
}

static bool fileExist(char * saveFile) {
    FILE *fptr;
    bool res= (fptr = fopen(saveFile, "r")) == NULL;
    fclose(fptr);
    return res;
}

int save(Maze maze, char * saveFile) {
    FILE *fptr;
    int blen = strlen(saveFile);
    if((blen < 4) || (0 != strcmp(saveFile + blen - 4, EXTENSION))){
        saveFile = strcat(saveFile, EXTENSION);
    }
    if(fileExist(saveFile)){
        char response;
        do{
            printf("Error, another file with the same name has been detected, would you like to overwrite it ? [Y/N] ");
            scanf("%c", &response);
        }while(response!='Y'&&response!='y'&&response!='n'&&response!='N');
        if(response=='n'||response=='N') {
            return 1;
        }
    }
    if ((fptr = fopen(saveFile, "w")) == NULL) {
        printf("Error! File cannot be created.");
        exit(1);
    }
    fprintf(fptr, "%hd;%hd\n", maze.height, maze.width);
    for(int i=0; i<maze.height-1; i++) {
        fprintf(fptr, "%s\n", maze.maze[i]);
    }
    fprintf(fptr, "%hd;%hd\n", maze.playerPos[0], maze.playerPos[1]);
    fprintf(fptr, "%hd;%hd\n", maze.goalPos[0], maze.goalPos[1]);
    fclose(fptr);
    return 0;
}
#ifdef TESTSAVEMANAGER
int main(void) {
    printf("The saveFile test.cfg %s exist\n", fileExist("saves/test.cfg") ? "" : "doesn't");
    save(*(newMaze()), "saves/test.cfg");
    Maze * maze = readSaveFile("saves/test.cfg");
    printf("Sauvegarde et lecture effectué avec succès\n");
    printMaze(*maze);
    return 0;
}
#endif