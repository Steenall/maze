#ifndef UTIL
#define UTIL

#include <stdbool.h>

bool promptBool(char * sentence);
char promptChar(char * sentence, char * availableResponse, bool arrowKey);
char * promptSentence(char * sentence, int * len);
int getInt();
#endif