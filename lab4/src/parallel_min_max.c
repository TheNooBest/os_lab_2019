#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int _pnum = 0;
pid_t* childs = NULL;
bool time_success = true;

void sigalrm_handler(int signo) {
  if (signo == SIGALRM) {
    for (int i = 0; i < _pnum; i++) {
      kill(childs[i], SIGKILL);
    }
    time_success = false;
    printf("Childs killed\n");
  }
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  int timeout = 0;
  bool with_files = false;

  signal(SIGALRM, sigalrm_handler);

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {"timeout", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            // your code here
            // error handling
            break;
          case 1:
            array_size = atoi(optarg);
            // your code here
            // error handling
            break;
          case 2:
            pnum = atoi(optarg);
            _pnum = pnum;
            // your code here
            // error handling
            break;
          case 3:
            with_files = true;
            break;
          case 4:
            timeout = atoi(optarg);
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" [--timeout \"num\"]\n",
           argv[0]);
    return 1;
  }

  int *array = (int*)malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  int* pipesfd = NULL;
  if (!with_files) {
    pipesfd = (int*)malloc(sizeof(int) * 2 * pnum);
  }

  childs = (pid_t*)malloc(sizeof(pid_t) * pnum);
  for (int i = 0; i < pnum; i++) {
    if (!with_files)
      pipe(pipesfd + i * 2);

    pid_t child_pid = fork();

    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process

        // parallel somehow

        struct MinMax min_max;
        int left, right;
        left = (array_size * i) / pnum;
        right = (array_size * (i + 1)) / pnum;
        min_max = GetMinMax(array, left, right);
        // printf("%d:\tleft: %d, right: %d, min: %d, max: %d\n", i, left, right, min_max.min, min_max.max);

        sleep(2);

        if (with_files) {
          // use files here
          FILE* fp;
          char* filename;

          asprintf(&filename, "part%d.dat", i);
          fp = fopen(filename, "wb");
          fwrite(&min_max, sizeof(struct MinMax), 1, fp);
          fclose(fp);
        } else {
          // use pipe here
          close(pipesfd[i * 2]);    // close to read
          write(pipesfd[i * 2 + 1], &min_max, sizeof(struct MinMax));
          close(pipesfd[i * 2 + 1]);
        }
        return 0;
      }
      else {
        childs[i] = child_pid;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  alarm(timeout);

  while (active_child_processes > 0) {
    // your code here
    int wstatus;
    pid_t ret = wait(&wstatus);

    active_child_processes -= 1;
  }

  alarm(0);

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  if (time_success) {
    for (int i = 0; i < pnum; i++) {
      struct MinMax _min_max;

      if (with_files) {
        // read from files
        FILE* fp;
        char* filename;

        asprintf(&filename, "part%d.dat", i);
        fp = fopen(filename, "rb");
        fread(&_min_max, sizeof(struct MinMax), 1, fp);
        fclose(fp);
        remove(filename);
      } else {
        // read from pipes
        close(pipesfd[i * 2 + 1]);        // close to write
        read(pipesfd[i * 2], &_min_max, sizeof(struct MinMax));
        close(pipesfd[i * 2]);
      }

      if (_min_max.min < min_max.min) min_max.min = _min_max.min;
      if (_min_max.max > min_max.max) min_max.max = _min_max.max;
    }
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  free(pipesfd);
  free(childs);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
