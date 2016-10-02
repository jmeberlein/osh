#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"tokenize.h"

int main() {
  char **arr;
  char *tmp = "Hello World      'Single Quote'\t\"Double Quote\" '\"Nested'\"\"";
  char *str = malloc(sizeof(char)*59);
  strcpy(str, tmp);
  int count, i;

  tokenize(str, &arr, &count);

  for (i = 0; i < count; i++) {
    printf("%s\n", arr[i]);
  }

  return 0;
}
