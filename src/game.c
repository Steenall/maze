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

char ** listAllSaveFiles() {
    return NULL;
}

/**
 * @return if the user want to save the game, it will returns true, else, it returns false 
 */
bool start(Maze * maze) {
    char response;
    bool exitTheGame = false;
    Direction direction;
    do{
        printMaze(*maze);
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
                perror("There was a problem when parsing the direction");
                exit(1);
        }
        if(!exitTheGame && !move(maze, direction)) {
            printf("\033[31;01mYou cannot move to this direction\033[0;01m\n");
        }
    }while(!exitTheGame && !isGameFinished(*maze));
    return false;
}

void afficheMenu() {
    int val = 0;
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
        int val=0;
        printf("\033[35;01m   Menu\n    \033[34;01m1 - Jouer\n    \033[0;01m2 - Options\n    \033[31;01m3 - Quitter\n\n\033[37;01m");
        scanf("%d",&val);
        printf("\033[H\033[2J\033[36;01m\033[0;01m");
        switch(val) {
            case 1:
                if(maze!=NULL) {
                    if(!promptBool("A game is already loaded, would you like to reload it ?")) {
                        freeMaze(maze);
                        maze = newMaze();
                    }
                }
                else {
                    maze = newMaze();
                }
                printf("\033[H\033[2J");
                if(start(maze)) {
                    printf("Bravo, vous avez terminé ce labyrinthe.\n");
                    sleep(5);
                }
                else {
                    if(promptBool("Voulez vous sauvegarder ce labyrinthe ?")){
                        char * name = malloc(sizeof(char) * 50);
                        printf("Comment voulez-vous appelez le fichier de sauvegarde (Pas plus de 50 caractères) ?\n");
                        char car;
                        int i = 0;
                        while(i<50 && (car = getchar()) != '\n') {
                            name[i] = car;
                        }
                        name[49] = '\0';
                        printf("%s", name);
                        save(*maze, name);
                    }
                }
                break;
            case 2:
                printf("\033[H\033[2J");
                printf("\033[31;01mIl n'y a pas d'options pour le moment\033[0;01m\n");
                break;
            case 3:
                printf("\033[H\033[2J");
                exit(0);
            default:
                printf("\033[H\033[2J");
                break;
        }
	}while(val!=1);
    if(maze) {
        freeMaze(maze);
        maze = NULL;
    }
}
int main(void) {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    #ifdef DEBUG
    int rand;
    printf("Quelle valeur doit être utilisé pour le srand ? ");
    scanf("%d", &rand);
    srand(rand);
    #else
    srand(time(NULL));
    #endif
    afficheMenu();
    return 0;
}
