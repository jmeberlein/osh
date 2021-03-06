#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

#include"execute.h"

int main() {
  Pipe *p = malloc(sizeof(Pipe));
  p->in = 0;
  p->out = 1;
  p->count = 3;
  p->type = 0;

  p->argv = malloc(sizeof(char**) * 3);
  p->argv[0] = malloc(sizeof(char*) * 3);
  p->argv[0][0] = "ls";
  p->argv[0][1] = "-l";
  p->argv[0][2] = NULL;

  p->argv[1] = malloc(sizeof(char*) * 4);
  p->argv[1][0] = "grep";
  p->argv[1][1] = "test";
  p->argv[1][2] = "-";
  p->argv[1][3] = NULL;

  p->argv[2] = malloc(sizeof(char*) * 2);
  p->argv[2][0] = "cat";
  p->argv[2][1] = NULL;
  
  int status = 0;
  execute(p, &status);

  return 0;
}
