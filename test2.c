#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

#include"datatypes.h"
#include"functions.h"

int main() {
  Command *c = malloc(sizeof(Command)*4);

  c[0].in = "test.c";
  c[0].out = "";
  c[0].type = PIPE;

  c[0].argv = malloc(sizeof(char*) * 4);
  c[0].argv[0] = "/bin/grep";
  c[0].argv[1] = "main";
  c[0].argv[2] = "-";
  c[0].argv[3] = NULL;

  c[1].in = "";
  c[1].out = "";
  c[1].type = ON_SUCCESS;

  c[1].argv = malloc(sizeof(char*) * 2);
  c[1].argv[0] = "/bin/cat";
  c[1].argv[1] = NULL;

  c[2].in = "";
  c[2].out = "";
  c[2].type = ON_FAILURE;

  c[2].argv = malloc(sizeof(char*) * 3);
  c[2].argv[0] = "/bin/echo";
  c[2].argv[1] = "Success!";
  c[2].argv[2] = NULL;

  c[3].in = "";
  c[3].out = "";
  c[3].type = ALWAYS;

  c[3].argv = malloc(sizeof(char*) * 3);
  c[3].argv[0] = "/bin/echo";
  c[3].argv[1] = "Failure...";
  c[3].argv[2] = NULL;

  Pipe *p;
  int count = break_chain(c, 4, &p);
  
  int status = 0;
  int i;
  for (i = 0; i < count; i++){
    execute(&p[i], &status);
  }
}
