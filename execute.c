#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

#include"datatypes.h"

#define fp_null (open("/dev/null", O_RDONLY))

int execute(Pipe *p, int *status) {
  if (p == NULL || status == NULL) {
    return 1;
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

  for (i = start + 1; i < p->count; i++) {
    if (!(pids[i] = fork())) {
      dup2(pipes[i][0], 0);
      dup2(pipes[i+1][1], 1);

      for (j = start; j < i-1; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
      }
      close(pipes[i][1]);
      close(pipes[i+1][0]);
      for (j = i+2; j <= p->count; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
      }
      execv(argv[i][0], argv[i]);
      exit(1);
    }
  }

  for (i = start; i <= p->count; i++) {
    close(pipes[i][0]);
    close(pipes[i][1]);
  }

  for (i = start + 1; i < p->count; i++) {
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