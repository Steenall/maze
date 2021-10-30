/**
 * @file game.c
 * @brief The Main component which help to launch a game and save it
 * @author Steenall
 * @version 1.0.0
 * @date 30 october 2021
 */
#include "maze.h"
#include "saveManager.h"
#include "util.h"

#include <time.h>
#if defined _WIN32 || defined _WIN64
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Start a game of maze
 * 
 * @param maze The maze in whitch the player aims to play
 * @return true The user exit the game manualy
 * @return false The user finished the game
 */
bool start(Maze * maze) {
    char response;
    unsigned int count;
    unsigned i;
    unsigned j;
    unsigned int len;
    Direction direction;
    bool exitTheGame;
    bool invalidPath;
    char * name;
    exitTheGame = false;
    invalidPath = false;
    count = 0;
    i = 0;
    name = malloc(sizeof(char) * 50);
    strcpy(name, "Hello");
    do{
        printMaze(*maze);
        if(invalidPath) {
            invalidPath = false;
            printf("\033[31;01mYou cannot move to this direction\033[0;01m\n");
        }
        response = promptChar(
            "In which direction do you want to go\n   z (↑) q (←) d (→) s (↓) e (exit)",
            "zqsde", true );
            
        printf("\n\033[H\033[2J");
        switch (response)
        {
            case 'z':
                direction = (Direction) TOP;
                break;
            case 'q':
                direction = (Direction) LEFT;
                break;
            case 's':
                direction = (Direction) BOTTOM;
                break;
            case 'd':
                direction = (Direction) RIGHT;
                break;
            case 'e':
                exitTheGame = true;
                break;
            default:
                printf("There was a problem when parsing the direction");
                exit(1);
        }
        if(!exitTheGame && !move(maze, direction)) {
           invalidPath = true;
        }
    }while(!exitTheGame && !isGameFinished(*maze));
    /*if(!exitTheGame) {
        for(i=0; i < maze->numPlayers; i++) {
            if(count > maze->score[i]){
                break;
            }
        }
        printf("Ok1\n");
        if(i!=10) {
            if(maze->numPlayers<10) {
               (maze->numPlayers)++;
            }
            printf("Ok2\n");
            for (j = maze->numPlayers-1; j > i; j--) {
                maze->score[j] = maze->score[j-1];
                printf("Ok3\n");
                len = strlen(maze->namePlayers[j-1]) + 1;
                if(strlen(maze->namePlayers[j]) < len ) {
                    realloc(maze->namePlayers[j], len * sizeof(char));
                }
                strcpy(maze->namePlayers[j], maze->namePlayers[j-1]);
            }
            printf("Ok4\n");
            printf("%s\n", name);
            printf("%d\n", i);
            printf("On attend\n");
            printf("%s\n", name);
            printf("L'erreur apparait\n");
            maze->score[i] = count;
            len = strlen(name) + 1;
            if(((unsigned int) strlen(maze->namePlayers[i])) < len ) {
                realloc(maze->namePlayers[i], len * sizeof(char));
            }
            strcpy(maze->namePlayers[i], name);
        }
    }*/
    return !exitTheGame;
}

/**
 * @brief Print the menu and ask what the user want to do
 * 
 */
