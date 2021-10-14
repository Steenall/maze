#include "util.h"
#include "maze.h"
#include "rawTerminal.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#if defined __CYGWIN__

#define KEY_UP 'A'
#define KEY_DOWN 'B'
#define KEY_RIGHT 'C'
#define KEY_LEFT 'D'
#define ARROW_KEYS "ABCD"
#elif defined _WIN32 || defined _WIN64
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


char * promptSentence(char * sentence) {
    char * response = malloc(sizeof(char) * 50);
    printf("%s ", sentence);
    scanf("%[\n]\n", response);
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
        response = response - UPARROW; // Transform arrow key to zqsd key
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
        printf("%d\n", response);
        if(response=='\033'){
            specialChar = true;
            printf("%d\n", getchar());
            response = (char) getchar();
        }
    }while( (!specialChar && strstr(availableResponse, &response) == 0) &&
        !(specialChar && arrowKey && response >= (char) UPARROW && response <= (char) LEFTARROW));
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
    /*
    do{
        printf("%c ", sentence);
        response = tolower(getchar());
    }while(strstr(availableResponse, &response)==0);
    return response;*/
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