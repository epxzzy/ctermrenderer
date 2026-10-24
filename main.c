#include <stdio.h>

#include <termios.h>
#include <unistd.h>

int main() {
  struct termios oldt, newt;

  tcgetattr(0, &oldt);
  newt = oldt;
  newt.c_lflag &= ~ICANON; // bitwise and ICANNON
  newt.c_cc[VMIN] = 1;     // control character 6 enabled
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  // set terminal to noncannonical mode

  int close = 0;
  do {
    int c;
    if ((c = getchar()) != EOF) {
      printf("%s", "\033[2J\033[0;0H");
      char chare = c;

      printf("%s", &chare);
    }
  } while (close != 1);

  return 0;
}
