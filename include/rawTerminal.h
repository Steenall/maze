#ifndef RAWTERMINAL
#define RAWTERMINAL
void changemode(bool);
#ifndef _WIN32
int kbhit(void);
#endif
#endif