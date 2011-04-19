#include "hw2.h"
#include "hw2_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <ctype.h>

void hw2_program_1(char *host){
	CLIENT *clnt;
	char * *result;

        char cmd[21];
        char filename[21];
        char strings[21];
	int len;

	clnt = clnt_create (host, HW2_PROGRAM, HW2_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

        while(1){
          printf("$");
          *strings = '\0';
          len = nextin(cmd, filename, strings);
          if (len<0){
            clnt_destroy(clnt);
            printf("\nExiting...\n");
            exit(0);
          }

          if (strcmp(cmd,"open") == 0){

            result = open_1(filename, strings, clnt);
            if (result == (char **) NULL) {
                    clnt_perror (clnt, "call failed");
            }
            printf(*result);
          }
          else if (strcmp(cmd,"find") == 0){

            result = find_1(filename, clnt);
            if (result == (char **) NULL) {
                    clnt_perror (clnt, "call failed");
            }
            printf(*result);
          }
          else if (strcmp(cmd,"remove") == 0){

            result = remove_1(filename, clnt);
            if (result == (char **) NULL) {
                    clnt_perror (clnt, "call failed");
            }
            printf(*result);
          }
          else if (strcmp(cmd,"show") == 0){

            result = show_1(filename, clnt);
            if (result == (char **) NULL) {
                    clnt_perror (clnt, "call failed");
            }
            printf(*result);
          }
          else if (strcmp(cmd,"write") == 0){

            result = write_1(strings, len, filename, clnt);
            if (result == (char **) NULL) {
                    clnt_perror (clnt, "call failed");
            }
            printf(*result);
          }
          else if(strcmp(cmd,"exit") == 0){
            clnt_destroy(clnt);
            printf("\nExiting...\n");
            exit(0);
          }

        }
}

int main (int argc, char *argv[]){
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	hw2_program_1 (host);
  exit(0);
}

int nextin(char *cmd, char *name, char *str){
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
