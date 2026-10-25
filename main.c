#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

void settermnoncannon();
void initstuff();
void cursor();
void debugmenu();
void proJect();

struct point rotate_xz(struct point vec3, float angle);
struct point rasterise(struct point vec3);

struct point {
  float x;
  float y;
  float z;
};

pthread_mutex_t lockerooni = PTHREAD_MUTEX_INITIALIZER;
int charbuffer;
float scalebias = 0.14;
struct point cur;

struct point vector[2][4] = {{{-0.25, -0.25, 0.25},
                              {-0.25, 0.25, 0.25},
                              {0.25, 0.25, 0.25},
                              {0.25, -0.25, 0.25}},
                             {{-0.25, -0.25, -0.25},
                              {-0.25, 0.25, -0.25},
                              {0.25, 0.25, -0.25},
                              {0.25, -0.25, -0.25}}};
/*
struct point vector[2][4] = {
    {{-0.25, -0.25, 1}, {-0.25, 0.25, 1}, {0.25, 0.25, 1}, {0.25, -0.25, 1}},
    {{-0.25, -0.25, 2}, {-0.25, 0.25, 2}, {0.25, 0.25, 2}, {0.25, -0.25, 2}}};
*/

int main() {
  settermnoncannon();
  initstuff();

  int open = 1;
  do {
    printf("%s", "\033[2J\033[0;0H");
    printf("menu:\n  Cursor\n  proJect\n  deBug\n  Quit\n  > ");
    // printf("\033[4:5H");

    if ((charbuffer = getchar()) != EOF) {
      char chare = charbuffer;
      switch (chare) {
      case 'c':
        cursor();
        break;

      case 'j':
        proJect();
        break;

      case 'b':
        debugmenu();
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
  cur.x = 0;
  cur.y = 0;
}

void cursor() {
  int open = 1;
  do {
    printf("\033[2J\033[%.0f:%.0fH", cur.x, cur.z);
    printf(" cursor %f %f", cur.x, cur.z);
    printf("\033[%.0f:%.0fH", cur.x, cur.z);

    if ((charbuffer = getchar()) != EOF) {
      char chare = charbuffer;
      switch (chare) {
      case 'q':
        open--;
        break;

      case 'w':
        if (cur.x > 0) {
          cur.x--;
        }
        break;

      case 's':
        cur.x++;
        break;

      case 'a':
        if (cur.z > 0) {
          cur.z--;
        }
        break;

      case 'd':
        cur.z++;
        break;
      }
    }
  } while (open);
}

int rotedangul = 0;
int autor = 0;
int autord = 1;

void *render_thread(void *arg) {
/*
  while (1) {
    if (autor) {

      rotedangul = rotedangul + (autord == 0 ? -1 : autord);
    }

    usleep(40000);
  }
  return NULL;
*/
}

void proJect() {
  int open = 1, jointa = 0;
  pthread_t thread;
  //pthread_create(&thread, NULL, render_thread, NULL);

  struct point jointer = {0, 0, 1};
  jointer = rasterise(jointer);

  do {
    printf("\033[2J");
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 4; j++) {
        struct point potter =
            rotate_xz(vector[i][j], rotedangul * 3.14159f / 180.0f);
        potter = rasterise(potter);

        printf("\033[%d;%dH", (int)roundf(potter.y - potter.z / 2),
               (int)roundf(potter.x - potter.z / 2));
        printf("x");
      }
    }

    if (jointa) {
      printf("\033[%.0f;%.0fH", jointer.x, jointer.y);
      printf("p %d", rotedangul);
    }

    // printf("cur %d %d", cur.x, cur.y);

    if ((charbuffer = getchar()) != EOF) {
      char chare = charbuffer;
      switch (chare) {
      case 'q':
        open--;
        //pthread_cancel(thread);
        break;

      case 'j':
        jointa = !jointa;
        break;

      case 'a':
        pthread_mutex_lock(&lockerooni);
        rotedangul--;
        pthread_mutex_unlock(&lockerooni);
        break;

      case 'd':
        pthread_mutex_lock(&lockerooni);
        rotedangul++;
        pthread_mutex_unlock(&lockerooni);
        break;

      case 'h':
        pthread_mutex_lock(&lockerooni);
        autor = !autor;
        pthread_mutex_unlock(&lockerooni);
        break;
      case 'e':
        pthread_mutex_lock(&lockerooni);
        autord = !autord;
        pthread_mutex_unlock(&lockerooni);
        break;
      }
    }
  } while (open);
}

void debugmenu() {
  int open = 1;
  do {
    printf("\033[2J\033[0;0H");
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
      printf("terminal size: %d rows, %d columns\n", w.ws_row, w.ws_col);
    }
    printf("scale bias: %0.3f\n", scalebias);

    printf("geometry:\n");
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 4; j++) {
        struct point potter = vector[i][j];
        printf(" point[%d][%d] %.2f,%.2f\n", i, j, potter.x, potter.y);
      }
    }

    if ((charbuffer = getchar()) != EOF) {
      char chare = charbuffer;
      switch (chare) {
      case 'q':
        open--;
        break;
      case 'j':
        scalebias = scalebias - 0.001;
        break;
      case 'k':
        scalebias = scalebias + 0.001;
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

struct point rasterise(struct point vec3) {
  int wr, wc;
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
    wr = w.ws_row;
    wc = w.ws_col;
  } else {
    wr = 39;
    wc = 158;
  }
  float bias = (float)wc / (float)wr * scalebias;

  struct point newjoint = {(((bias * vec3.x + 1) / 2) * wc),
                           (((vec3.y + 1) / 2) * wr), vec3.z};
  return newjoint;
}

struct point rotate_xz(struct point vec3, float angle) {
  float c = cosf(angle);
  float s = sinf(angle);
  struct point roated = {
      vec3.x * c - vec3.z * s,
      vec3.y,
      vec3.x * s + vec3.z * c,
  };
  return roated;
}
