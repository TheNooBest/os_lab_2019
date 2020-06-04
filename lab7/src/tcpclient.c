#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>

#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
  int fd;
  int nread;
  int port = 0;
  int buffsize = 0;
  char* addrstr = NULL;
  char* buf = NULL;
  struct sockaddr_in servaddr;

  // GETOPT
  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"addr", required_argument, 0, 0},
                                      {"port", required_argument, 0, 0},
                                      {"buff", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        asprintf(&addrstr, "%s", optarg);
        break;
      case 1:
        port = atoi(optarg);
        break;
      case 2:
        buffsize = atoi(optarg);
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (addrstr == NULL || port == 0 || buffsize == 0) {
    fprintf(stderr, "Using: %s --addrstr 127.0.0.1 --port 1234 --buff 100\n",
            argv[0]);
    return 1;
  }
  // GETOPT

  buf = (char*)malloc(sizeof(char) * buffsize);

  if (argc < 3) {
    printf("Too few arguments \n");
    goto error;
  }

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    goto error;
  }

  memset(&servaddr, 0, SIZE);
  servaddr.sin_family = AF_INET;

  if (inet_pton(AF_INET, addrstr, &servaddr.sin_addr) <= 0) {
    perror("bad address");
    goto error;
  }

  servaddr.sin_port = htons(port);

  if (connect(fd, (SADDR *)&servaddr, SIZE) < 0) {
    perror("connect");
    goto error;
  }

  write(1, "Input message to send\n", 22);
  while ((nread = read(0, buf, buffsize)) > 0) {
    // printf("debug: nread=%d, buf=\"%s\"\n", nread, buf);
    if (write(fd, buf, nread) < 0) {
      perror("write");
      goto error;
    }
  }

  free(buf);

  close(fd);
  exit(0);

error:
  free(buf);
  exit(1);
}
