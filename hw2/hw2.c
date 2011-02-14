#include "hw2.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

#define OPEN "open"
#define REMOVE "remove"
#define SHOW "show"
#define FIND "find"
#define WRITE "write"

int main (int argc, char **argv) {
	
	char cmd[MAXCOMMANDLEN + 1];
	char filename[MAXFILENAME + 1];
	char string[MAXSTRLEN + 1];
	int len;
	file* lst;
	char *data;
	long fsize;
	int res;

	while (1) {
		printf ("$");
		*string = '\0';
		len = nextin(cmd, filename, string);
		if (len < 0) {
			exit(0);
		}

		if (strcmp(cmd, OPEN) == 0) {
			res = create(filename);
			if (res == 0) {
				int length = atoi(string);
				if (length <= 0) {
					printf("please speficy the number of bytes to read from file s%\n", filename);
					continue;
				}
				data = malloc(length);
				fsize = readfile(data, length, filename);
				if (fsize < 0) {
						printf("Error in opening:\n");
				} else {
					printf("%s\n", data);
				}
			} else if(res == -1) {
				printf("Error in opening %s\n", filename);
			} else {
				printf("File %s has been created \n", filename);
			}
		}

		else if (strcmp(cmd, FIND) == 0) {		
			if ((fsize = lookup(filename)) < 0) {
				printf("Could not find %s\n", filename);
			} else {
				printf("File %s has %ld bytes\n", filename, fsize);
			}
		}

		else if (strcmp(cmd, REMOVE) == 0) {
			res = delete(filename);
			if (res == 0) {
				printf("File %s has been removed\n", filename);
			} else if (res == -2) {
				printf("File %s does not exist\n", filename);
			} else {
				printf("Error in deleting %s\n", filename);
			}
		}	
	
		else if (strcmp(cmd, SHOW) == 0) {
			lst = listfiles();
			if (lst == NULL) {
				printf("Error in showing:\n");
			} else {
				for (; lst != (file *) NULL; lst = lst->next) {
					printf("%s\n", lst->filename);
				}
			}
		}
        
         	else if (strcmp(cmd, WRITE) == 0) {
                	fsize = writefile(string, len, filename);
                	if (fsize < 0) {
                        	printf("Error in writing:\n");
                	} else {
				printf("%d bypes of data were written to file %s\n", fsize, filename);
                	}
        	}

		else if (strcmp(cmd, "exit") == 0) {
			printf("exiting...\n");
			exit(0);
		}
	}

}

/* assuming a long integer is enough for the file size
   return -1 on error */
long lookup(char *filename) {
	int status;
	struct stat statbuf;

	if (stat(filename, &statbuf) == -1) {	
		return -1;
	}
	
	return (long)statbuf.st_size;

}

/* length specifies the number of bytes read from a file */
/* the data read from the file is stored in parameter data */
int readfile(char *data, int length, char *filename) {
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		return -1;
	}
	int size = 
		fread(data, sizeof(char), length, fp);
	fclose(fp);
        return size;
}

int writefile(char *data, int length, char *filename) {
	FILE *fp = fopen(filename, "w+");
	if (fp == NULL) {
		return -1;
	}
	int size = fwrite(data, sizeof(char), length, fp);
	fclose(fp);
	return size;
}

int create(char *filename) {
	if (lookup(filename) > -1) {
		return 0;
	}
	return open(filename, O_CREAT);
}

int delete(char *filename) {
	if (lookup(filename) < 0) {
		return -2;
	}
	return remove(filename);
}

file *listfiles() {
	DIR *pdir;
	struct dirent *ent;
	file *tmp;
	static file *lst;

	pdir = opendir("."); /*assuming the simple file system is located in the current directory */
	if (pdir == NULL) {
		return NULL;
	}
	
	lst = (file *) NULL;
	while((ent = readdir(pdir)) != NULL) {
		tmp = (file *) malloc(sizeof(file));
		tmp->filename = (char *) malloc(sizeof(ent->d_name)+1);
		strcpy (tmp->filename, ent->d_name);
		tmp->next = lst;
		lst = tmp;
	}
	closedir(pdir);
	return lst;
}

int nextin (char *cmd, char *name, char *str) {
	int i, ch;

	ch = getc(stdin);
	while (isspace(ch)) {
		ch = getc(stdin);
	}
	if (ch == EOF) {
		return -1;
	}
	i = 0;
	while (!isspace(ch)) {
		if (++i > MAXCOMMANDLEN) {
			printf("Error: Command is too long.\n");
			exit (1);
		}
		*cmd++ = ch;
		ch = getc(stdin);
	}
	*cmd = '\0';

	while (isspace(ch)) {
		if (ch == '\n') {
			*name = '\0';
			return 0;
		}
		ch = getc(stdin);
	}
	i = 0;
	while (!isspace(ch)) {
		if (++i > MAXFILENAME) {
			printf("Error: File name is too long.\n");
			exit (1);
		}
		*name++ = ch;
		ch = getc(stdin);
	}
	*name = '\0';

        while (isspace(ch)) {
                if (ch == '\n') {
                        *str = '\0';
                        return 0;
                }
                ch = getc(stdin);
        }
        i = 0;
        while (ch != '\n') {
                if (++i > MAXSTRLEN) {
                        printf("Error: String is too long.\n");
                        exit (1);
                }
                *str++ = ch;
                ch = getc(stdin);
        }
        *str = '\0';
	fflush(stdin);
	return i;
}