void menu() {
    int val;
    int len;
    int width;
    int height;
    char * name;
    int i;
    SaveFilesList saveFiles;
    Maze * maze = NULL;
    printf("\033[H\033[2J\033[36;01m");
    do{
        /*printf("          _____                    _____                    _____                    _____          \n");
        printf("         /\\    \\                  /\\    \\                  /\\    \\                  /\\    \\         \n");
        printf("        /::\\____\\                /::\\    \\                /::\\    \\                /::\\    \\        \n");
        printf("       /::::|   |               /::::\\    \\               \\:::\\    \\              /::::\\    \\       \n");
        printf("      /:::::|   |              /::::::\\    \\               \\:::\\    \\            /::::::\\    \\      \n");
        printf("     /::::::|   |             /:::/\\:::\\    \\               \\:::\\    \\          /:::/\\:::\\    \\     \n");
        printf("    /:::/|::|   |            /:::/__\\:::\\    \\               \\:::\\    \\        /:::/__\\:::\\    \\    \n");
        printf("   /:::/ |::|   |           /::::\\   \\:::\\    \\               \\:::\\    \\      /::::\\   \\:::\\    \\   \n");
        printf("  /:::/  |::|___|______    /::::::\\   \\:::\\    \\               \\:::\\    \\    /::::::\\   \\:::\\    \\  \n");
        printf(" /:::/   |::::::::\\    \\  /:::/\\:::\\   \\:::\\    \\               \\:::\\    \\  /:::/\\:::\\   \\:::\\    \\ \n");
        printf("/:::/    |:::::::::\\____\\/:::/  \\:::\\   \\:::\\____\\_______________\\:::\\____\\/:::/__\\:::\\   \\:::\\____\n");
        printf("\\::/    / ~~~~~/:::/    /\\::/    \\:::\\  /:::/    /\\::::::::::::::::::/    /\\:::\\   \\:::\\   \\::/    /\n");
        printf(" \\/____/      /:::/    /  \\/____/ \\:::\\/:::/    /  \\::::::::::::::::/____/  \\:::\\   \\:::\\   \\/____/ \n");
        printf("             /:::/    /            \\::::::/    /    \\:::\\~~~~\\~~~~~~         \\:::\\   \\:::\\    \\     \n");
        printf("            /:::/    /              \\::::/    /      \\:::\\    \\               \\:::\\   \\:::\\____\\    \n");
        printf("           /:::/    /               /:::/    /        \\:::\\    \\               \\:::\\   \\::/    /    \n");
        printf("          /:::/    /               /:::/    /          \\:::\\    \\               \\:::\\   \\/____/     \n");
        printf("         /:::/    /               /:::/    /            \\:::\\    \\               \\:::\\    \\         \n");
        printf("        /:::/    /               /:::/    /              \\:::\\____\\               \\:::\\____\\        \n");
        printf("        \\::/    /                \\::/    /                \\::/    /                \\::/    /        \n");
        printf("         \\/____/                  \\/____/                  \\/____/                  \\/____/         \n\n");*/
        printf("\033[36;01m          ____                                     \n");
        printf("        ,'  , `.                                   \n");
        printf("     ,-+-,.' _ |                  ,----,           \n");
        printf("  ,-+-. ;   , ||                .'   .`|           \n");
        printf(" ,--.'|'   |  || ,--.--.     .'   .'  .'   ,---.   \n");
        printf("|   |  ,', |  |,/       \\  ,---, '   ./   /     \\  \n");
        printf("|   | /  | |--'.--.  .-. | ;   | .'  /   /    /  | \n");
        printf("|   : |  | ,    \\__\\/: . . `---' /  ;--,.    ' / | \n");
        printf("|   : |  |/     ,\" .--.; |   /  /  / .`|'   ;   /| \n");
        printf("|   | |`-'     /  /  ,.  | ./__;     .' '   |  / | \n");
        printf("|   ;/        ;  :   .'   \\;   |  .'    |   :    | \n");
        printf("'---'         |  ,     .-./`---'         \\   \\  /  \n");
        printf("               `--`---'                   `----'   \n\n");
        val = 0;
        if(maze!=NULL){
            printf("\033[32;01mThe maze %s is loaded\033[0;01m\n\n",
                maze->mazeName);
        }else {
            printf("\033[31;01mThere isn't any maze loaded\033[0;01m\n\n");
        }
        printf("\033[35;01m   Menu\n");
        printf("    \033[34;01m1 - Créer un labyrinthe\n");
        printf("    \033[33;01m2 - Charger un labyrinthe\n");
        printf("    \033[32;01m3 - Jouer\n");
        printf("    \033[31;01m4 - Quitter\n\n\033[37;01m");
        val = getInt();
        printf("\033[H\033[2J\033[36;01m\033[0;01m");
        switch(val) {
            case 1:
                do{
                    printf("De quelle hauteur doit être le labyrinthe (minimum 5) ?\n");
                    height = getInt();
                }while(height<5);
                do{
                    printf("De quelle longueur doit être le labyrinthe (minimum 5) ?\n");
                    width = getInt();
                }while(width<5);
                maze = newMaze(width, height);
                name = promptSentence(
                    "Comment voulez-vous appelez ce nouveau labyrinthe (pas plus de 50 caractères) ?\n",
                    &len, true
                );
                save(*maze, name, len);
                if(promptBool("Do you want to load it ?")){
                    start(maze);
                }
                freeMaze(maze);
                maze = NULL;
                break;
            case 2:
                printf("\033[H\033[2J");
                saveFiles = listSaveFiles();
                i = selectSaveFile(saveFiles);
                printf("%s\n", saveFiles.saveFilesList[i]);
                maze = readSaveFile(saveFiles.saveFilesList[i]);
                break;
            case 3:
                printf("\033[H\033[2J");
                if(maze==NULL){
                    printf(
                       "\033[31;01mPlease load a maze before starting a game\n"
                    );
                }else{
                    start(maze);
                }
                break;
            case 4:
                printf("\033[H\033[2J");
                if(maze){
                    freeMaze(maze);
                    maze = NULL;
                }
                break;
            default:
                printf("\033[H\033[2J");
                break;
        }
	}while(val!=4);
}

int main(int argc, char ** argv) {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    #ifdef DEBUG
    int rand = 0;
    printf("Quelle valeur doit être utilisé pour le srand ? ");
    rand = getInt();
    srand(rand);
    #else
    srand(time(NULL));
    #endif
    menu();
    return 0;
}
