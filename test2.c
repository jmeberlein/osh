#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

#include"datatypes.h"
#include"functions.h"

int main() {
  Pipe *p = malloc(sizeof(Pipe)*3);

  p[0].in = open("test.c", O_RDONLY);
  p[0].out = 1;
  p[0].count = 1;
  p[0].type = ON_SUCCESS;

  p[0].argv = malloc(sizeof(char**) * 1);
  p[0].argv[0] = malloc(sizeof(char*) * 5);
  p[0].argv[0][0] = "/bin/grep";
  p[0].argv[0][1] = "main";
  p[0].argv[0][2] = "-";
  p[0].argv[0][3] = "-q";
  p[0].argv[0][4] = NULL;


  p[1].in = 0;
  p[1].out = 1;
  p[1].count = 1;
  p[1].type = ON_FAILURE;

  p[1].argv = malloc(sizeof(char**) * 1);
  p[1].argv[0] = malloc(sizeof(char*) * 3);
  p[1].argv[0][0] = "/bin/echo";
  p[1].argv[0][1] = "Success!";
  p[1].argv[0][2] = NULL;


  p[2].in = 0;
  p[2].out = 1;
  p[2].count = 1;
  p[2].type = ALWAYS;

  p[2].argv = malloc(sizeof(char**) * 1);
  p[2].argv[0] = malloc(sizeof(char*) * 3);
  p[2].argv[0][0] = "/bin/echo";
  p[2].argv[0][1] = "Failure...";
  p[2].argv[0][2] = NULL;

  
  int status = 0;
  int i;
  for (i = 0; i < 3; i++){
    execute(&p[i], &status);
  }
}
