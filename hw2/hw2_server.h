#define MAXCOMMANDLEN 10
#define MAXFILENAME 20
#define MAXSTRLEN 1000

typedef char *filetype;
struct file{
  filetype filename;
  struct file *next;
};

typedef struct file file;

file* listfiles();
long lookup(char *);
int create(char *);
int delete(char *);
int readfile(char *, int, char *);
int writefile(char *, int, char *);
