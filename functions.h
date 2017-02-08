int execute(Pipe *p, int *status);
int break_chain(Command *commands, int n, Pipe **out);
int tokenize(char *str, char ***arr, int *count);
