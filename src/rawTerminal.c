#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdbool.h>
#include <rawTerminal.h>
#if defined _WIN32 || defined _WIN64
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#endif

/**
 * Inspiré par :
 * https://stackoverflow.com/questions/13114301/raw-terminal-mode-how-to-take-in-input
 **/

int main2(void)
{
  int ch;
  changemode(1);
  while ( !kbhit() );      /* Waiting for some keyboard input.*/

  /* something has been detected. now get that.*/
  ch = getchar();

  printf("\nGot %c\n", ch);

  changemode(0);
  return 0;
}

void changemode(bool dir)
{
  static struct termios oldTerminal, newTerminal;

  if ( dir)
  {
    tcgetattr( STDIN_FILENO, &oldTerminal);
    newTerminal = oldTerminal;
    /*&= : et binaire
     ~() : négation binaire*/
    newTerminal.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newTerminal);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldTerminal);
}
#ifndef _WIN32
/*Cette fonction est disponible sur windows*/
int kbhit (void) 
{
  struct timeval tv;
  fd_set rdfs;

  tv.tv_sec = 0;
  tv.tv_usec = 0;

  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);

  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);

}
#endif
