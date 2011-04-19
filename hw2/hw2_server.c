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

char **open_1_svc(char *arg1, char *arg2,  struct svc_req *rqstp){
	static char * result;
        char filename[MAXFILENAME+1]; 
        char strings[MAXSTRLEN+1];
	char *data;
	int res;
        long fsize;
        strcpy(filename, arg1);
        if (arg2 == NULL){
          strcpy(strings, "0"); 
        } else{
          strcpy(strings, arg2); 
        }

        if (result == (char *)NULL){
          result = (char *)malloc(MAXSTRLEN+1);
        }
        else{
          result = (char *)realloc(result,MAXSTRLEN+1);
        }
        
        res = create(filename);
        if (res == 0) {
            int length = atoi(strings);
            if (length <= 0) {
                    sprintf(result,"please speficy the number of bytes to read from file %s\n", filename);
            }
            data = (char *)malloc(length);
            fsize = readfile(data, length, filename);
            if (fsize < 0) {
                    sprintf(result,"Error in opening:\n");
            } else {
                    sprintf(result,"%s\n", data);
            }
            free(data);
        } else if(res == -1) {
                sprintf(result,"Error in opening %s\n", filename);
        } else {
                sprintf(result,"File %s has been created \n", filename);
        }

	return &result;
}

char **find_1_svc(char *arg1,  struct svc_req *rqstp){
	static char * result;
        char filename[MAXFILENAME+1]; 
        strcpy(filename, arg1);
        long fsize;

        if (result == (char *)NULL){
          result = (char *)malloc(MAXSTRLEN+1);
        }
        else{
          result = (char *)realloc(result,MAXSTRLEN+1);
        }
        
        if ((fsize = lookup(filename)) < 0) {
                sprintf(result,"Could not find %s\n", filename);
        } else {
                sprintf(result,"File %s has %ld bytes\n", filename, fsize);
        }

	return &result;
}

char **remove_1_svc(char *arg1,  struct svc_req *rqstp){
	static char * result;
        char filename[MAXFILENAME+1]; 
        strcpy(filename, arg1);
        int res;

        if (result == (char *)NULL){
          result = (char *)malloc(MAXSTRLEN+1);
        }
        else{
          result = (char *)realloc(result,MAXSTRLEN+1);
        }
        res = delete(filename);
        if (res == 0) {
                sprintf(result,"File %s has been removed\n", filename);
        } else if (res == -2) {
                sprintf(result,"File %s does not exist\n", filename);
        } else {
                sprintf(result,"Error in deleting %s\n", filename);
        }
        
	return &result;
}

char **show_1_svc(char *arg1,  struct svc_req *rqstp){
	static char * result;
	file* lst;

        if (result == (char *)NULL){
          result = (char *)calloc(MAXSTRLEN+1,1);
        }
        else{
          free(result);
          result = (char *)calloc(MAXSTRLEN+1,1);
        }

        lst = listfiles();
        if (lst == NULL) {
                printf("Error in showing:\n");
        } else {
                for (; lst != (file *) NULL; lst = lst->next) {
                        sprintf(result,"%s%s\n", result,lst->filename);
                }
        }

	return &result;
}

char **write_1_svc(char *arg1, int arg2, char *arg3,  struct svc_req *rqstp){
	static char * result;
        char filename[MAXFILENAME+1]; 
        char strings[MAXSTRLEN+1];
        long fsize;

        strcpy(filename, arg3);
        if (arg1 == NULL){
          strcpy(strings, "0"); 
        } else{
          strcpy(strings, arg1); 
        }

        if (result == (char *)NULL){
          result = (char *)malloc(MAXSTRLEN+1);
        }
        else{
          result = (char *)realloc(result,MAXSTRLEN+1);
        }

        fsize = writefile(strings, arg2, filename);
        if (fsize < 0) {
                sprintf(result,"Error in writing:\n");
        } else {
                sprintf(result,"%ld bytes of data were written to file %s\n", fsize, filename);
        }

	return &result;
}

/* assuming a long integer is enough for the file size
   return -1 on error */
long lookup(char *filename) {
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
