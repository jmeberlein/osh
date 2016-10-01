#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

#include"datatypes.h"
#include"functions.h"

int main() {
  char **arr;
  char *str = "Hello World 'Single Quote' \"Double Quote\" '\"Nested'\"\"";
  int count;

  tokenize(str, &arr, &count);
  return 0;
}
