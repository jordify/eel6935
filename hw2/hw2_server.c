#include "hw2.h"
#include "hw2_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

char **open_1_svc(char *arg1, int length,  struct svc_req *rqstp){
	static char * result;
        char filename[MAXFILENAME+1];

        strcpy(filename, arg1);
        
        printf(filename);
        printf("%d\n", length);
        result = "Open\n";

	return &result;
}

char **find_1_svc(char *arg1,  struct svc_req *rqstp){
	static char * result;

        printf(arg1);
        result = "Find\n";

	return &result;
}

char **remove_1_svc(char *arg1,  struct svc_req *rqstp){
	static char * result;

        printf(arg1);
        result = "Remove\n";

	return &result;
}

char **show_1_svc(char *arg1,  struct svc_req *rqstp){
	static char * result;

        printf(arg1);
        result = "Show\n";

	return &result;
}

char **write_1_svc(char *arg1, int arg2, char *arg3,  struct svc_req *rqstp){
	static char * result;

        printf(arg1);
        printf("%d\n",arg2);
        printf(arg3);
        result = "Write\n";

	return &result;
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
