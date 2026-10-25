#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

void settermnoncannon();
void initstuff();
void configcam();
void checkSize();
void seeProJection();
struct point rasterise();

struct point {
  int x;
  int y;
  int z;
};

int charbuffer;
struct point camera;

struct point vector[4] = {{-1, -1, -1}, {-1, 1, -1}, {1, 1, -1}, {1, -1, -1}};

int main() {
  settermnoncannon();
  initstuff();

  int open = 1;
  do {
    printf("%s", "\033[2J\033[0;0H");
    printf("menu:\n  configCam\n  seeproJection\n  checkSize\n  Quit\n  > ");
    // printf("\033[4:5H");

    if ((charbuffer = getchar()) != EOF) {
      char chare = charbuffer;
      switch (chare) {
      case 'c':
        configcam();
        break;

      case 'j':
        seeProJection();
        break;

      case 's':
        checkSize();
        break;

      case 'q':
        open--;
        break;
      default:
        continue;
      }
    } else
      continue;
  } while (open);

  return 0;
}

void initstuff() {
  camera.x = 0;
  camera.y = 0;
}

void configcam() {
  int open = 1;
  do {
    printf("%s", "\033[2J\033[0;0H");
    printf("\033[%d:%dH", camera.x, camera.y);
    printf("camera %d %d", camera.x, camera.y);

    if ((charbuffer = getchar()) != EOF) {
      char chare = charbuffer;
      switch (chare) {
      case 'q':
        open--;
        break;

      case 'w':
        if (camera.x > 0) {
          camera.x--;
        }
        break;

      case 's':
        camera.x++;
        break;

      case 'a':
        if (camera.y > 0) {
          camera.y--;
        }
        break;

      case 'd':
        camera.y++;
        break;
      }
    }
  } while (open);
}

void seeProJection() {
  int open = 1;
  do {
    printf("%s", "\033[2J\033[0;0H");
    printf("\033[%d:%dH", camera.x, camera.y);
    // printf("camera %d %d", camera.x, camera.y);

    if ((charbuffer = getchar()) != EOF) {
      char chare = charbuffer;
      switch (chare) {
      case 'q':
        open--;
        break;
      }
    }
  } while (open);
}

void checkSize() {
  int open = 1;
  do {
    printf("%s", "\033[2J\033[0;0H");
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
      printf("Terminal size: %d rows, %d columns\n", w.ws_row, w.ws_col);
    }

    if ((charbuffer = getchar()) != EOF) {
      char chare = charbuffer;
      switch (chare) {
      case 'q':
        open--;
        break;
      }
    }
  } while (open);
}

void settermnoncannon() {
  struct termios oldt, newt;

  tcgetattr(0, &oldt);
  newt = oldt;
  newt.c_lflag &= ~ICANON; // invert ICANNON
  newt.c_cc[VMIN] = 1;     // control character 6 enabled
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  // set terminal to noncannonical mode
}

struct point rasterise() {

}
