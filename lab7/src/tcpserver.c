#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SADDR struct sockaddr

int main(int argc, char *argv[]) {
  const size_t kSize = sizeof(struct sockaddr_in);

  int lfd, cfd;
  int nread;
  int port = 0;
  int buffsize = 0;
  char* buf = NULL;
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  // GETOPT
  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"port", required_argument, 0, 0},
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
        port = atoi(optarg);
        break;
      case 1:
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

  if (port == 0 || buffsize == 0) {
    fprintf(stderr, "Using: %s --port 1234 --buff 100\n",
            argv[0]);
    return 1;
  }
  // GETOPT

  buf = (char*)malloc(sizeof(char) * buffsize);

  if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    goto error;
  }

  memset(&servaddr, 0, kSize);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
    perror("bind");
    goto error;
  }

  if (listen(lfd, 5) < 0) {
    perror("listen");
    goto error;
  }

  while (1) {
    unsigned int clilen = kSize;

    if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
      perror("accept");
      goto error;
    }
    printf("connection established\n");

    while ((nread = read(cfd, buf, buffsize)) > 0) {
      // write(1, &buf, nread);
      // printf("debug: nread=%d\n", nread);
      buf[nread] = '\0';
      printf("%s", buf);
    }

    if (nread == -1) {
      perror("read");
      goto error;
    }
    close(cfd);
  }

error:
  free(buf);
  exit(1);
}
