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
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include "util.h"

#define SAVE_DIRECTORY_NAME "./saves/"
#define EXTENSION ".cfg"

void checkReturnValueForScanf(int valueOfScanf, int goodReturnValue) {
    if(valueOfScanf!=goodReturnValue) {
        if(valueOfScanf==EOF) {
            printf("Erreur %d lors de l'appel de la fonction scanf\n", errno);
            exit(errno);
        }
        printf(
            "Erreur inconnu lors de l'appel à la fonction scanf, des valeurs n'ont pas été lues/écrites"
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
		if (errno == ENOENT) {
            #ifdef _WIN32
            if(mkdir(SAVE_DIRECTORY_NAME)==0) {
            #else
            if(mkdir(SAVE_DIRECTORY_NAME, S_IRWXU)==0) {
            #endif
                dh = opendir(SAVE_DIRECTORY_NAME);
                if(dh) {
                    saveFilesList.saveFilesList = NULL;
                    return saveFilesList;
                }
                else {
			        perror("Impossible de lire le dossier");
		            exit(EXIT_FAILURE);
                }
            }
            else {
                perror("Impossible de créer le dossier de sauvegarde");
		        exit(EXIT_FAILURE);
            }
		} else {
            perror("Impossible de lire le dossier");
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
            #ifdef DEBUG
            printf("Bonne extension !\n");
            #endif
            saveFilesList.saveFilesList[i++] = d->d_name;
        }else{
            #ifdef DEBUG
            printf("Mauvaise extension !\n");
            #endif
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
                strlen(saveFile) + 7 + strlen(SAVE_DIRECTORY_NAME) )
            );
    strcpy(path, SAVE_DIRECTORY_NAME);
    strcat(path, saveFile);
    if ((fptr = fopen(path, "r")) == NULL) {
        printf("Erreur! Le fichier ne peut pas être ouvert");
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
    #ifdef DEBUG
    printf("%u %u\n", maze->goalPos[0], maze->goalPos[1]);
    printf("%u %u\n", maze->playerPos[0], maze->playerPos[1]);
    #endif
    checkReturnValueForScanf(err, 2);
    fclose(fptr);
    memcpy(path + strlen(path) - 3, "score", 5);
    #ifdef DEBUG
    printf("%s\n",path);
    #endif
    if ((fptr = fopen(path, "r")) == NULL) {
        printf("Erreur! Le fichier ne peut pas être ouvert");
        exit(1);
    }
    err = fscanf(fptr, "%u", &(maze->numPlayers));
    maze->score = malloc(sizeof(short)*10);
    maze->namePlayers = malloc(sizeof(char *)*10);
    for(i=0; i<maze->numPlayers; i++){
        maze->namePlayers[i] = calloc(50, sizeof(char));
        err = fscanf(fptr, "%s:%u\n", maze->namePlayers[i], &(maze->score[i]));
        #ifdef DEBUG
        printf("%s:%u", maze->namePlayers[i], maze->score[i]);
        #endif
    }
    for(i; i<10; i++) {
        maze->namePlayers[i] = calloc(50, sizeof(char));
        maze->score[i] = 0;
        #ifdef DEBUG
        printf("Score non enregistré\n");
        #endif
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
    path = malloc(sizeof(char)*(len+8+strlen(SAVE_DIRECTORY_NAME)));
    strcpy(path, SAVE_DIRECTORY_NAME);
    strcat(path, saveFile);
    #ifdef DEBUG
    printf("%s", path);
    #endif
    if(fileExist(path)){
        if(!promptBool(
            "Erreur, un autre fichier avec le même nom existe, voulez-vous l'écraser ?"
        )) {

            return 1;
        }
    }
    if ((fptr = fopen(path, "w")) == NULL) {
        printf(
            "Erreur! Le fichier ne peut pas être ouvert ou créer avec les droits en écriture"
        );

        exit(1);
    }
    fprintf(fptr, "%hu;%hu\n", maze.height, maze.width);
    for(i = 0; i < maze.height; i++) {
        fprintf(fptr, "%s\n", maze.maze[i]);
    }
    fprintf(fptr, "%hu;%hu\n", maze.playerPos[0], maze.playerPos[1]);
    fprintf(fptr, "%hu;%hu\n", maze.goalPos[0], maze.goalPos[1]);
    fclose(fptr);
    /*We change the .cfg to .score*/
    memcpy(path+strlen(path)-3,"score",5);
    if ((fptr = fopen(path, "w")) == NULL) {
        printf(
            "Erreur! Le fichier de score ne peut pas être ouvert ou créer avec les droits en écriture"
        );

        exit(1);
    }
    fprintf(fptr, "%d", maze.numPlayers);
    for(i=0; i < maze.numPlayers; i++) {
        fprintf(fptr, "%s:%hu\n", maze.namePlayers[i], maze.score[i]);
    }
    fclose(fptr);
    return 0;
}
#ifdef TESTSAVEMANAGER
int main(void) {
    printf("Le fichier de sauvegarde test.cfg existe ? %s\n",
        fileExist("saves/test.cfg") ? "Oui" : "non"
    );
    save(*(newMaze()), "test123", 5);
    Maze * maze = readSaveFile("test123");
    printf("Sauvegarde et lecture effectuée avec succès\n");
    printMaze(*maze);
    return 0;
}
#endif
