#include "util.h"
#include "maze.h"
#include "saveManager.h"
#include "rawTerminal.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#if defined _WIN32 || defined _WIN64
#define KEY_UP 'H'
#define KEY_DOWN 'K'
#define KEY_RIGHT 'P'
#define KEY_LEFT 'M'
#define ARROW_KEYS "HKPM"
#else
#define KEY_UP 'A'
#define KEY_DOWN 'B'
#define KEY_RIGHT 'C'
#define KEY_LEFT 'D'
#define ARROW_KEYS "ABCD"
#endif

typedef enum {UPARROW=KEY_UP, DOWNARROW=KEY_DOWN, RIGHTARROW=KEY_RIGHT, LEFTARROW=KEY_LEFT}ArrowKey;

bool promptBool(char * sentence) {
    char response;
    do{
        printf("%s [Y/N] ", sentence);
        response = getchar();
    }while(response!='Y'&&response!='y'&&response!='n'&&response!='N');
    return response == 'Y'|| response == 'y';
}


char * promptSentence(char * sentence, int * len) {
    char c;
    char * response;
    int i;
    i = 0;
    response = malloc(sizeof(char) * 50);
    printf("%s", sentence);
    c = getchar();
    if(c!='\n'){
        response[i++] = c;
    }
    while(i < 50 && (c = getchar()) != '\n'){
        response[i++] = c;
    }
    printf("----\n%s\n----", response);
    return response;
}

char movePlayer() {
    char directions[4] = "zqsd";
    char response;
    bool specialChar;
    changemode(1);
    do {
        specialChar = false;
        while ( !kbhit() );
        response = (char) getchar();
        printf("%d\n", response);
        if(response=='\033'){
            specialChar = true;
            printf("%d\n", getchar());
            response = (char) getchar();
        }
    }while( (specialChar && strstr(directions, &response) == 0) &&
            !(specialChar && strstr(directions, ARROW_KEYS)));

    changemode(0);
    if(specialChar && response){
        response = response - UPARROW; /* Transform arrow key to zqsd key */
    }else {
        response = *strchr(directions, response);
    }
    return (Direction) response;
}

char promptChar(char * sentence, char * availableResponse, bool arrowKey) {
    char response;
    bool specialChar;
    changemode(1);
    do {
        specialChar = false;
        while ( !kbhit() );
        response = (char) getchar();
        #ifdef DEBUG
        printf("%d\n", response);
        #endif
        if(response=='\033'){
            specialChar = true;
            #ifdef DEBUG
            printf("%d\n", getchar());
            #else
            getchar();
            #endif
            response = (char) getchar();
        }
    }while( (!specialChar && strstr(availableResponse, &response) == 0) &&
        !(specialChar && arrowKey && strstr(ARROW_KEYS, &response)));
    changemode(0);
    if(specialChar && response){
        switch (response)
        {
            case KEY_UP:
                response = 'z';
                break;
            case KEY_DOWN:
                response = 's';
                break;
            case KEY_RIGHT:
                response = 'd';
                break;            
            default:
                response = 'q';
                break;
        }
    }
    return response;
    /* do{
        printf("%c ", sentence);
        response = tolower(getchar());
    }while(strstr(availableResponse, &response)==0);
    return response;*/
}

int getInt(){
    int val;
    if(scanf("%d",&val)!=1){
        printf("Error when using scanf %d", errno);
        exit(1);
    }
    return val;
}

int selectSaveFile(SaveFilesList saves) {
    char response;
    int selected;
    int i;
    char * c;
    selected = 0;
    response = getchar();
    printf("\033[?25l");
    do {
        printf("\033[H\033[2J");
        printf("Fichiers de sauvegardes :\n");
        for(i=0; i< saves.length; i++) {
            if(selected == i) {
                c = "\033[47m \033[0m";
            }else{
                c = " ";
            }
            printf("\t[%s] %s\n", c, saves.saveFilesList[i]);
        }
        changemode(1);
        while ( !kbhit() );
        response = (char) getchar();
        changemode(0);
        switch (response)
        {
        case 'z':
            if(selected > 0){
                selected--;
            }
            break;
        case 's':
            if(selected < saves.length -1){
                selected++;
            }
            break;
        case '\033':
            getchar();
            response = (char) getchar();
            if(response == KEY_UP){
                if(selected > 0){
                    selected--;
                }
            }else if(response==KEY_DOWN){
                if(selected < saves.length -1){
                    selected++;
                }
            }
        }
        printf("%d\n", response);
        printf("%d\n", selected);
    }while(response!='\x0D' && response != '\x0A' && response != 32);
    /* Detect CR, LF or space */
    printf("\033[?25h");
    return selected;
}

#ifdef DEBUG_UTIL
int main(void) {
    changemode(1);
    for(int i=0; i<100;i++) {
        while ( !kbhit() );
        printf("%d\n", (char) getchar());
    }
}
#endif