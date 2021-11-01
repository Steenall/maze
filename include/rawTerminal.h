/**
 * @file rawTerminal.h
 * @brief Header of rawTerminal.c
 * @author Steenall
 * @version 1.0.0
 * @date 30 october 2021
 */
#ifndef RAWTERMINAL
#define RAWTERMINAL

/**
 * @brief Change the terminal mode between standart and raw terminal
 * (can interpret all keyboard event).
 * Inspired by :
 * https://stackoverflow.com/questions/13114301/raw-terminal-mode-how-to-take-in-input
 * 
 * @param dir If true, change the terminal mode into raw terminal,
 * else change it back to standard terminal
 */
void changemode(bool raw);

#ifndef _WIN32

/**
 * @brief Check when a keyboard event occur on raw terminal mode
 * (not needed on windows)
 * 
 * @return int 1 if an event occur, else return 0
 */
int kbhit(void);
#endif
#endif