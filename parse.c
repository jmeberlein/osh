#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"execute.h"

#define IS_LINK(str) (!(strcmp(str, "&&") && strcmp(str, "||") && strcmp(str, "|") && strcmp(str, ";")))
#define IS_FILE(str) (!(strcmp(str, "<") && strcmp(str, ">")))

void scrub(char *str) {
  int i, j, state = 1;
  for (i = j = 0; str[i] != '\0'; i++) {
    switch (state) {
    case 1:
      switch (str[i]) {
      case '\\':
        state = 2;
        continue;
      case '"':
        state = 5;
        continue;
      case '\'':
        state = 3;
        continue;
      default:
        str[j++] = str[i];
        state = 1;
        continue;
      }
      
    case 2:
      switch (str[i]) {
      default:
        str[j++] = str[i];
        state = 1;
        continue;
      }
    
    case 3:
      switch (str[i]) {
      case '\'':
        state = 1;
        continue;
      default:
        str[j++] = str[i];
        state = 3;
        continue;
      }

    case 4:
      switch (str[i]) {
      case '\\':
        state = 5;
        continue;
      case '"':
        state = 1;
        continue;
      default:
        str[j++] = str[i];
        state = 4;
        continue;
      }

    case 5:
      switch (str[i]) {
      case '"':
        state = 1;
        continue;
      default:
        str[j++] = str[i];
        state = 5;
        continue;
      }
    }
  }

  str[j] = '\0';
}

int parse(char **tok, int in_size, Command **arr, int *out_size) {
  if (tok == NULL || arr == NULL || out_size == NULL) {
    return 1;
  }

  int i, j, k, n, count, state;
  state = *out_size = 0;
  for (i = 0; i < in_size; i++) {
    switch (state) {
    case 0:
      if (IS_LINK(tok[i]) || IS_FILE(tok[i])) {
        return 2;
      }
      (*out_size)++;
      state = 1;
      continue;

    case 1:
      if (IS_LINK(tok[i])) {
        state = 0;
      } else if (IS_FILE(tok[i])) {
        state = 2;
      }
      continue;

    case 2:
      if (IS_LINK(tok[i]) || IS_FILE(tok[i])) {
        return 2;
      }
      state = 1;
      continue;
    }
  }

  *arr = (Command*)malloc(sizeof(Command)*(*out_size));
  
  j = k = n = 0;
  count = 0;
  for (i = 0; i < in_size; i++) {
    if (IS_FILE(tok[i])) {
      i++;
    } else if (!IS_LINK(tok[i])) {
      count++; // found an argument
    } 
    if (IS_LINK(tok[i]) || i == in_size - 1) { // reached end
      (*arr)[k].in = NULL;
      (*arr)[k].out = NULL;
      (*arr)[k].type = !strcmp(tok[i], "&&") ? ON_SUCCESS :
                       !strcmp(tok[i], "||") ? ON_FAILURE :
                       !strcmp(tok[i], "|") ? PIPE : ALWAYS;
      (*arr)[k].argv = (char**)malloc(sizeof(char*)*(count+1));
      for (; j < i; j++) {
        if (!strcmp(tok[j], "<")) {
          if ((*arr)[k].in != NULL) return 3;
          (*arr)[k].in = tok[++j];
        } else if (!strcmp(tok[j], ">")) {
          if ((*arr)[k].out != NULL) return 3;
          (*arr)[k].out = tok[++j];
        } else {
          (*arr)[k].argv[n++] = tok[j];
        }
        scrub(tok[j]);
      }
      if (i == in_size - 1 && (i == 0 || !IS_FILE(tok[i-1]))) {
        (*arr)[k].argv[n++] = tok[i];
        scrub(tok[i]);
      }
      (*arr)[k].argv[count] = 0; // null terminate argv

      count = n = 0;
      k++; j++;
    }
  }
  
  return 0;
}
