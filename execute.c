#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

#include"execute.h"

#define fp_null (open("/dev/null", O_RDONLY))

int execute(Pipe *p, int *status) {
  if (p == NULL || status == NULL) {
    return 1;
  }

  if (*status && p->count == 1) {
    *status = p->type == ON_FAILURE ? 0 : 1;
    return 0;
  }

  int i, j, start, in, out, l_status;
  int pipes[p->count + 1][2];
  int pids[p->count];

  // Status = 0 signals execution of the first command
  // based on the POSIX standard of returning 0 to indicate success
  if (!*status) {
    start = 0;
    in = p->in;
    out = p->out;
  } else {
    start = 1;
    in = fp_null;
    out = p->out;
  }

  pipes[start][0] = in;
  pipes[start][1] = fp_null;
  pipes[p->count][0] = fp_null;
  pipes[p->count][1] = out;
  for (i = start + 1; i < p->count; i++) {
    pipe(pipes[i]);
  }

  for (i = start; i < p->count; i++) {
    if (!(pids[i] = fork())) {
      dup2(pipes[i][0], 0);
      dup2(pipes[i+1][1], 1);

      // Close all but the required file descriptors,
      // skipping 1 and 0 (stdout and stdin)
      for (j = start; j < i; j++) {
        if (pipes[j][0] != 0) close(pipes[j][0]);
        if (pipes[j][1] != 1) close(pipes[j][1]);
      }
      close(pipes[i][1]);
      close(pipes[i+1][0]);
      for (j = i+2; j <= p->count; j++) {
        if (pipes[j][0] != 0) close(pipes[j][0]);
        if (pipes[j][1] != 1) close(pipes[j][1]);
      }
      execvp(p->argv[i][0], p->argv[i]);
      exit(1);
    }
  }

  for (i = start; i <= p->count; i++) {
    if (pipes[i][0] != 0) close(pipes[i][0]);
    if (pipes[i][1] != 1) close(pipes[i][1]);
  }

  for (i = start; i < p->count; i++) {
    waitpid(pids[i], &l_status, 0);
  }

  if (p->type == ALWAYS) {
    *status = 0;
  } else if (WIFEXITED(l_status) && !WEXITSTATUS(l_status)) {
    *status = p->type == ON_SUCCESS ? 0 : 1;
  } else {
    *status = p->type == ON_FAILURE ? 0 : 1;
  }

  return 0;
}

int break_chain(Command *commands, int n, Pipe **out) {
  if (commands == NULL || out == NULL || n == 0) {
    return 0;
  }

  int i, j, k, m, c, count;
  
  count = 1;
  for (i = 0; i < n-1; i++) {
    if (commands[i].type != PIPE || commands[i].out != 0 || commands[i+1].in != 0) {
      count++;
    } 
  }

  *out = (Pipe*)malloc(sizeof(Pipe)*count);
  c = 0;
  i = j = -1;
  while (i < n - 1) {
    c++; i++;
    if (i == n || commands[i].type != PIPE || commands[i].out != 0 || commands[i+1].in != 0) {
      (*out)[m].in = commands[j+1].in == 0 ? 0 : open(commands[j+1].in, O_RDONLY);
      (*out)[m].out = commands[i].out == 0 ? 1 : open(commands[i].out, O_WRONLY | O_CREAT, 0664);
      (*out)[m].type = commands[i].type;
      (*out)[m].count = c;
      (*out)[m].argv = (char***)malloc(sizeof(char**)*c);
      c = k = 0;
      while (j != i) {
        (*out)[m].argv[k++] = commands[++j].argv;
      }
      m++;
    }
  }

  return count;
}
