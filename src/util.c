/**
 * @file util.c
 * @brief Functions which could be needed by multiple files
 * @author Steenall
 * @version 1.0.0
 * @date 30 october 2021
 */
#include "util.h"
#include "maze.h"
#include "saveManager.h"
#include "rawTerminal.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a < b ? b : a;
}

bool promptBool(char * sentence) {
    char response;
    do{
        printf("%s [Y/N] ", sentence);
        response = (char) getchar();
    }while(response!='Y'&&response!='y'&&response!='n'&&response!='N');
    return response == 'Y'|| response == 'y';
}


char * promptSentence(char * sentence, unsigned int * len, bool isAFile) {
    char c;
    char * response;
    int i;
    bool verifNoForbiddenChar;
    bool verifNoSpaceAtBounds;
    char * illegalChar = {FORBIDDEN_CHAR};
    verifNoSpaceAtBounds = true;
    verifNoForbiddenChar = true;
    response = calloc(50, sizeof(char));
    i = 0;
    do{
        memset(response, 0, sizeof(char) * 50);
        if(!verifNoSpaceAtBounds) {
            printf("\033[31;01mVeuillez ne pas mettre d'espace à la fin ");
            printf("ou au début de cette chaine de caractère\033[0;01m\n");
        }else if(!verifNoForbiddenChar) {
            printf("\033[31;01mVeuillez ne pas rentrer de caractère interdit");
            printf("(restriction du système de fichier NTFS) {%s}\033[0;01m\n",
                    illegalChar
                );

        }else if(i==50) {
            printf("\033[31;01mVeuillez ne pas donner un nom de plus de ");
            printf("50 caractères\033[0;01m\n");
        }
        verifNoSpaceAtBounds = true;
        verifNoForbiddenChar = true;
        i = 0;
        printf("%s", sentence);
        c = (char) getchar();
        if(c!='\n'){
            if(!isAFile || strchr(illegalChar, c)==0){
                if(c==' '){
                    verifNoSpaceAtBounds = false;
                }else{
                    verifNoSpaceAtBounds = true;
                    response[i++] = c;
                }
            }else {
                verifNoForbiddenChar = false;
            }
        }
        while(i < 48 && (c = (char) getchar()) != '\n'){
            if(!isAFile || strchr(illegalChar, c)==0){
                if(c==' '){
                    verifNoSpaceAtBounds = false;
                }else{
                    verifNoSpaceAtBounds = true;
                    response[i++] = c;
                }
            }else {
                verifNoForbiddenChar = false;
                continue;
            }
        }
    }while((isAFile && !verifNoForbiddenChar) ||
            i==0 || !verifNoSpaceAtBounds);
    *len = i;
    return response;
}

/*Direction movePlayer() {
    char directions[4] = "zqsd";
    char response;
    bool specialChar;
    bool verifResponse;
    changemode(1);
    do {
        verifResponse = false;
        specialChar = false;
        while ( !kbhit() );
        response = (char) getchar();
        printf("%d\n", response);
        if(response=='\033'){
            specialChar = true;
            getchar();
            response = (char) getchar();
        }
    }while( (specialChar && strstr(directions, &response) == 0) &&
            !(specialChar && strstr(directions, ARROW_KEYS)));

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
}*/

char promptChar(char * sentence, char * availableResponse, bool arrowKey) {
    char response;
    bool specialChar;
    do {
        printf("%s", sentence);
        specialChar = false;
        changemode(1);
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
        changemode(0);
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
    int result;
    do{
        result = scanf("%d", &val);
        if (result == EOF) {
            printf("Impossible de lire le flux d'entrée");
            exit(1);
        }
        if (result == 0) {
            while (fgetc(stdin) != '\n');
        }
    }while(result==EOF||result==0);
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
            break;
        default:
            break;
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