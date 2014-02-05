/*
 * testing communication
 * listening to messages
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include "communicate.h"

int port;

/* send command to the others */
void send_cmd(int bfd)
{
  int x = 99;

  // create a message
  int nbytes = 0;
  char *msg = NULL;
  char *char_from = NULL;
  char *char_x = NULL;

  msg = (char *) malloc(60*sizeof(char));
  char_from = (char *) malloc(10*sizeof(char));
  char_x = (char *) malloc(10*sizeof(char));

  itoa((int)port, char_from);
  itoa((int)x, char_x);

  strcpy(msg, "C");
  strcat(msg, char_from);
  strcat(msg, "$");
  strcat(msg, char_x);
  strcat(msg, "!");

  //printf("sending message: %s\n", msg);
  nbytes = talk_to_all(bfd, msg, H);
}

void parse_msg(char *msg)
{
  char *ptr, *token;

  token = strtok(msg, "!");
  while (token != NULL) {
    if (token[0] = 'C') {
      printf("This is a C type of message\n");
    }
    ptr = strstr(token, "C");
    ptr++;
    printf("port number %d\n", atoi(ptr));
    ptr = strstr(token, "$");
    ptr++;
    printf("x position is %d\n", atoi(ptr));
    token = strtok(NULL, "!");
  }
}

/* main function */
int main(int argc, char** argv)
{
  int nbytes = 0;
  char msg[MAXBUF];

  if (argc < 2) {
    printf("mytest port\n");
    exit(1);
  }
  port = atoi(argv[1]);

  // initiate communication
  int lfd = create_listen(port, H); // listening fd
  int bfd = create_broadcast(port, H); // broadcast fd

  while (1) {
    send_cmd(bfd);
    nbytes = listen_to_robot(lfd, msg);
    if (nbytes == 0) continue;
    printf("I received message: %s\n", msg);
    parse_msg(msg);
    sleep(3);
  }
}
