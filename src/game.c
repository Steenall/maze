#include "maze.h"
#include "saveManager.h"
#include "util.h"

#include <time.h>
#if defined _WIN32 || defined _WIN64
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @return if the user exit the maze manualy, it will returns true, else, it returns false 
 */
bool start(Maze * maze) {
    char response;
    bool exitTheGame = false;
    bool invalidPath = false;
    Direction direction;
    do{
        printMaze(*maze);
        if(invalidPath) {
            invalidPath = false;
            printf("\033[31;01mYou cannot move to this direction\033[0;01m\n");
        }
        response = promptChar("In witch direction do you want to go\n   z (↑) q (←) d (→) s (↓) e (exit)", "zqsde", true);
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
    return !exitTheGame;
}

void afficheMenu() {
    int val;
    int len;
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
            /*TODO*/
        }
        printf("\033[35;01m   Menu\n    \033[34;01m1 - Créer un labyrinthe\n    \033[33;01m2 - Charger un labyrinthe\n    \033[32;01m3 - Jouer\n    \033[31;01m4 - Quitter\n\n\033[37;01m");
        val = getInt();
        printf("\033[H\033[2J\033[36;01m\033[0;01m");
        switch(val) {
            case 1:
                maze = newMaze();
                name = promptSentence("Comment voulez-vous appelez ce nouveau labyrinthe (Pas plus de 50 caractères) ?\n", &len);
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
                    printf("\033[31;01mPlease load a maze before starting a game\n");
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
    int i;
    for(i=0; i<argc; i++) {
        printf("%s", argv[i]);
    }
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    #ifdef DEBUG
    int rand;
    printf("Quelle valeur doit être utilisé pour le srand ? ");
    getInt();
    srand(rand);
    #else
    srand(time(NULL));
    #endif
    afficheMenu();
    return 0;
}
