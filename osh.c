#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"execute.h"
#include"parse.h"
#include"tokenize.h"

int main() {
  char *str = (char*)malloc(sizeof(char)*256);
  char **tok;
  Command *c;
  Pipe *p;
  int i, j, status, c_count, p_count, t_count;

  printf("osh>");
  while (fgets(str, 256, stdin) != NULL && strcmp(str, "exit\n")) {
      tokenize(str, &tok, &t_count);
      parse(tok, t_count, &c, &c_count);
      p_count = break_chain(c, c_count, &p);
      for (status = i = 0; i < p_count; i++){
        execute(&p[i], &status);
        /*
        for (j = 0; p[i].argv[j] != NULL; j++) {
          free(p[i].argv[j]);
        }
        free(p[i].argv);*/
      }     
      //free(p);
      //free(c);
      printf("osh>");
  }

  return 0;
}
