#include<stdlib.h>

int tokenize(char *str, char ***arr, int *count) {
  if (str == NULL || arr == NULL || count == NULL) {
    return 1;
  }

  int i, j, state;
  
  state = 0;
  *count = 0;
  for (i = 0; str[i] != '\0'; i++) {
    switch (state) {
    case 0:
      switch (str[i]) {
      case ' ':
      case '\t':
      case '\n':
        state = 0;
        continue;
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
        state = 1;
        continue;
      }
    
    case 1:
      switch (str[i]) {
      case ' ':
      case '\t':
      case '\n':
        state = 0;
        (*count)++;
        continue;
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
        state = 1;
        continue;
      }
      
    case 2:
      switch (str[i]) {
      default:
        state = 1;
        continue;
      }
    
    case 3:
      switch (str[i]) {
      case '\'':
        state = 1;
        continue;
      default:
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
        state = 4;
        continue;
      }

    case 5:
      switch (str[i]) {
      case '"':
        state = 1;
        continue;
      default:
        state = 5;
        continue;
      }
    }
  }

  if (state == 1) {
    (*count)++;
  } else if (state > 1) {
    return 2;
  }

  *arr = (char**)malloc(sizeof(char*)*(*count));

  j = 0;
  state = 0;
  for (i = 0; str[i] != '\0'; i++) {
    switch (state) {
    case 0:
      switch (str[i]) {
      case ' ':
      case '\t':
      case '\n':
        state = 0;
        continue;
      case '\\':
        (*arr)[j++] = &str[i];
        state = 2;
        continue;
      case '"':
        (*arr)[j++] = &str[i];
        state = 5;
        continue;
      case '\'':
        (*arr)[j++] = &str[i];
        state = 3;
        continue;
      default:
        (*arr)[j++] = &str[i];
        state = 1;
        continue;
      }
    
    case 1:
      switch (str[i]) {
      case ' ':
      case '\t':
      case '\n':
        str[i] = '\0';
        state = 0;
        continue;
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
        state = 1;
        continue;
      }
      
    case 2:
      switch (str[i]) {
      default:
        state = 1;
        continue;
      }
    
    case 3:
      switch (str[i]) {
      case '\'':
        state = 1;
        continue;
      default:
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
        state = 4;
        continue;
      }

    case 5:
      switch (str[i]) {
      case '"':
        state = 1;
        continue;
      default:
        state = 5;
        continue;
      }
    }
  }
  
  return 0;
}
