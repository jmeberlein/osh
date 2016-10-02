#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"execute.h"
#include"parse.h"
#include"tokenize.h"

int main() {
  Command *arr2;
  char **arr;
  char *tmp = "grep main - < test1.c && echo \"Success!\" || echo \"Failure\"";
  char *str = malloc(sizeof(char)*63);
  strcpy(str, tmp);
  int count, i, count2;

  tokenize(str, &arr, &count);

  for (i = 0; i < count; i++) {
    printf("%s\n", arr[i]);
  }

  parse(arr, count, &arr2, &count2);
  printf("%d\n", count2);

  printf("%s < %s > %s\n", arr2[0].argv[0], arr2[0].in, arr2[0].out);
  for (i = 1; i < 3; i++) {
    printf("\t%s\n", arr2[0].argv[i]);
  }

  printf("%s < %s > %s\n", arr2[1].argv[0], arr2[1].in, arr2[1].out);
  for (i = 1; i < 2; i++) {
    printf("\t%s\n", arr2[1].argv[i]);
  }
  
  printf("%s < %s > %s\n", arr2[2].argv[0], arr2[2].in, arr2[2].out);
  for (i = 1; i < 2; i++) {
    printf("\t%s\n", arr2[2].argv[i]);
  }  

  return 0;
}

