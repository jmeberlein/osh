typedef enum { ALWAYS = 0, ON_SUCCESS, ON_FAILURE, PIPE, READ, WRITE, APPEND } Link;

typedef struct {
  int in;
  int out;
  int count;
  char ***argv;
  Link type;
} Pipe;

