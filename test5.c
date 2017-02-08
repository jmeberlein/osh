#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"execute.h"
#include"parse.h"
#include"tokenize.h"

int main() {
  Pipe *p;
  Command *arr2;
  char **arr;
  char *tmp = "grep main - -q < test1.c && echo \"Success!\" || echo \"Failure\"";
  char *str = malloc(sizeof(char)*70);
  strcpy(str, tmp);
  int count, i, count2, count3, status = 0;

  tokenize(str, &arr, &count);
  parse(arr, count, &arr2, &count2);
  count3 = break_chain(arr2, count2, &p);
  for (i = 0; i < count3; i++){
    execute(&p[i], &status);
  }

  return 0;
}

