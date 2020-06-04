#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char **argv) {
  int sockfd, n;
  int port = 0;
  int buffsize = 0;
  char* addrstr = NULL;
  char* sendline = NULL;
  char* recvline = NULL;
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

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

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  if (inet_pton(AF_INET, addrstr, &servaddr.sin_addr) < 0) {
    perror("inet_pton problem");
    free(addrstr);
    exit(1);
  }
  free(addrstr);
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  write(1, "Enter string\n", 13);

  sendline = (char*)malloc(sizeof(char) * buffsize);
  recvline = (char*)malloc(sizeof(char) * buffsize);

  while ((n = read(0, sendline, buffsize)) > 0) {
    if (sendto(sockfd, sendline, n, 0, (SADDR *)&servaddr, SLEN) == -1) {
      perror("sendto problem");
      goto error;
    }

    if (recvfrom(sockfd, recvline, buffsize, 0, NULL, NULL) == -1) {
      perror("recvfrom problem");
      goto error;
    }

    printf("REPLY FROM SERVER= %s\n", recvline);
  }
  close(sockfd);

error:
  free(sendline);
  free(recvline);
  exit(1);
}
