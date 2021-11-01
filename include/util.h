/**
 * @file util.h
 * @brief Header of util.c
 * @author Steenall
 * @version 1.0.0
 * @date 30 october 2021
 */
#ifndef UTIL
#define UTIL

#include <stdbool.h>
#include "maze.h"
#include "saveManager.h"


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

/**
 * @brief A list of forbidden char (NTFS file limitations)
 * 
 */
#define FORBIDDEN_CHAR "<>:\"/\\|?*\0"

/**
 * @brief Enumeration of all ArrowKey with their values
 * 
 */
typedef enum {
    UPARROW=KEY_UP,
    DOWNARROW=KEY_DOWN,
    RIGHTARROW=KEY_RIGHT,
    LEFTARROW=KEY_LEFT
}ArrowKey;

/**
 * @brief Check whitch integer is the minimum
 * 
 * @param a An integer to test
 * @param b The second integer to test
 * @return int The minimum value
 */
int min(int a, int b);

/**
 * @brief Check whitch integer is the maximum
 * 
 * @param a An integer to test
 * @param b The second integer to test
 * @return int The maximum value
 */
int max(int a, int b);

/**
 * @brief Ask the user with a sentence to enter yes or no
 * 
 * @param sentence The sentence to print before asking to choose an answer
 * @return true if the user answer yes
 * @return false if the user answer no
 */
bool promptBool(char * sentence);

/**
 * @deprecated
 * 
 * @brief Check which direction the user want to go, it support zqsd key and arrow key
 * 
 * @return Direction The direction the player choose
 */
Direction movePlayer();

/**
 * @brief Ask the user to enter a char in raw terminal mode
 * 
 * @param sentence The sentence to print before asking to choose an answer
 * @param availableResponse All the response the user can enter
 * @param arrowKey true if the user can use arrow key
 * @return char The char in the available response
 */
char promptChar(char * sentence, char * availableResponse, bool arrowKey);

/**
 * @brief Ask the user to enter a sentence
 * 
 * @param sentence The sentence to print before asking to enter the answer
 * @param len The maximum length of the sentence
 * @param isAFile if true, NTFS file limitations will be check on the sentence
 * @return char* The sentence
 */
char * promptSentence(char * sentence, unsigned int * len, bool isAFile);

/**
 * @brief A top level function of scanf for decimal number which continue if
 * The input is wrong
 * 
 * @return int the number entered by the user
 */
int getInt();

/**
 * @brief Ask the user to select a savefiles in raw terminal mode
 * 
 * @param saves The list of savefiles
 * @return int The index of the savefile choosen
 */
int selectSaveFile(SaveFilesList saves);
#endif